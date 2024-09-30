#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
	DeviceMemoryChunk::DeviceMemoryChunk(Ref<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(false),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(nullptr, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(Ref<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const AbstractTexture* texture, std::size_t size, unsigned int memoryTypeIndex) :
		mRenderDevice(std::move(renderDevice)),
		mBlockPool(blockPool),
		mSize(size),
		mMemoryTypeIndex(memoryTypeIndex),
		mDedicated(true),
		mPtr(nullptr)
	{
		AllocateDeviceMemory(texture, nullptr);
	}

	DeviceMemoryChunk::DeviceMemoryChunk(Ref<RenderDevice> renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const Buffer* buffer, std::size_t size, unsigned int memoryTypeIndex) :
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
		auto it = std::find_if(mBlocks.begin(), mBlocks.end(), [&](const auto& block) {
			if (!block->IsFree())
				return false;

			std::size_t padding = block->ComputeAllocationPadding(alignment);
			std::size_t allocationSize = size + padding;
			
			return block->GetSize() >= allocationSize;
		});

		if (it == mBlocks.end())
			return nullptr;

		DeviceMemoryBlock* allocatedBlock = it->Get();
		Ref<DeviceMemoryBlock> remainingBlock = allocatedBlock->Split(mBlockPool, alignment, size);
		if (remainingBlock)
			mBlocks.insert(std::next(it), std::move(remainingBlock));

		allocatedBlock->Acquire();
		
		return allocatedBlock;
	}

	void DeviceMemoryChunk::Compact()
	{
		for (auto it = mBlocks.begin(); it != mBlocks.end();)
		{
			auto itNext = std::next(it);
			DeviceMemoryBlock* block = it->Get();
			if (block->IsFree() && itNext != mBlocks.end() && (*itNext)->IsFree())
			{
				block->Merge(**itNext);
				it = mBlocks.erase(itNext);
			}
			else
			{
				it = itNext;
			}
		}
	}

	bool DeviceMemoryChunk::IsFree() const
	{
		return std::all_of(mBlocks.begin(), mBlocks.end(), [](const auto& block) {
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

	const Ref<DeviceMemory>& DeviceMemoryChunk::GetDeviceMemory() const
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

		mBlocks.push_back(mBlockPool.Allocate(this, 0, mSize, mPtr));
	}
}
