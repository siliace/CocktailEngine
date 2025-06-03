#include <cstring>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp>

namespace Ck::Vulkan
{
	StagingBuffer::StagingBuffer(const std::shared_ptr<RenderDevice>& renderDevice, Renderer::BufferUsageFlags bufferUsage, std::size_t bufferSize) :
		mRemainingCapacity(bufferSize)
	{
		Renderer::BufferCreateInfo createInfo;
		createInfo.Usage = bufferUsage;
		createInfo.Size = bufferSize;
		createInfo.MemoryType = Renderer::MemoryType::Dynamic;
		createInfo.Exclusive = true;

		mBuffer = std::static_pointer_cast<Buffer>(renderDevice->CreateBuffer(createInfo));
	}

	std::size_t StagingBuffer::PushData(std::size_t alignment, std::size_t length, const void* data)
	{
		std::size_t padding = ComputePadding(alignment);
		std::size_t offset = mBuffer->GetSize() - mRemainingCapacity;

		assert(alignment == 0 || (offset + padding) % alignment == 0);

		offset += padding;

		void* destination = mBuffer->Map(offset, length);
		std::memcpy(destination, data, length);
		mBuffer->Unmap();

		mRemainingCapacity -= padding + length;

		return offset;
	}

	std::size_t StagingBuffer::ComputePadding(std::size_t alignment) const
	{
		if (alignment)
		{
			std::size_t currentOffset = mBuffer->GetSize() - mRemainingCapacity;
			if (currentOffset)
			{
				std::size_t currentAlignment = currentOffset % alignment;
				if (currentAlignment)
					return alignment - currentAlignment;
			}
		}

		return 0;
	}
		
	void StagingBuffer::Reset()
	{
		mRemainingCapacity = mBuffer->GetSize();
	}

	Buffer* StagingBuffer::GetBuffer() const
	{
		return mBuffer.get();
	}

	std::size_t StagingBuffer::GetRemainingCapacity() const
	{
		return mRemainingCapacity;
	}
}
