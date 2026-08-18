#ifndef COCKTAILENGINE_CORE_MEMORY_ASAN_HPP
#define COCKTAILENGINE_CORE_MEMORY_ASAN_HPP

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
    /**
     * \brief Provides utilities for interacting with AddressSanitizer
     */
    class COCKTAILENGINE_CORE_API Asan
    {
    public:
        /**
         * \brief Poisons a memory region
         *
         * Accessing the poisoned memory region through an instrumented access
         * triggers an AddressSanitizer error.
         *
         * \param pointer Start address of the memory region
         * \param size Size of the memory region in bytes
         */
        static void Poison(void* pointer, Uint64 size);

        /**
         * \brief Checks whether a memory address is poisoned
         *
         * \param pointer Address to check
         * \return true if the address is poisoned, false otherwise
         */
        static bool IsPoisoned(void* pointer);

        /**
         * \brief Checks whether any byte in a memory range is poisoned
         *
         * \param pointer Start address of the memory range
         * \param size Size of the memory range in bytes
         * \return true if any byte in the range is poisoned, false otherwise
         */
        static bool IsRangePoisoned(void* pointer, Uint64 size);

        /**
         * \brief Unpoisons a memory region
         *
         * \param pointer Start address of the memory region
         * \param size Size of the memory region in bytes
         */
        static void Unpoison(void* pointer, Uint64 size);

        /**
         * \brief Annotates a contiguous container for AddressSanitizer
         *
         * Marks the portion of the container between \p previous and \p next as
         * accessible while keeping the remaining capacity poisoned.
         *
         * \tparam T Container element type
         * \param pointer Start address of the container storage
         * \param range Number of elements in the allocated storage
         * \param previous Previous number of accessible elements
         * \param next New number of accessible elements
         */
        template <typename T>
        static void AnnotateContiguousContainer(T* pointer, Uint64 range, Uint64 previous, Uint64 next)
        {
            AnnotateContiguousContainer(pointer, sizeof(T), range, previous, next);
        }

        /**
         * \brief Annotates a contiguous container for AddressSanitizer
         *
         * Marks the portion of the container between \p previous and \p next as
         * accessible while keeping the remaining capacity poisoned.
         *
         * \param pointer Start address of the container storage
         * \param elementSize Size of a single container element in bytes
         * \param range Number of elements in the allocated storage
         * \param previous Previous number of accessible elements
         * \param next New number of accessible elements
         */
        static void AnnotateContiguousContainer(void* pointer, Uint64 elementSize, Uint64 range, Uint64 previous, Uint64 next);
    };
}

#endif // COCKTAILENGINE_CORE_MEMORY_ASAN_HPP
