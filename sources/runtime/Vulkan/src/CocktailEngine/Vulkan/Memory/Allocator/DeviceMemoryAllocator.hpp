#ifndef COCKTAILENGINE_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
#define COCKTAILENGINE_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP

#include <CocktailEngine/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp>

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
         * Compaction is also performed when \p compact is false but a heap is over its budget:
         * the alternative is letting the driver move the excess to host memory, which is far more
         * expensive than compacting and is never reported.
         *
         * \param compact If true, attempts to compact memory by merging free blocks
         */
		void GarbageCollect(bool compact);

	private:

	    /**
         * \brief How much of a memory heap this process may use, and how much it already uses
         *
         * Reported by VK_EXT_memory_budget. Unlike VkMemoryHeap::size, which is the physical size
         * of the heap, a budget accounts for what the rest of the system is doing: another
         * process, the compositor, the desktop. It is therefore only valid at the instant it was
         * queried and is never cached.
         *
         * Both values are a driver estimate rather than a contract: an allocation that fits in
         * \p Budget can still fail, and one that does not fit can still succeed. They drive the
         * allocator's policy, never its correctness.
         *
         * \see RenderDeviceFeature::MemoryBudget
         * \see QueryHeapBudget
         */
	    struct HeapBudget
	    {
	        /**
             * \brief Whether the driver actually reported a budget
             *
             * False when the RenderDeviceFeature::MemoryBudget feature is unsupported, in which
             * case every other member is meaningless and the allocator behaves as unconstrained.
             */
	        bool Known = false;

	        /**
             * \brief Number of bytes of the heap this process may use
             *
             * Only meaningful when \p Known is true.
             */
	        VkDeviceSize Budget = 0;

	        /**
             * \brief Number of bytes of the heap this process currently uses
             *
             * Counts only this process' allocations, and counts them per heap rather than per
             * memory type, so usage cannot be attributed to one memory type when several share a
             * heap. Only meaningful when \p Known is true.
             */
	        VkDeviceSize Usage = 0;

	        /**
             * \brief Retrieves how many bytes are left before the budget is reached
             *
             * \return The remaining bytes, zero when already over budget or when unknown
             */
	        VkDeviceSize GetAvailable() const
	        {
	            if (!Known || Usage >= Budget)
	                return 0;

	            return Budget - Usage;
	        }
	    };

	    /**
         * \brief Queries the budget of the heap backing a memory type
         *
         * \param memoryTypeIndex Index of the memory type whose heap is queried
         *
         * \return The budget of the heap, unknown when the feature is unsupported
         */
	    HeapBudget QueryHeapBudget(unsigned int memoryTypeIndex) const;

	    /**
         * \brief Tells whether any heap of the physical device is over its budget
         *
         * Being over budget is not an error and nothing else will report it: drivers commonly back
         * the excess with host memory reached over PCIe, which keeps everything working while
         * costing an order of magnitude in bandwidth on the resources concerned.
         *
         * \return True when at least one heap is over budget, false otherwise or when unsupported
         */
	    bool IsAnyHeapOverBudget() const;

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

		    mChunks.Add(Move(chunk));

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

	    /**
         * \brief Searches the chunks already held for one able to satisfy an allocation
         *
         * \param priority Memory priority hint
         * \param memoryRequirements Vulkan memory requirements
         * \param memoryTypeIndex Selected memory type index
         *
         * \return Pointer to the allocated block, or nullptr when no chunk could satisfy it
         */
	    DeviceMemoryBlock* FindBlock(Renderer::MemoryPriority priority, const VkMemoryRequirements& memoryRequirements, unsigned int memoryTypeIndex);

	    /**
         * \brief Chooses the size of a chunk about to be created
         *
         * Defaults to the configured chunk size, shrinks it when the heap has little budget left,
         * and never returns less than what the allocation itself requires.
         *
         * \param requiredSize Size the allocation the chunk is created for needs
         * \param budget Budget of the heap backing the chunk, possibly unknown
         *
         * \return The size of the chunk to create
         */
	    std::size_t ChooseChunkSize(VkDeviceSize requiredSize, const HeapBudget& budget) const;

	    /**
         * \brief Reports, once per episode, that a heap no longer has room for what is allocated
         *
         * \param memoryTypeIndex Memory type whose heap is concerned
         * \param budget Budget of the heap backing the memory type
         * \param chunkSize Size of the chunk about to be created
         */
	    void ReportBudgetPressure(unsigned int memoryTypeIndex, const HeapBudget& budget, std::size_t chunkSize);

		RenderDevice* mRenderDevice; /*!< Owning render device */
		std::size_t mDefaultChunkSize; /*!< Default size used when allocating new chunks */
		Array<ObjectPool<DeviceMemoryChunk>::Unique> mChunks; /*!< List of active memory chunks */
		ObjectPool<DeviceMemoryChunk> mChunkPool; /*!< Pool for allocating memory chunks */
		ObjectPool<DeviceMemoryBlock> mBlockPool; /*!< Pool for allocating memory blocks */
		bool mBudgetPressureReported = false; /*!< Whether the current budget pressure episode was already reported */
	}; 
}

#endif // COCKTAILENGINE_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYALLOCATOR_HPP
