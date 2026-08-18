#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Buffer/BufferAllocator.hpp>
#include <CocktailEngine/Vulkan/Buffer/BufferPool.hpp>
#include <CocktailEngine/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	BufferAllocator::BufferAllocator(RenderDevice* renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType) :
		mRenderDevice(renderDevice),
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
		bufferArea.BufferResource = bufferPool;
		bufferArea.BaseOffset = bufferPool->PushData(mMinAlignment, size, data);
		bufferArea.Range = size;

		return bufferArea;
	}

	void BufferAllocator::Reserve(std::size_t size)
	{
		for (const ObjectPoolUniquePtr<BufferPool>& acquiredBufferPool : mAcquiredBufferPools)
		{
			if (size < acquiredBufferPool->GetRemainingCapacity())
				return;
		}

		for (const ObjectPoolUniquePtr<BufferPool>& availableBufferPool : mAvailableBufferPools)
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
			for (ObjectPoolUniquePtr<BufferPool>& bufferPool : mAcquiredBufferPools)
			{
				bufferPool->Reset();
				mAvailableBufferPools.Add(Move(bufferPool));
			}
		}
		else
		{
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
		for (const ObjectPoolUniquePtr<BufferPool>& buffer : mAcquiredBufferPools)
		{
			std::size_t padding = buffer->ComputePadding(mMinAlignment);
			if (padding + size <= buffer->GetRemainingCapacity())
				return buffer.Get();
		}

		ObjectPoolUniquePtr<BufferPool> bufferPool = mAvailableBufferPools.FindIndexIf([&](const ObjectPoolUniquePtr<BufferPool>& bufferPool) {
			return bufferPool->GetRemainingCapacity() + bufferPool->ComputePadding(mMinAlignment) > size;
		}).Map([&](unsigned int index) {
			return mAvailableBufferPools.RemoveAt(index);
		}).GetOrElse([&]() -> ObjectPoolUniquePtr<BufferPool> {
			Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = mUsage;
			bufferCreateInfo.Size = std::max(mBufferSize, size);
			bufferCreateInfo.ResourceMemoryType = mMemoryType;
			bufferCreateInfo.Exclusive = true;

			return mBufferPool.Allocate(mRenderDevice, bufferCreateInfo, nullptr);
		});

		BufferPool* bufferPoolPtr = bufferPool.Get();
		mAcquiredBufferPools.Add(Move(bufferPool));

		return bufferPoolPtr;
	}
}
