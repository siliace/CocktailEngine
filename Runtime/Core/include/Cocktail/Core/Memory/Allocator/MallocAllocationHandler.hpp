#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATIONHANDLER_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATIONHANDLER_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/Memory/Allocator/MallocAllocator.hpp>

namespace Ck
{
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

        static constexpr std::size_t DefaultAlignment = 16;

        /**
         * \brief Allocates memory using the MallocAllocator service
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
        static void* Allocate(std::size_t size, std::size_t alignment = DefaultAlignment);

        /**
         * \brief Reallocates memory using the MallocAllocator service
         *
         * \param pointer Pointer to the current memory block
         * \param size New size of the memory block, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the reallocated memory block, or nullptr on failure
         */
        static void* Reallocate(void* pointer, std::size_t size, std::size_t alignment = DefaultAlignment);

        /**
         * \brief Frees memory using the MallocAllocator service
         *
         * \param pointer Pointer to the memory block to free
         */
        static void Free(void* pointer);
    };
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_MALLOCALLOCATIONHANDLER_HPP
