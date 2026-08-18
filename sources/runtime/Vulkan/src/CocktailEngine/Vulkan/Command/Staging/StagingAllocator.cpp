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
		for (StagingBufferPtr& buffer : mAcquiredBuffers)
		{
			std::size_t padding = buffer->ComputePadding(alignment);
			if (padding + allocationSize <= buffer->GetRemainingCapacity())
				return buffer.Get();
		}

		StagingBufferPtr buffer = mAvailableBuffers.FindIndexIf([&](const StagingBufferPtr& stagingBuffer) {
			return stagingBuffer->GetRemainingCapacity() + stagingBuffer->ComputePadding(alignment) >= allocationSize;
		}).Map([&](unsigned int index) {
			return mAvailableBuffers.RemoveAt(index);
		}).GetOrElse([&]() {
			return mStagingBufferPool.AllocateUnique(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize));
		});

		StagingBuffer* bufferPtr = buffer.Get();
		mAcquiredBuffers.Add(Move(buffer));

		return bufferPtr;
	}

	void StagingAllocator::Reserve(std::size_t allocationSize)
	{
		for (const StagingBufferPtr& acquiredBuffer : mAcquiredBuffers)
		{
			if (allocationSize < acquiredBuffer->GetRemainingCapacity())
				return;
		}

		for (const StagingBufferPtr& availableBuffer : mAvailableBuffers)
		{
			if (allocationSize < availableBuffer->GetRemainingCapacity())
				return;
		}

		mAvailableBuffers.Add(
			mStagingBufferPool.AllocateUnique(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize))
		);
	}

	void StagingAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBuffers.IsEmpty())
		{
			for (StagingBufferPtr& stagingBuffer : mAcquiredBuffers)
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
