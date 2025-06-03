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

	StagingBuffer& StagingAllocator::AcquireStagingBuffer(std::size_t alignment, std::size_t allocationSize)
	{
		for (StagingBuffer& buffer : mAcquiredBuffers)
		{
			std::size_t padding = buffer.ComputePadding(alignment);
			if (padding + allocationSize <= buffer.GetRemainingCapacity())
				return buffer;
		}

		auto itBuffer = std::find_if(mAvailableBuffers.begin(), mAvailableBuffers.end(), [&](const StagingBuffer& stagingBuffer) {
			return stagingBuffer.GetRemainingCapacity() + stagingBuffer.ComputePadding(alignment) >= allocationSize;
		});

		if (itBuffer != mAvailableBuffers.end())
		{
			StagingBuffer buffer = *itBuffer;

			mAvailableBuffers.erase(itBuffer);
			return mAcquiredBuffers.emplace_back(std::move(buffer));
		}

		return mAcquiredBuffers.emplace_back(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize));
	}

	void StagingAllocator::Reserve(std::size_t allocationSize)
	{
		for (const StagingBuffer& acquiredBuffer : mAcquiredBuffers)
		{
			if (allocationSize < acquiredBuffer.GetRemainingCapacity())
				return;
		}

		for (const StagingBuffer& availableBuffer : mAvailableBuffers)
		{
			if (allocationSize < availableBuffer.GetRemainingCapacity())
				return;
		}

		mAvailableBuffers.emplace_back(mRenderDevice, mBufferUsage, std::max(mBufferSize, allocationSize));
	}

	void StagingAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBuffers.empty())
		{
			for (StagingBuffer& stagingBuffer : mAcquiredBuffers)
			{
				stagingBuffer.Reset();
				mAvailableBuffers.push_back(stagingBuffer);
			}
		}
		else
		{
			mAvailableBuffers.clear();
		}

		mAcquiredBuffers.clear();
		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}
}
