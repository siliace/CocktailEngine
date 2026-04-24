#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
    namespace
    {
        float GetMemoryPriorityFactor(Renderer::MemoryPriority p)
        {
            switch (p)
            {
                case Renderer::MemoryPriority::Critical: return 1.0f;
                case Renderer::MemoryPriority::High: return 0.8f;
                case Renderer::MemoryPriority::Medium: return 0.5f;
                case Renderer::MemoryPriority::Low: return 0.3f;
                case Renderer::MemoryPriority::VeryLow: return 0.1f;
                case Renderer::MemoryPriority::Streaming: return 0.0f;
                default: return 0.5f;
            }
        }
    }

	DeviceMemoryChunk::DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
        mMemoryPriority(priority),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(false),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(nullptr, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const AbstractTexture* texture, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(true),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(texture, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const Buffer* buffer, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(true),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(nullptr, buffer);
	}

	DeviceMemoryBlock* DeviceMemoryChunk::AllocateBlock(std::size_t alignment, std::size_t size)
	{
		return mBlocks.FindIndexIf([&](const auto& block) {
			if (!block->IsFree())
				return false;

			std::size_t padding = block->ComputeAllocationPadding(alignment);
			std::size_t allocationSize = size + padding;

			return block->GetSize() >= allocationSize;
		}).Map([&](unsigned int index) {
			DeviceMemoryBlock* allocatedBlock = mBlocks[index].Get();
			ObjectPool<DeviceMemoryBlock>::Unique remainingBlock = allocatedBlock->Split(mBlockPool, alignment, size);
			if (remainingBlock)
				mBlocks.AddAt(index + 1, std::move(remainingBlock));

			allocatedBlock->Acquire();

			return allocatedBlock;
		}).GetOr(nullptr);
	}

	void DeviceMemoryChunk::Compact()
	{
		for (unsigned int i = 0; i < mBlocks.GetSize();)
		{
			DeviceMemoryBlock* block = mBlocks[i].Get();
			DeviceMemoryBlock* nextBlock = mBlocks.TryAt(i + 1).GetOr(nullptr).Get();

			if (block->IsFree() && nextBlock && nextBlock->IsFree())
			{
				block->Merge(*nextBlock);
				mBlocks.RemoveAt(i + 1);
			}
			else
			{
				++i;
			}
		}
	}

	bool DeviceMemoryChunk::IsFree() const
	{
		return mBlocks.AllOf([](const ObjectPool<DeviceMemoryBlock>::Unique& block) {
			return block->IsFree();
		});
	}

    Renderer::MemoryPriority DeviceMemoryChunk::GetMemoryPriority() const
    {
        return mMemoryPriority;
    }

    std::size_t DeviceMemoryChunk::GetSize() const
	{
		return mSize;
	}

	unsigned int DeviceMemoryChunk::GetMemoryTypeIndex() const
	{
		return mMemoryTypeIndex;
	}

	bool DeviceMemoryChunk::IsDedicated() const
	{
		return mDedicated;
	}

	std::shared_ptr<DeviceMemory> DeviceMemoryChunk::GetDeviceMemory() const
	{
		return mDeviceMemory;
	}

	void DeviceMemoryChunk::AllocateDeviceMemory(const AbstractTexture* texture, const Buffer* buffer)
	{
		DeviceMemoryCreateInfo createInfo;
        createInfo.Priority = GetMemoryPriorityFactor(mMemoryPriority);
		createInfo.Size = mSize;
		createInfo.MemoryTypeIndex = mMemoryTypeIndex;
		createInfo.Dedicated = mDedicated;
		createInfo.Texture = texture;
		createInfo.Buffer = buffer;

		mDeviceMemory = mRenderDevice->CreateDeviceMemory(createInfo);

		VkPhysicalDevice physicalDevice = mRenderDevice->GetPhysicalDeviceHandle();

		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);
		if (memoryProperties.memoryTypes[mMemoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			vkMapMemory(mRenderDevice->GetHandle(), mDeviceMemory->GetHandle(), 0, VK_WHOLE_SIZE, 0, &mPtr);

		mBlocks.Add(mBlockPool.AllocateUnique(this, 0, mSize, mPtr));
	}
}
