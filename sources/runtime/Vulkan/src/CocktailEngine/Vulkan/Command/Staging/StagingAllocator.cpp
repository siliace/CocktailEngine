#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Command/Staging/StagingAllocator.hpp>
#include <CocktailEngine/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	StagingAllocator::StagingAllocator(RenderDevice* renderDevice, Renderer::BufferUsageFlags bufferUsage, std::size_t bufferSize) :
		mRenderDevice(renderDevice),
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
		for (ObjectPoolUniquePtr<StagingBuffer>& buffer : mAcquiredBuffers)
		{
			std::size_t padding = buffer->ComputePadding(alignment);
			if (padding + allocationSize <= buffer->GetRemainingCapacity())
				return buffer.Get();
		}

		ObjectPoolUniquePtr<StagingBuffer> buffer = mAvailableBuffers.FindIndexIf([&](const ObjectPoolUniquePtr<StagingBuffer>& stagingBuffer) {
			return stagingBuffer->GetRemainingCapacity() + stagingBuffer->ComputePadding(alignment) >= allocationSize;
		}).Map([&](unsigned int index) {
			return mAvailableBuffers.RemoveAt(index);
		}).GetOrElse([&]() {
			return mStagingBufferPool.Allocate(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize));
		});

		StagingBuffer* bufferPtr = buffer.Get();
		mAcquiredBuffers.Add(Move(buffer));

		return bufferPtr;
	}

	void StagingAllocator::Reserve(std::size_t allocationSize)
	{
		for (const ObjectPoolUniquePtr<StagingBuffer>& acquiredBuffer : mAcquiredBuffers)
		{
			if (allocationSize < acquiredBuffer->GetRemainingCapacity())
				return;
		}

		for (const ObjectPoolUniquePtr<StagingBuffer>& availableBuffer : mAvailableBuffers)
		{
			if (allocationSize < availableBuffer->GetRemainingCapacity())
				return;
		}

		mAvailableBuffers.Add(
			mStagingBufferPool.Allocate(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize))
		);
	}

	void StagingAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBuffers.IsEmpty())
		{
			for (ObjectPoolUniquePtr<StagingBuffer>& stagingBuffer : mAcquiredBuffers)
			{
				stagingBuffer->Reset();
				mAvailableBuffers.Add(Move(stagingBuffer));
			}
		}
		else
		{
			mAvailableBuffers.Clear();
		}

		mAcquiredBuffers.Clear();

		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}
}
