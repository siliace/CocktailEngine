#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/Staging/StagingAllocator.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	StagingAllocator::StagingAllocator(const Ref<RenderDevice>& renderDevice, std::size_t bufferSize) :
		mRenderDevice(renderDevice),
		mBufferSize(bufferSize)
	{
		/// Nothing
	}

	Ref<StagingBuffer> StagingAllocator::AcquireStagingBuffer(std::size_t alignment, std::size_t size)
	{
		for (Ref<StagingBuffer> buffer : mAcquiredBuffers)
		{
			std::size_t padding = buffer->ComputePadding(alignment);
			if (padding + size <= buffer->GetRemainingCapacity())
				return buffer;
		}

		auto itBuffer = std::find_if(mAvailableBuffers.begin(), mAvailableBuffers.end(), [&](const Ref<StagingBuffer>& stagingBuffer) {
			return stagingBuffer->GetRemainingCapacity() + stagingBuffer->ComputePadding(alignment) > size;
		});

		Ref<StagingBuffer> buffer;
		if (itBuffer != mAvailableBuffers.end())
		{
			buffer = std::move(*itBuffer);
			mAvailableBuffers.erase(itBuffer);
		}
		else
		{
			Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Size = std::max(mBufferSize, size);
			bufferCreateInfo.MemoryType = Renderer::MemoryType::Dynamic;

			buffer = mBufferPool.Allocate(mRenderDevice, bufferCreateInfo, nullptr);
		}

		mAcquiredBuffers.push_back(buffer);

		return buffer;
	}

	void StagingAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBuffers.empty())
		{
			for (Ref<StagingBuffer> stagingBuffer : mAcquiredBuffers)
			{
				stagingBuffer->Reset();
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

	std::size_t StagingAllocator::GetBufferSize() const
	{
		return mBufferSize;
	}
}
