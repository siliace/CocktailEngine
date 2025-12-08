#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP

#include <Cocktail/Core/Memory/Memory.hpp>
#include <Cocktail/Core/Memory/SizeType.hpp>

namespace Ck
{
    /**
     * \class SizedHeapAllocator
     *
     * \brief Typed heap allocator with configurable index size and allocation backend
     *
     * This allocator provides typed heap allocation for objects of type \p T or for raw memory,
     * using a configurable integer size type based on \p IndexSize and a customizable
     * memory allocation handler (defaulting to Memory).
     *
     * \tparam IndexSize Number of bits used to define the internal size type
     * \tparam AllocationHandler Memory allocation backend providing Allocate/Free
     */
	template <Uint32 IndexSize, typename AllocationHandler = Memory>
	class SizedHeapAllocator
	{
	public:

	    /**
         * \brief Integer type used to represent allocation sizes
         */
	    using SizeType = SizeTypeOf<IndexSize>;

        /**
	     * \brief Specialized allocator for \p T type
	     *
	     * \tparam T The type of object allocated by this allocator
	     */
	    template <typename T>
	    struct ForType
	    {
	        using ValueType = T;

	        /**
             * \brief Flag indicating whether the allocator can be moved from a container to another
             *
             * Heap allocators does not own any data so we allow the move to improve performances.
             */
            static constexpr bool PropagateOnContainerMove = true;

	        /**
             * \brief Allocates a block of memory large enough to hold \p size elements of type T
             *
             * \param size Number of elements to allocate
             *
             * \return Pointer to the allocated memory, or nullptr on failure
             */
	        T* Allocate(SizeType size)
	        {
	            return static_cast<T*>(AllocationHandler::Allocate(size * sizeof(T)));
	        }

	        /**
             * \brief Frees a previously allocated block of memory
             *
             * \param pointer Pointer to the memory block to free
             */
	        void Deallocate(T* pointer)
	        {
	            AllocationHandler::Free(pointer);
	        }
	    };
	};

    /**
     * \brief Alias for a standard heap allocator using a 32-bit index size
     *
     * Suitable for most medium-scale allocations.
     */
    using HeapAllocator = SizedHeapAllocator<32>;

    /**
     * \brief Alias for a heap allocator using a 64-bit index size
     *
     * Suitable for very large allocations where a wider size type is needed.
     */
    using LargeHeapAllocator = SizedHeapAllocator<64>;
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDHEAPALLOCATOR_HPP
