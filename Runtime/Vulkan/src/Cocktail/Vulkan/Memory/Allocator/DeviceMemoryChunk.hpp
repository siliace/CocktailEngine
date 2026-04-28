#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP

#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
    /**
     * \brief Represents a Vulkan device memory allocation used for sub-allocation
     *
     * A DeviceMemoryChunk encapsulates a VkDeviceMemory allocation and manages
     * sub-allocations through DeviceMemoryBlock instances. It is responsible for
     * allocating, tracking, and compacting memory blocks within the chunk.
     *
     * Chunks can be either general-purpose (supporting multiple sub-allocations)
     * or dedicated to a single resource (buffer or texture).
     */
	class DeviceMemoryChunk
	{
	public:

	    /**
         * \brief Constructs a general-purpose memory chunk
         *
         * \param renderDevice The render device used for allocation
         * \param blockPool Pool used to allocate memory blocks
         * \param priority Memory priority hint
         * \param size Size of the chunk in bytes
         * \param memoryTypeIndex Vulkan memory type index
         */
		DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex);

	    /**
         * \brief Constructs a dedicated memory chunk for a texture
         *
         * \param renderDevice The render device used for allocation
         * \param blockPool Pool used to allocate memory blocks
         * \param texture The texture this memory is dedicated to
         * \param priority Memory priority hint
         * \param size Size of the allocation in bytes
         * \param memoryTypeIndex Vulkan memory type index
         */
		DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const AbstractTexture* texture, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex);

	    /**
         * \brief Constructs a dedicated memory chunk for a buffer
         *
         * \param renderDevice The render device used for allocation
         * \param blockPool Pool used to allocate memory blocks
         * \param buffer The buffer this memory is dedicated to
         * \param priority Memory priority hint
         * \param size Size of the allocation in bytes
         * \param memoryTypeIndex Vulkan memory type index
         */
		DeviceMemoryChunk(RenderDevice* renderDevice, ObjectPool<DeviceMemoryBlock>& blockPool, const Buffer* buffer, Renderer::MemoryPriority priority, std::size_t size, unsigned int memoryTypeIndex);

	    /**
         * \brief Allocates a memory block from this chunk
         *
         * Searches for a suitable free block that satisfies the requested size
         * and alignment, potentially splitting an existing block.
         *
         * \param alignment Required alignment in bytes
         * \param size Size of the allocation in bytes
         *
         * \return Pointer to the allocated block, or nullptr if allocation failed
         */
		DeviceMemoryBlock* AllocateBlock(std::size_t alignment, std::size_t size);

	    /**
         * \brief Compacts the chunk by merging adjacent free blocks
         *
         * Helps reduce fragmentation and improve future allocation success.
         */
		void Compact();

	    /**
         * \brief Indicates whether the chunk is entirely free
         *
         * \return True if all blocks are free, false otherwise
         */
		bool IsFree() const;

	    /**
         * \brief Retrieves the memory priority of this chunk
         *
         * \return Memory priority hint
         */
	    Renderer::MemoryPriority GetMemoryPriority() const;

	    /**
         * \brief Retrieves the total size of the chunk
         *
         * \return Size in bytes
         */
		std::size_t GetSize() const;

	    /**
         * \brief Retrieves the Vulkan memory type index
         *
         * \return Memory type index used for allocation
         */
		unsigned int GetMemoryTypeIndex() const;

	    /**
         * \brief Indicates whether this chunk is dedicated to a single resource
         *
         * Dedicated allocations are typically used for large resources or when
         * required by Vulkan memory constraints.
         *
         * \return True if this chunk is dedicated, false otherwise
         */
		bool IsDedicated() const;

	    /**
         * \brief Retrieves the underlying Vulkan device memory
         *
         * \return Shared pointer to the DeviceMemory wrapper
         */
		std::shared_ptr<DeviceMemory> GetDeviceMemory() const;

	private:
		
	    /**
         * \brief Allocates the underlying Vulkan device memory
         *
         * If a texture or buffer is provided, the allocation may use
         * dedicated allocation flags depending on Vulkan requirements.
         *
         * \param texture Optional texture for dedicated allocation
         * \param buffer Optional buffer for dedicated allocation
         */
		void AllocateDeviceMemory(const AbstractTexture* texture, const Buffer* buffer);

		RenderDevice* mRenderDevice; /*!< Owning render device */
		ObjectPool<DeviceMemoryBlock>& mBlockPool; /*!< Pool used to allocate DeviceMemoryBlock instances */
	    Renderer::MemoryPriority mMemoryPriority; /*!< Memory priority hint */
		std::size_t mSize; /*!< Total size of the chunk in bytes */
		unsigned int mMemoryTypeIndex; /*!< Heap memory type index */
		bool mDedicated; /*!< Indicates whether this is a dedicated allocation */
		void* mPtr; /*!< Pointer to mapped memory (if host-visible) */
		std::shared_ptr<DeviceMemory> mDeviceMemory; /*!< Underlying Vulkan device memory wrapper */
		Array<ObjectPool<DeviceMemoryBlock>::Unique> mBlocks; /*!< List of sub-allocated memory blocks */
	};
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYCHUNK_HPP
