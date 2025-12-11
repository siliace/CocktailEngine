#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
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
        void Free(void* original);
    };

    /**
     * \class MallocAllocationHandler
     *
     * \brief Facade for the MallocAllocator service
     *
     * This class provides static helper methods that forward memory operations
     * to the active MallocAllocator instance through the ServiceFacade system.
     */
    class COCKTAIL_CORE_API MallocAllocationHandler : public ServiceFacade<MallocAllocator>
    {
    public:

        /**
         * \brief Allocates memory using the MallocAllocator service
         *
         * \param size Size of the memory block to allocate, in bytes
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
        static void* Allocate(std::size_t size);

        /**
         * \brief Reallocates memory using the MallocAllocator service
         *
         * \param pointer Pointer to the current memory block
         * \param size New size of the memory block, in bytes
         *
         * \return Pointer to the reallocated memory block, or nullptr on failure
         */
        static void* Reallocate(void* pointer, std::size_t size);

        /**
         * \brief Frees memory using the MallocAllocator service
         *
         * \param pointer Pointer to the memory block to free
         */
        static void Free(void* pointer);
    };
}

#endif //COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATOR_HPP
