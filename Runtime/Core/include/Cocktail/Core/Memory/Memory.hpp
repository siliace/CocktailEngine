#ifndef COCKTAIL_CORE_MEMORY_MEMORY_HPP
#define COCKTAIL_CORE_MEMORY_MEMORY_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    /**
     * \class Memory
     *
     * \brief Provides low-level memory manipulation utilities.
     *
     * This class offers static helper functions for zeroing, setting, copying,
     * and moving blocks of memory. It also provides templated overloads that
     * automatically deduce the size of the target types.
     */
    class COCKTAIL_CORE_API Memory
    {
    public:

        static constexpr std::size_t DefaultAlignment = 16;

        /**
         * \brief Fills a block of memory with zeros
         *
         * \param destination Pointer to the memory block to clear
         * \param size Number of bytes to set to zero
         */
        static void Zero(void* destination, std::size_t size);

        /**
         * \brief Templated helper that zeros an object of type T
         *
         * \tparam T Type of the object to zero
         *
         * \param destination Reference to the object to clear
         */
        template <typename T>
        static void Zero(T& destination)
        {
            Zero(static_cast<void*>(&destination), sizeof(T));
        }

        /**
         * \brief Fills a block of memory with a specific byte value
         *
         * \param destination Pointer to the memory block to fill
         * \param value Byte value to write
         * \param size Number of bytes to set
         */
        static void Set(void* destination, Byte value, std::size_t size);

        /**
         * \brief Copies a block of memory from one location to another
         *
         * \param destination Pointer to the destination buffer
         * \param source Pointer to the source buffer
         * \param size Number of bytes to copy
         */
        static void Copy(void* destination, const void* source, std::size_t size);

        /**
         * \brief Moves a block of memory from one location to another
         *
         * This function handles overlapping memory regions safely
         *
         * \param destination Pointer to the destination buffer
         * \param source Pointer to the source buffer
         * \param size Number of bytes to move
         */
        static void Move(void* destination, const void* source, std::size_t size);

        /**
         * \brief Allocates a block of memory
         *
         * This function will resolve the default implementation of MemoryAllocator
         * to perform memory allocation.
         *
         * \param size Size of the memory block to allocate, in bytes
         * \param alignment Alignment constraint of the memory to allocate
         *
         * \return Pointer to the allocated memory block, or nullptr on failure
         */
        static void* Allocate(std::size_t size, std::size_t alignment = DefaultAlignment);

        /**
         * \brief Resizes an existing memory block
         *
         * This function will resolve the default implementation of MemoryAllocator
         * to perform memory allocation.
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
        static void* Reallocate(void* pointer, std::size_t size, std::size_t alignment = DefaultAlignment);

        /**
         * \brief Frees a previously allocated memory block
         *
         * This function will resolve the default implementation of MemoryAllocator
         * to perform memory deallocation.
         *
         * \param pointer Pointer to the memory block to free
         */
        static void Free(void* pointer);

    private:

        static void CreateGlobalAllocator();
    };
}

#endif // COCKTAIL_CORE_MEMORY_MEMORY_HPP