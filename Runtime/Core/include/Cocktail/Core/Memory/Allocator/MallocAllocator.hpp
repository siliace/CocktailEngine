#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP

#include <Cocktail/Core/Memory/Allocator/MemoryAllocator.hpp>

namespace Ck
{
    /**
     * \class MallocAllocator
     *
     * \brief Memory allocator based on the standard C malloc/realloc/free functions
     *
     * This allocator provides a thin abstraction layer over the standard C
     * memory management functions, offering allocation, reallocation, and
     * deallocation using malloc(), realloc(), and free().
     */
    class COCKTAIL_CORE_API MallocAllocator : public MemoryAllocator
    {
    public:

        /**
         * \brief Allocates a block of memory using malloc()
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
        void* Allocate(std::size_t size, std::size_t alignment) override;

        /**
         * \brief Resizes a memory block using realloc()
         *
         * The contents of the block are preserved up to the minimum of the
         * old and new sizes. If reallocation fails, the original pointer
         * remains valid.
         *
         * \param pointer Pointer to the previously allocated memory block
         * \param size New size of the block, in bytes
         * \param alignment
         *
         * \return Pointer to the reallocated block, or nullptr on failure
         */
        void* Reallocate(void* pointer, std::size_t size, std::size_t alignment) override;

        /**
         * \brief Frees a memory block using free()
         *
         * \param original Pointer to the memory block to free
         */
        void Free(void* original) override;

        /**
         * \brief Tell whether the allocator needs to be externalized synchronized
         *
         * \return True if the allocators needs external synchronization, false otherwise
         */
        bool IsThreadSafe() const override;

        /**
         * \brief Tell whether the allocator should be instantiated per thread
         *
         * \return True if the allocators is per thread, false otherwise
         */
        bool IsThreadLocal() const override;
    };
}

#endif //COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP
