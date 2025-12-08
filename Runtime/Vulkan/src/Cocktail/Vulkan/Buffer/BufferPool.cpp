#include <cstring>

#include <Cocktail/Core/Memory/Memory.hpp>

#include <Cocktail/Vulkan/Buffer/BufferPool.hpp>

namespace Ck::Vulkan
{
	BufferPool::BufferPool(RenderDevice* renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Buffer(std::move(renderDevice), createInfo, allocationCallbacks),
		mRemainingCapacity(createInfo.Size)
	{
		/// Nothing
	}

	std::size_t BufferPool::PushData(std::size_t alignment, std::size_t length, const void* data)
	{
		std::size_t padding = ComputePadding(alignment);
		std::size_t offset = GetSize() - mRemainingCapacity;

		assert(alignment == 0 || (offset + padding) % alignment == 0);

		offset += padding;

		void* destination = Map(offset, length);
		Memory::Copy(destination, data, length);
		Unmap();

		mRemainingCapacity -= padding + length;

		return offset;
	}

	std::size_t BufferPool::ComputePadding(std::size_t alignment) const
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

	void BufferPool::Reset()
	{
		mRemainingCapacity = GetSize();
	}

	std::size_t BufferPool::GetRemainingCapacity() const
	{
		return mRemainingCapacity;
	}
}
