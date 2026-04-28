#ifndef COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP
#define COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP

#include <Cocktail/Core/Utility/ObjectPool.hpp>

namespace Ck::Vulkan
{
    class AbstractTexture;
    class Buffer;
    class DeviceMemoryChunk;
    class RenderDevice;

    /**
     * \brief Represents a sub-allocation within a Vulkan device memory chunk
     *
     * A DeviceMemoryBlock describes a contiguous region of memory allocated from a
     * larger VkDeviceMemory object (DeviceMemoryChunk). It is used to bind buffers
     * and images while enabling efficient memory sub-allocation.
     *
     * Blocks can be split and merged to manage fragmentation and reuse memory.
     */
    class DeviceMemoryBlock
    {
    public:

        /**
         * \brief Constructs a memory block from a parent chunk
         *
         * \param chunk The parent memory chunk owning the VkDeviceMemory
         * \param offset Offset in bytes from the beginning of the chunk
         * \param size Size of the block in bytes
         * \param ptr Optional mapped pointer to the beginning of the chunk (nullptr if not mapped)
         */
        DeviceMemoryBlock(DeviceMemoryChunk* chunk, VkDeviceSize offset, VkDeviceSize size, void* ptr);

        /**
         * \brief Computes the padding required to satisfy an alignment constraint
         *
         * \param alignment Required alignment in bytes
         *
         * \return The number of padding bytes needed before this block can be used
         */
        VkDeviceSize ComputeAllocationPadding(VkDeviceSize alignment) const;

        /**
         * \brief Binds this memory block to a buffer
         *
         * The block must satisfy the buffer memory requirements (alignment, size, memory type).
         *
         * \param renderDevice The render device used to perform the binding
         * \param buffer The buffer to bind to this memory block
         */
        void BindToBuffer(const RenderDevice& renderDevice, const Buffer& buffer) const;

        /**
         * \brief Binds this memory block to an image
         *
         * The block must satisfy the image memory requirements (alignment, size, memory type).
         *
         * \param renderDevice The render device used to perform the binding
         * \param texture The texture to bind to this memory block
         */
        void BindToTexture(const RenderDevice& renderDevice, const AbstractTexture& texture) const;

        /**
         * \brief Splits this block into two blocks
         *
         * The current block is reduced to fit the requested allocation, and a new
         * block is created from the remaining memory.
         *
         * \param blockPool Pool used to allocate the new block
         * \param alignment Required alignment for the new allocation
         * \param size Size of the requested allocation
         *
         * \return A unique pointer to the remaining block after the split
         */
        ObjectPool<DeviceMemoryBlock>::Unique Split(ObjectPool<DeviceMemoryBlock>& blockPool, VkDeviceSize alignment, VkDeviceSize size);

        /**
         * \brief Merges this block with an adjacent free block
         *
         * The provided block must be contiguous in memory and free.
         *
         * \param block The block to merge with
         */
        void Merge(const DeviceMemoryBlock& block);

        /**
         * \brief Returns a CPU pointer to the mapped memory at a given offset
         *
         * \param offset Offset in bytes from the beginning of this block
         *
         * \return Pointer to the mapped memory, or nullptr if memory is not mapped
         */
        void* Map(VkDeviceSize offset) const;

        /**
         * \brief Marks the block as in use
         *
         * Typically called when the block is allocated to a resource.
         */
        void Acquire();

        /**
         * \brief Marks the block as free
         *
         * Typically called when the associated resource is destroyed.
         */
        void Release();

        /**
         * \brief Retrieves the parent memory chunk
         *
         * \return Pointer to the owning DeviceMemoryChunk
         */
        DeviceMemoryChunk* GetChunk() const;

        /**
         * \brief Retrieves the size of the block
         *
         * \return Size in bytes
         */
        VkDeviceSize GetSize() const;

        /**
         * \brief Indicates whether the block is free
         *
         * \return True if the block is not currently in use
         */
        bool IsFree() const;

    private:

        DeviceMemoryChunk* mChunk; /*!< Parent memory chunk owning the VkDeviceMemory */
        VkDeviceSize mOffset; /*!< Offset of the block within the chunk */
        VkDeviceSize mPadding; /*!< Padding applied to satisfy alignment constraints */
        VkDeviceSize mSize; /*!< Size of the block (excluding padding)*/
        void* mPtr; /*!< Pointer to mapped memory (if available) */
        bool mFree; /*!< Indicates whether the block is free or in use */
    };
}

#endif // COCKTAIL_VULKAN_MEMORY_ALLOCATOR_DEVICEMEMORYBLOCK_HPP
