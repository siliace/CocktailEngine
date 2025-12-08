#ifndef COCKTAIL_CORE_SYSTEM_SYSTEMMEMORY_HPP
#define COCKTAIL_CORE_SYSTEM_SYSTEMMEMORY_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief Provides low-level system memory management utilities
     *
     * This class exposes static functions to allocate and release memory directly
     * from the operating system, as well as to query system-specific memory
     * characteristics such as the memory page size.
     *
     * The interface is intentionally minimal and stateless, making it suitable
     * for use in low-level subsystems such as memory allocators, virtual memory
     * managers, or platform abstraction layers.
     *
     * All memory allocated through this class must be explicitly released by the
     * caller. The class does not perform any internal tracking or safety checks.
     */
    class SystemMemory
    {
    public:

        /**
         * \brief Allocates a contiguous block of system memory
         *
         * This function requests a memory region of the given size from the operating
         * system. The returned memory is readable and writable.
         *
         * \param size Size of the memory block to allocate, in bytes
         * \return Pointer to the allocated memory block, or nullptr on failure
         *
         * \note The allocated memory must be released using SystemMemory::Free() with the same size parameter
         */
        static void* Allocate(std::size_t size);

        /**
         * \brief Releases a previously allocated block of system memory
         *
         * This function frees a memory region that was previously allocated using
         * SystemMemory::Allocate().
         *
         * \param pointer Pointer to the memory block to release
         * \param size Size of the memory block to release, in bytes
         *
         * \warning The size parameter must exactly match the size used during allocation
         */
        static void Free(void* pointer, std::size_t size);

        /**
         * \brief Retrieves the system memory page size
         *
         * This function returns the size, in bytes, of a memory page as defined by
         * the underlying system. This value is typically used to align memory
         * allocations or manage low-level memory operations.
         *
         * \return The system memory page size, in bytes
         */
        static std::size_t GetPageSize();
    };
}

#endif // COCKTAIL_CORE_SYSTEM_SYSTEMMEMORY_HPP
