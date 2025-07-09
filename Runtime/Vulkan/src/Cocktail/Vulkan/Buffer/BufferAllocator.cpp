#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Buffer/BufferAllocator.hpp>
#include <Cocktail/Vulkan/Buffer/BufferPool.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	BufferAllocator::BufferAllocator(std::shared_ptr<RenderDevice> renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType) :
		mRenderDevice(std::move(renderDevice)),
		mUsage(usage),
		mBufferSize(bufferSize),
		mMemoryType(memoryType),
		mMinAlignment(4)
	{
		if (mUsage & Renderer::BufferUsageFlagBits::Storage | Renderer::BufferUsageFlagBits::Uniform)
		{
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(mRenderDevice->GetPhysicalDeviceHandle(), &physicalDeviceProperties);

			if (mUsage & Renderer::BufferUsageFlagBits::Storage)
				mMinAlignment = std::max(mMinAlignment, physicalDeviceProperties.limits.minStorageBufferOffsetAlignment);

			if (mUsage & Renderer::BufferUsageFlagBits::Uniform)
				mMinAlignment = std::max(mMinAlignment, physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
		}
	}

    BufferAllocator::~BufferAllocator()
    {
		Reset(true);
    }

    Renderer::BufferArea BufferAllocator::PushData(std::size_t size, const void *data)
    {
		BufferPool* bufferPool = AcquirePool(size);

		Renderer::BufferArea bufferArea;
		bufferArea.Buffer = bufferPool;
		bufferArea.BaseOffset = bufferPool->PushData(mMinAlignment, size, data);
		bufferArea.Range = size;

		return bufferArea;
	}

	void BufferAllocator::Reserve(std::size_t size)
	{
		for (BufferPool* acquiredBufferPools : mAcquiredBufferPools)
		{
			if (size < acquiredBufferPools->GetRemainingCapacity())
				return;
		}

		for (BufferPool* availableBufferPool : mAvailableBufferPools)
		{
			if (size < availableBufferPool->GetRemainingCapacity())
				return;
		}

		AcquirePool(size);
	}

	void BufferAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBufferPools.IsEmpty())
		{
			for (BufferPool* bufferPool : mAcquiredBufferPools)
			{
				bufferPool->Reset();
				mAvailableBufferPools.Add(bufferPool);
			}
		}
		else
		{
			for (BufferPool* bufferPool : mAcquiredBufferPools)
				mBufferPool.Recycle(bufferPool);

			for (BufferPool* bufferPool : mAvailableBufferPools)
				mBufferPool.Recycle(bufferPool);

			mAvailableBufferPools.Clear();
		}

		mAcquiredBufferPools.Clear();
		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}

	std::size_t BufferAllocator::GetBufferSize() const
	{
		return mBufferSize;
	}

	BufferPool* BufferAllocator::AcquirePool(std::size_t size)
	{
		for (BufferPool* buffer : mAcquiredBufferPools)
		{
			std::size_t padding = buffer->ComputePadding(mMinAlignment);
			if (padding + size <= buffer->GetRemainingCapacity())
				return buffer;
		}

		BufferPool* buffer = mAvailableBufferPools.FindIndexIf([&](BufferPool* bufferPool) {
			return bufferPool->GetRemainingCapacity() + bufferPool->ComputePadding(mMinAlignment) > size;
		}).Map([&](unsigned int index) {
			return mAvailableBufferPools.RemoveAt(index);
		}).GetOrElse([&]() {
			Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = mUsage;
			bufferCreateInfo.Size = std::max(mBufferSize, size);
			bufferCreateInfo.MemoryType = mMemoryType;
			bufferCreateInfo.Exclusive = true;

			return mBufferPool.AllocateUnsafe(mRenderDevice, bufferCreateInfo, nullptr);
		});

		mAcquiredBufferPools.Add(buffer);

		return buffer;
	}
}
