#include <cassert>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
	DeviceMemoryBlock::DeviceMemoryBlock(DeviceMemoryChunk* chunk, VkDeviceSize offset, VkDeviceSize size, void* ptr) :
		mChunk(chunk),
		mOffset(offset),
		mPadding(0),
		mSize(size),
		mPtr(ptr),
		mFree(true)
	{
		/// Nothing
	}

	VkDeviceSize DeviceMemoryBlock::ComputeAllocationPadding(VkDeviceSize alignment) const
	{
		return mOffset != 0 && alignment != 0 ? alignment - (mOffset % alignment) : 0;
	}

	void DeviceMemoryBlock::BindToBuffer(const RenderDevice& renderDevice, const Buffer& buffer) const
	{
		COCKTAIL_VK_CHECK(vkBindBufferMemory(renderDevice.GetHandle(), buffer.GetHandle(), GetChunk()->GetDeviceMemory()->GetHandle(), mOffset + mPadding));
	}

	void DeviceMemoryBlock::BindToTexture(const RenderDevice& renderDevice, const AbstractTexture& texture) const
	{
		COCKTAIL_VK_CHECK(vkBindImageMemory(renderDevice.GetHandle(), texture.GetHandle(), GetChunk()->GetDeviceMemory()->GetHandle(), mOffset + mPadding));
	}

	ObjectPool<DeviceMemoryBlock>::UniquePtr DeviceMemoryBlock::Split(ObjectPool<DeviceMemoryBlock>& blockPool, VkDeviceSize alignment, VkDeviceSize size)
	{
		assert(mFree);

		mPadding = ComputeAllocationPadding(alignment);
		assert(mPadding + size <= mSize);

		VkDeviceSize oldSize = mSize;
		mSize = mPadding + size;

		if (mSize == oldSize)
			return nullptr;

		void* ptr = nullptr;
		if (mPtr)
			ptr = static_cast<unsigned char*>(mPtr) + mSize;

		return blockPool.AllocateUnique(mChunk, mOffset + mSize, oldSize - mSize, ptr);
	}

	void DeviceMemoryBlock::Merge(const DeviceMemoryBlock& block)
	{
		assert(mOffset + mSize == block.mOffset);
		mSize += block.mSize;
	}

	void* DeviceMemoryBlock::Map(VkDeviceSize offset) const
	{
		if (!mPtr)
			return nullptr;

		return static_cast<unsigned char*>(mPtr) + mPadding + offset;
	}

	void DeviceMemoryBlock::Acquire()
	{
		mFree = false;
	}

	void DeviceMemoryBlock::Release()
	{
		mFree = true;
	}

	DeviceMemoryChunk* DeviceMemoryBlock::GetChunk() const
	{
		return mChunk;
	}

	VkDeviceSize DeviceMemoryBlock::GetSize() const
	{
		return mSize;
	}

	bool DeviceMemoryBlock::IsFree() const
	{
		return mFree;
	}
}
