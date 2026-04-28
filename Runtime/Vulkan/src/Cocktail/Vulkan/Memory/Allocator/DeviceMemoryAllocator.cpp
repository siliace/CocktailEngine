#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
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

		if (compact)
		{
			mChunks.ForEach([](const ObjectPool<DeviceMemoryChunk>::Unique& chunk) {
				chunk->Compact();
			});
		}
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
	    DeviceMemoryBlock* block;
	    for (auto& chunk : mChunks)
	    {
	        if (chunk->IsDedicated())
	            continue;

	        if (chunk->GetMemoryTypeIndex() != memoryTypeIndex)
	            continue;

	        if (chunk->GetMemoryPriority() != priority)
	            continue;

	        block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);
            if (block == nullptr)
                continue;

	        return block;
	    }

	    auto chunk = mChunkPool.AllocateUnique(mRenderDevice, mBlockPool, priority, std::max(memoryRequirements.size, mDefaultChunkSize), memoryTypeIndex);
	    block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);

	    mChunks.Add(std::move(chunk));

	    return block;
    }
}
