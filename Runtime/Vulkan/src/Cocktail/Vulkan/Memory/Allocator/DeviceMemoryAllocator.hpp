#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP

#include <algorithm>

#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

    /**
     * \brief High-level Vulkan device memory allocator
     *
     * This class manages Vulkan memory allocations by organizing them into
     * DeviceMemoryChunk objects and sub-allocating DeviceMemoryBlock instances.
     *
     * It handles resource allocation for buffers and textures, selecting appropriate
     * memory types, managing dedicated allocations when required, and performing
     * garbage collection to reduce fragmentation.
     */
	class DeviceMemoryAllocator
	{
	public:

	    /**
         * \brief Constructs a memory allocator
         *
         * \param renderDevice The render device used for Vulkan memory operations
         * \param defaultChunkSize Default size (in bytes) used when creating new memory chunks
         */
		DeviceMemoryAllocator(RenderDevice* renderDevice, std::size_t defaultChunkSize);

	    /**
         * \brief Destructor
         *
         * Releases all allocated memory chunks and associated resources.
         */
		~DeviceMemoryAllocator();

	    /**
         * \brief Allocates memory for a texture
         *
         * Determines memory requirements, selects a suitable memory type,
         * and allocates a memory block. May use a dedicated allocation if required.
         *
         * \param texture The texture to allocate memory for
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
		DeviceMemoryBlock* Allocate(const AbstractTexture& texture);

	    /**
         * \brief Allocates memory for a buffer
         *
         * Determines memory requirements, selects a suitable memory type,
         * and allocates a memory block. May use a dedicated allocation if required.
         *
         * \param buffer The buffer to allocate memory for
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
		DeviceMemoryBlock* Allocate(const Buffer& buffer);

	    /**
         * \brief Performs garbage collection on memory chunks
         *
         * Frees unused chunks and optionally compacts remaining ones to reduce fragmentation.
         *
         * \param compact If true, attempts to compact memory by merging free blocks
         */
		void GarbageCollect(bool compact);

	private:

	    /**
         * \brief Retrieves memory requirements for a buffer
         *
         * \param buffer The buffer to query
         * \param requirements Output Vulkan memory requirements
         * \param dedicated Output flag indicating whether a dedicated allocation is required
         */
		void GetBufferMemoryRequirements(const Buffer& buffer, VkMemoryRequirements& requirements, bool& dedicated) const;

	    /**
         * \brief Retrieves memory requirements for a texture
         *
         * \param texture The texture to query
         * \param requirements Output Vulkan memory requirements
         * \param dedicated Output flag indicating whether a dedicated allocation is required
         */
		void GetTextureMemoryRequirements(const AbstractTexture& texture, VkMemoryRequirements& requirements, bool& dedicated) const;

	    /**
         * \brief Finds a compatible Vulkan memory type index
         *
         * \param typeIndexMask Bitmask of supported memory types from Vulkan
         * \param memoryType Desired memory usage type
         *
         * \return Index of a suitable memory type
         */
		unsigned int FindMemoryTypeIndex(unsigned int typeIndexMask, Renderer::MemoryType memoryType) const;

	    /**
         * \brief Allocates a dedicated memory block for a resource
         *
         * Creates a new chunk dedicated to a single resource and allocates a block
         * matching its exact memory requirements.
         *
         * \tparam T Resource type (buffer or texture)
         *
         * \param resource The resource to allocate memory for
         * \param priority Memory priority hint
         * \param memoryRequirements Vulkan memory requirements
         * \param memoryTypeIndex Selected memory type index
         *
         * \return Pointer to the allocated memory block
         */
		template <typename T>
		DeviceMemoryBlock* AllocateDedicatedBlock(const T* resource, Renderer::MemoryPriority priority, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex)
		{
		    auto chunk = mChunkPool.AllocateUnique(mRenderDevice, mBlockPool, resource, priority, memoryRequirements.size, memoryTypeIndex);
			auto block = chunk->AllocateBlock(memoryRequirements.alignment, memoryRequirements.size);

		    mChunks.Add(std::move(chunk));

		    return block;
		}

	    /**
         * \brief Allocates a block from existing or new chunks
         *
         * Searches for a suitable chunk matching the requested memory type and priority,
         * or creates a new one if necessary.
         *
         * \param priority Memory priority hint
         * \param memoryRequirements Vulkan memory requirements
         * \param memoryTypeIndex Selected memory type index
         *
         * \return Pointer to the allocated block, or nullptr if allocation failed
         */
	    DeviceMemoryBlock* AllocateBlock(Renderer::MemoryPriority priority, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex);

		RenderDevice* mRenderDevice; /*!< Owning render device */
		std::size_t mDefaultChunkSize; /*!< Default size used when allocating new chunks */
		Array<ObjectPool<DeviceMemoryChunk>::Unique> mChunks; /*!< List of active memory chunks */
		ObjectPool<DeviceMemoryChunk> mChunkPool; /*!< Pool for allocating memory chunks */
		ObjectPool<DeviceMemoryBlock> mBlockPool; /*!< Pool for allocating memory blocks */
	}; 
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
