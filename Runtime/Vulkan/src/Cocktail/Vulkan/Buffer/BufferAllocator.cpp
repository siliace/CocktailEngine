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
		mAcquiredBufferPools.clear();
		mAvailableBufferPools.clear();
		mBufferPool.Clear();
    }

    Renderer::BufferArea BufferAllocator::PushData(std::size_t size, const void *data)
    {
		std::shared_ptr<BufferPool> bufferPool = AcquirePool(size);

		Renderer::BufferArea bufferArea;
		bufferArea.Buffer = bufferPool.get();
		bufferArea.BaseOffset = bufferPool->PushData(mMinAlignment, size, data);
		bufferArea.Range = size;

		return bufferArea;
	}

	void BufferAllocator::Reserve(std::size_t size)
	{
		for (std::shared_ptr<BufferPool> acquiredBufferPools : mAcquiredBufferPools)
		{
			if (size < acquiredBufferPools->GetRemainingCapacity())
				return;
		}

		for (std::shared_ptr<BufferPool> availableBufferPool : mAvailableBufferPools)
		{
			if (size < availableBufferPool->GetRemainingCapacity())
				return;
		}

		AcquirePool(size);
	}

	void BufferAllocator::Reset(bool release)
	{
		if (!release)
		{
			for (std::shared_ptr<BufferPool> bufferPool : mAcquiredBufferPools)
			{
				bufferPool->Reset();
				mAvailableBufferPools.push_back(bufferPool);
			}
		}
		else
		{
			mAvailableBufferPools.clear();
		}

		mAcquiredBufferPools.clear();
		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}

	std::size_t BufferAllocator::GetBufferSize() const
	{
		return mBufferSize;
	}

	std::shared_ptr<BufferPool> BufferAllocator::AcquirePool(std::size_t size)
	{
		for (std::shared_ptr<BufferPool> buffer : mAcquiredBufferPools)
		{
			std::size_t padding = buffer->ComputePadding(mMinAlignment);
			if (padding + size <= buffer->GetRemainingCapacity())
				return buffer;
		}

		auto itBuffer = std::find_if(mAvailableBufferPools.begin(), mAvailableBufferPools.end(), [&](std::shared_ptr<BufferPool> bufferPool) {
			return bufferPool->GetRemainingCapacity() + bufferPool->ComputePadding(mMinAlignment) > size;
		});

		std::shared_ptr<BufferPool> buffer;
		if (itBuffer != mAvailableBufferPools.end())
		{
			buffer = std::move(*itBuffer);
			mAvailableBufferPools.erase(itBuffer);
		}
		else
		{
			Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = mUsage;
			bufferCreateInfo.Size = std::max(mBufferSize, size);
			bufferCreateInfo.MemoryType = mMemoryType;
			bufferCreateInfo.Exclusive = true;

			buffer = mBufferPool.Allocate(mRenderDevice, bufferCreateInfo, nullptr);
		}

		mAcquiredBufferPools.push_back(buffer);

		return buffer;
	}
}
