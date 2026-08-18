#include <cassert>

#include <CocktailEngine/Core/Log/Log.hpp>

#include <CocktailEngine/Vulkan/Buffer/Buffer.hpp>
#include <CocktailEngine/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>
#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Vulkan.hpp>
#include <CocktailEngine/Vulkan/VulkanUtils.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		/**
		 * \brief Fraction of the remaining heap budget a single chunk may reserve
		 *
		 * A chunk is a speculative reservation: it is sized to serve allocations that have not
		 * been requested yet. Spending everything that is left on the first one to ask defeats
		 * that, so a chunk is capped to a quarter of what remains once the heap gets tight.
		 */
		constexpr VkDeviceSize MaxChunkBudgetDivisor = 4;

		/**
		 * \brief Queries the memory properties of a physical device with the budget chained in
		 *
		 * The budget is re-queried every time it is needed rather than cached: it moves as the
		 * rest of the system allocates and frees, so a stored value describes a state that no
		 * longer holds.
		 */
		bool QueryMemoryProperties(const RenderDevice& renderDevice, VkPhysicalDeviceMemoryProperties2KHR& memoryProperties, VkPhysicalDeviceMemoryBudgetPropertiesEXT& budgetProperties)
		{
			if (!renderDevice.IsFeatureSupported(RenderDeviceFeature::MemoryBudget))
				return false;

			memoryProperties = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2_KHR, nullptr };
			budgetProperties = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT, nullptr };
			Chain(memoryProperties, budgetProperties);

			vkGetPhysicalDeviceMemoryProperties2KHR(renderDevice.GetPhysicalDeviceHandle(), &memoryProperties);

			return true;
		}
	}

	DeviceMemoryAllocator::DeviceMemoryAllocator(RenderDevice* renderDevice, std::size_t defaultChunkSize) :
		mRenderDevice(renderDevice),
		mDefaultChunkSize(defaultChunkSize)
	{
		/// Nothing
	}

	DeviceMemoryAllocator::~DeviceMemoryAllocator()
	{
		mChunks.Clear();

		mChunkPool.Clear();
		mBlockPool.Clear();
	}

	DeviceMemoryBlock* DeviceMemoryAllocator::Allocate(const AbstractTexture& texture)
	{
		bool dedicated;
		VkMemoryRequirements memoryRequirements;
		GetTextureMemoryRequirements(texture, memoryRequirements, dedicated);

		unsigned int memoryTypeIndex = FindMemoryTypeIndex(memoryRequirements.memoryTypeBits, texture.GetMemoryType());

		DeviceMemoryBlock* block;
	    if (dedicated)
	    {
	        block = AllocateDedicatedBlock(&texture, texture.GetPriority(), memoryRequirements, memoryTypeIndex);
	    }
	    else
	    {
	        block = AllocateBlock(texture.GetPriority(), memoryRequirements, memoryTypeIndex);
	    }

		block->BindToTexture(*mRenderDevice, texture);

		return block;
	}

	DeviceMemoryBlock* DeviceMemoryAllocator::Allocate(const Buffer& buffer)
	{
		bool dedicated;
		VkMemoryRequirements memoryRequirements;
		GetBufferMemoryRequirements(buffer, memoryRequirements, dedicated);

		unsigned int memoryTypeIndex = FindMemoryTypeIndex(memoryRequirements.memoryTypeBits, buffer.GetMemoryType());

	    DeviceMemoryBlock* block;
	    if (dedicated)
	    {
		    block = AllocateDedicatedBlock(&buffer, buffer.GetPriority(), memoryRequirements, memoryTypeIndex);
	    }
	    else
	    {
	        block = AllocateBlock(buffer.GetPriority(), memoryRequirements, memoryTypeIndex);
	    }

		block->BindToBuffer(*mRenderDevice, buffer);

		return block;
	}

	void DeviceMemoryAllocator::GarbageCollect(bool compact)
	{
		mChunks.RemoveIf([&](const ObjectPool<DeviceMemoryChunk>::Unique& chunk) {
			return chunk->IsFree();
		});

		// Compaction pays for itself once a heap is over budget, whatever the caller asked for:
		// what the driver does instead is move the excess to host memory, silently.
		if (!compact)
			compact = IsAnyHeapOverBudget();

		if (compact)
		{
			mChunks.ForEach([](const ObjectPool<DeviceMemoryChunk>::Unique& chunk) {
				chunk->Compact();
			});
		}
	}

	DeviceMemoryAllocator::HeapBudget DeviceMemoryAllocator::QueryHeapBudget(unsigned int memoryTypeIndex) const
	{
		HeapBudget heapBudget;

		VkPhysicalDeviceMemoryProperties2KHR memoryProperties;
		VkPhysicalDeviceMemoryBudgetPropertiesEXT budgetProperties;
		if (!QueryMemoryProperties(*mRenderDevice, memoryProperties, budgetProperties))
			return heapBudget;

		assert(memoryTypeIndex < memoryProperties.memoryProperties.memoryTypeCount);

		const unsigned int heapIndex = memoryProperties.memoryProperties.memoryTypes[memoryTypeIndex].heapIndex;

		heapBudget.Known = true;
		heapBudget.Budget = budgetProperties.heapBudget[heapIndex];
		heapBudget.Usage = budgetProperties.heapUsage[heapIndex];

		return heapBudget;
	}

	bool DeviceMemoryAllocator::IsAnyHeapOverBudget() const
	{
		VkPhysicalDeviceMemoryProperties2KHR memoryProperties;
		VkPhysicalDeviceMemoryBudgetPropertiesEXT budgetProperties;
		if (!QueryMemoryProperties(*mRenderDevice, memoryProperties, budgetProperties))
			return false;

		for (unsigned int i = 0; i < memoryProperties.memoryProperties.memoryHeapCount; i++)
		{
			if (budgetProperties.heapUsage[i] > budgetProperties.heapBudget[i])
				return true;
		}

		return false;
	}

	void DeviceMemoryAllocator::GetBufferMemoryRequirements(const Buffer& buffer, VkMemoryRequirements& requirements, bool& dedicated) const
	{
		const bool supportedDedicatedAllocation = mRenderDevice->IsFeatureSupported(RenderDeviceFeature::DedicatedAllocation);
		if (supportedDedicatedAllocation)
		{
			VkMemoryRequirements2KHR memoryRequirements{ VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR, nullptr };
			VkMemoryDedicatedRequirementsKHR memoryDedicatedRequirements{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR, nullptr };
			Chain(memoryRequirements, memoryDedicatedRequirements);

			VkBufferMemoryRequirementsInfo2KHR memoryRequirementsInfo{ VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2_KHR, nullptr };
			{
				memoryRequirementsInfo.buffer = buffer.GetHandle();
			}
			vkGetBufferMemoryRequirements2KHR(mRenderDevice->GetHandle(), &memoryRequirementsInfo, &memoryRequirements);

			dedicated = memoryDedicatedRequirements.prefersDedicatedAllocation || memoryDedicatedRequirements.requiresDedicatedAllocation;
			requirements = memoryRequirements.memoryRequirements;
		}
		else
		{
			dedicated = false;
			vkGetBufferMemoryRequirements(mRenderDevice->GetHandle(), buffer.GetHandle(), &requirements);
		}
	}

	void DeviceMemoryAllocator::GetTextureMemoryRequirements(const AbstractTexture& texture, VkMemoryRequirements& requirements, bool& dedicated) const
	{
		const bool supportedDedicatedAllocation = mRenderDevice->IsFeatureSupported(RenderDeviceFeature::DedicatedAllocation);
		if (supportedDedicatedAllocation)
		{
			VkMemoryRequirements2KHR memoryRequirements{ VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR, nullptr };
			VkMemoryDedicatedRequirementsKHR memoryDedicatedRequirements{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR, nullptr };
			Chain(memoryRequirements, memoryDedicatedRequirements);

			VkImageMemoryRequirementsInfo2KHR memoryRequirementsInfo{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2_KHR, nullptr };
			{
				memoryRequirementsInfo.image = texture.GetHandle();
			}
			vkGetImageMemoryRequirements2KHR(mRenderDevice->GetHandle(), &memoryRequirementsInfo, &memoryRequirements);

			dedicated = memoryDedicatedRequirements.prefersDedicatedAllocation || memoryDedicatedRequirements.requiresDedicatedAllocation;
			requirements = memoryRequirements.memoryRequirements;
		}
		else
		{
			dedicated = false;
			vkGetImageMemoryRequirements(mRenderDevice->GetHandle(), texture.GetHandle(), &requirements);
		}
	}

	unsigned int DeviceMemoryAllocator::FindMemoryTypeIndex(unsigned int typeIndexMask, Renderer::MemoryType memoryType) const
	{
		VkPhysicalDevice physicalDevice = mRenderDevice->GetPhysicalDeviceHandle();

		VkMemoryPropertyFlags memoryTypeProperties = 0;
		switch (memoryType)
		{
		case Renderer::MemoryType::Static:
			memoryTypeProperties |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			break;

		case Renderer::MemoryType::Dynamic:
			memoryTypeProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			break;

		case Renderer::MemoryType::Unified:
			memoryTypeProperties |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			memoryTypeProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			break;
		}

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
		for (unsigned int i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			if (!(typeIndexMask & Bit(i)))
				continue;

			if (memoryProperties.memoryTypes[i].propertyFlags != memoryTypeProperties)
				continue;

			return i;
		}

		return 0;
	}

    DeviceMemoryBlock* DeviceMemoryAllocator::AllocateBlock(Renderer::MemoryPriority priority, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex)
    {
	    if (DeviceMemoryBlock* block = FindBlock(priority, memoryRequirements, memoryTypeIndex))
	        return block;

        // Past this point the allocator grows, which means one more VkDeviceMemory, and that is
        // the only moment the heap budget can still change the outcome. The budget is queried here
        // rather than cached because it moves with what the rest of the system allocates, and the
        // cost of the query is negligible next to the vkAllocateMemory it guards.
	    HeapBudget budget = QueryHeapBudget(memoryTypeIndex);

        const std::size_t wantedChunkSize = std::max<std::size_t>(memoryRequirements.size, mDefaultChunkSize);
	    if (budget.Known && budget.GetAvailable() < wantedChunkSize)
	    {
            // Reclaiming what is already held is cheaper than a chunk the heap cannot hold, which
            // most drivers accept and then back with host memory instead of failing.
	        GarbageCollect(true);

	        if (DeviceMemoryBlock* block = FindBlock(priority, memoryRequirements, memoryTypeIndex))
	            return block;

	        budget = QueryHeapBudget(memoryTypeIndex);
	    }

	    const std::size_t chunkSize = ChooseChunkSize(memoryRequirements.size, budget);
	    ReportBudgetPressure(memoryTypeIndex, budget, chunkSize);

	    auto chunk = mChunkPool.AllocateUnique(mRenderDevice, mBlockPool, priority, chunkSize, memoryTypeIndex);
	    DeviceMemoryBlock* block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);

	    mChunks.Add(Move(chunk));

	    return block;
    }

    DeviceMemoryBlock* DeviceMemoryAllocator::FindBlock(Renderer::MemoryPriority priority, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex)
    {
	    for (auto& chunk : mChunks)
	    {
	        if (chunk->IsDedicated())
	            continue;

	        if (chunk->GetMemoryTypeIndex() != memoryTypeIndex)
	            continue;

	        if (chunk->GetMemoryPriority() != priority)
	            continue;

	        DeviceMemoryBlock* block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);
	        if (block == nullptr)
	            continue;

	        return block;
	    }

	    return nullptr;
    }

    std::size_t DeviceMemoryAllocator::ChooseChunkSize(VkDeviceSize requiredSize, const HeapBudget& budget) const
    {
	    std::size_t chunkSize = mDefaultChunkSize;

	    if (budget.Known)
	    {
	        // Carving the full default chunk out of a heap that has little left is precisely what
	        // pushes it over budget. Staying well under what remains leaves room for the
	        // allocations that follow, and for the driver's own bookkeeping.
	        const VkDeviceSize budgetedChunkSize = budget.GetAvailable() / MaxChunkBudgetDivisor;
	        if (budgetedChunkSize < chunkSize)
	            chunkSize = budgetedChunkSize;
	    }

	    // The requirement is a floor: a chunk unable to hold the allocation it exists for is
	    // useless, budget or not. Going over the budget here is deliberate, and reported.
	    return std::max<std::size_t>(requiredSize, chunkSize);
    }

    void DeviceMemoryAllocator::ReportBudgetPressure(unsigned int memoryTypeIndex, const HeapBudget& budget, std::size_t chunkSize)
    {
	    if (!budget.Known)
	        return;

	    if (budget.GetAvailable() >= chunkSize)
	    {
	        mBudgetPressureReported = false;
	        return;
	    }

	    // Once per episode: under pressure this path is taken by every allocation, and the useful
	    // signal is the transition, not its repetition.
	    if (mBudgetPressureReported)
	        return;

	    mBudgetPressureReported = true;

	    CK_LOG(
	        VulkanLogCategory, LogLevel::Warning,
	        CK_TEXT("Memory type %u exceeds the budget of its heap: %llu MiB used, %llu MiB granted. Further allocations are likely to be backed by host memory, without the driver reporting it."),
	        memoryTypeIndex, budget.Usage / (1024llu * 1024llu), budget.Budget / (1024llu * 1024llu)
	    );
    }
}
