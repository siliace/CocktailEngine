#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_MEMORYALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_MEMORYALLOCATOR_HPP

#include <Cocktail/Core/Memory/Allocator/UseSystemAllocator.hpp>

namespace Ck
{
    /**
     * \class MemoryAllocator
     *
     * \brief Base interface for classes implementing memory allocation
     *
     * This interface defines the fundamental operations required for custom
     * memory allocators: allocating, reallocating, and freeing memory blocks.
     */
    class MemoryAllocator : public UseSystemAllocator
    {
    public:

        /**
         * \brief Default virtual destructor
         *
         * Ensures proper cleanup of derived allocator implementations.
         */
        virtual ~MemoryAllocator() = default;

        /**
         * \brief Allocates a block of memory
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
        virtual void* Allocate(std::size_t size, std::size_t alignment) = 0;

        /**
         * \brief Resizes an existing memory block
         *
         * The contents of the memory block should be preserved up to the smaller
         * of the old and new sizes.
         *
         * \param pointer Pointer to the previously allocated block
         * \param size New size of the memory block, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the reallocated memory block, or nullptr on failure
         */
        virtual void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) = 0;

        /**
         * \brief Frees a previously allocated memory block
         *
         * \param pointer Pointer to the memory block to free
         */
        virtual void Free(void* pointer) = 0;
    };
}

#endif //COCKTAIL_CORE_MEMORY_ALLOCATOR_MEMORYALLOCATOR_HPP