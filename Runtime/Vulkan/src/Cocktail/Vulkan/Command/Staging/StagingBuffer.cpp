#include <cstring>

#include <Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp>

namespace Ck::Vulkan
{
	StagingBuffer::StagingBuffer(Ref<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Super(std::move(renderDevice), createInfo, allocationCallbacks),
		mRemainingCapacity(createInfo.Size)
	{
		/// Nothing
	}

	std::size_t StagingBuffer::PushData(std::size_t alignment, std::size_t length, const void* data)
	{
		std::size_t padding = ComputePadding(alignment);
		std::size_t offset = GetSize() - mRemainingCapacity;

		assert(alignment == 0 || (offset + padding) % alignment == 0);

		offset += padding;

		void* destination = Map(offset, length);
		std::memcpy(destination, data, length);
		Unmap();

		mRemainingCapacity -= padding + length;

		return offset;
	}

	std::size_t StagingBuffer::ComputePadding(std::size_t alignment) const
	{
		if (alignment)
		{
			std::size_t currentOffset = GetSize() - mRemainingCapacity;
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
		mRemainingCapacity = GetSize();
	}

	std::size_t StagingBuffer::GetRemainingCapacity() const
	{
		return mRemainingCapacity;
	}
}
