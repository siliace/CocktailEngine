#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
	DeviceMemoryChunk::DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(false),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(nullptr, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const AbstractTexture* texture, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(true),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(texture, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(std::shared_ptr<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const Buffer* buffer, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(true),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(nullptr, buffer);
	}

	DeviceMemoryChunk::~DeviceMemoryChunk()
	{
		mBlocks.ForEach([&](DeviceMemoryBlock* deviceMemoryBlock) {
			mBlockPool.Recycle(deviceMemoryBlock);
		});
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
			DeviceMemoryBlock* allocatedBlock = mBlocks[index];
			DeviceMemoryBlock* remainingBlock = allocatedBlock->Split(mBlockPool, alignment, size);
			if (remainingBlock)
				mBlocks.AddAt(index + 1, remainingBlock);

			allocatedBlock->Acquire();

			return allocatedBlock;
		}).GetOr(nullptr);
	}

	void DeviceMemoryChunk::Compact()
	{
		for (unsigned int i = 0; i < mBlocks.GetSize();)
		{
			DeviceMemoryBlock* block = mBlocks[i];
			DeviceMemoryBlock* nextBlock = mBlocks.TryAt(i + 1).GetOr(nullptr);

			if (block->IsFree() && nextBlock && nextBlock->IsFree())
			{
				block->Merge(*nextBlock);
				mBlocks.RemoveAt(i + 1);

				mBlockPool.Recycle(nextBlock);
			}
			else
			{
				++i;
			}
		}
	}

	bool DeviceMemoryChunk::IsFree() const
	{
		return mBlocks.AllOf([](const DeviceMemoryBlock* block) {
			return block->IsFree();
		});
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

		mBlocks.Add(mBlockPool.AllocateUnsafe(this, 0, mSize, mPtr));
	}
}
