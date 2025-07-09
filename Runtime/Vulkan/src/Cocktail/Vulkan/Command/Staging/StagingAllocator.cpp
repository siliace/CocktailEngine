#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/Staging/StagingAllocator.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	StagingAllocator::StagingAllocator(std::shared_ptr<RenderDevice> renderDevice, Renderer::BufferUsageFlags bufferUsage, std::size_t bufferSize) :
		mRenderDevice(std::move(renderDevice)),
		mBufferUsage(bufferUsage),
		mBufferSize(bufferSize)
	{
		/// Nothing
	}

	StagingAllocator::~StagingAllocator()
	{
		Reset(true);
	}

	StagingBuffer* StagingAllocator::AcquireStagingBuffer(std::size_t alignment, std::size_t allocationSize)
	{
		for (StagingBuffer* buffer : mAcquiredBuffers)
		{
			std::size_t padding = buffer->ComputePadding(alignment);
			if (padding + allocationSize <= buffer->GetRemainingCapacity())
				return buffer;
		}

		StagingBuffer* buffer = mAvailableBuffers.FindIndexIf([&](StagingBuffer* stagingBuffer) {
			return stagingBuffer->GetRemainingCapacity() + stagingBuffer->ComputePadding(alignment) >= allocationSize;
		}).Map([&](unsigned int index) {
			return mAvailableBuffers.RemoveAt(index);
		}).GetOrElse([&]() {
			return mStagingBufferPool.AllocateUnsafe(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize));
		});

		mAcquiredBuffers.Add(buffer);

		return buffer;
	}

	void StagingAllocator::Reserve(std::size_t allocationSize)
	{
		for (const StagingBuffer* acquiredBuffer : mAcquiredBuffers)
		{
			if (allocationSize < acquiredBuffer->GetRemainingCapacity())
				return;
		}

		for (const StagingBuffer* availableBuffer : mAvailableBuffers)
		{
			if (allocationSize < availableBuffer->GetRemainingCapacity())
				return;
		}

		mAvailableBuffers.Add(
			mStagingBufferPool.AllocateUnsafe(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize))
		);
	}

	void StagingAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBuffers.IsEmpty())
		{
			for (StagingBuffer* stagingBuffer : mAcquiredBuffers)
			{
				stagingBuffer->Reset();
				mAvailableBuffers.Add(stagingBuffer);
			}
		}
		else
		{
			for (StagingBuffer* stagingBuffer : mAcquiredBuffers)
				mStagingBufferPool.Recycle(stagingBuffer);

			for (StagingBuffer* stagingBuffer : mAcquiredBuffers)
				mStagingBufferPool.Recycle(stagingBuffer);

			mAvailableBuffers.Clear();
		}

		mAcquiredBuffers.Clear();

		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}
}
