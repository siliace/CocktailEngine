#ifndef COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDLINEARALLOCATOR_HPP
#define COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDLINEARALLOCATOR_HPP

#include <Cocktail/Core/Memory/ObjectMemoryUtils.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    /**
     * \brief Allocator that provides inline storage for a fixed number of elements
     *        and falls back to a secondary allocator when the inline buffer is full.
     *
     * \tparam InlineSize Number of inline elements the allocator can store without heap allocation.
     * \tparam IndexSize  Size (in bits) of the integer type representing allocation lengths.
     * \tparam TSecondaryAllocator Allocator used when the inline storage is exhausted.
     */
    template <Uint32 InlineSize, Uint32 IndexSize, typename TSecondaryAllocator = SizedHeapAllocator<IndexSize>>
    class SizedInlineAllocator
    {
    public:

        static_assert(InlineSize > 0, "SizedInlineAllocator should not be used with 0 inline elements");

        /**
         * \brief Integer type used to represent allocation sizes
         */
        using SizeType = SizeTypeOf<IndexSize>;

        static_assert(std::is_same_v<SizeType, typename TSecondaryAllocator::SizeType>, "Linear allocator and its secondary allocator should have the same size type");

        /**
         * \brief Per-type interface providing allocation and deallocation methods for objects of type T
         *
         * \tparam T Type of objects to be allocated
         */
        template <typename T>
        class ForType
        {
        public:

	        using ValueType = T;

            /**
             * \brief Flag indicating whether the allocator can be moved from a container to another
             *
             * Linear allocators are owning data so they shall not be copied or moved.
             */
            static constexpr bool PropagateOnContainerMove = false;

            /**
             * \brief Convenience alias for the secondary allocator instantiated for T
             */
            using SecondaryAllocatorType = typename TSecondaryAllocator::template ForType<T>;

            /**
             * \brief Default constructor
             */
            ForType() :
                mInlineCount(0)
            {
                /// Nothing
            }

            ForType(const ForType&) = delete;
            ForType(ForType&&) noexcept = default;
            ForType& operator=(const ForType&) = delete;
            ForType& operator=(ForType&&) noexcept = default;

            /**
             * \brief Allocates memory for \p size elements of type T
             *
             * The allocator first attempts to allocate within the inline buffer.
             * If insufficient inline space remains, allocation is forwarded to the secondary allocator.
             *
             * \param size Number of elements to allocate
             *
             * \return Pointer to the allocated memory block
             */
            T* Allocate(SizeType size)
            {
                if (mInlineCount + size <= InlineSize)
                {
                    T* first = GetInlineBegin() + mInlineCount;
                    mInlineCount += size;

                    return first;
                }

                return mSecondaryAllocator.Allocate(size);
            }

            /**
             * \brief Deallocates memory previously allocated by Allocate()
             *
             * If the pointer lies within the inline buffer, the function performs no action,
             * as inline allocations are freed only when the allocator itself is destroyed.
             * Otherwise, the request is forwarded to the secondary allocator.
             *
             * \param pointer Pointer to the memory block to deallocate
             */
            void Deallocate(T* pointer)
            {
                if (pointer >= GetInlineBegin() && pointer < GetInlineEnd())
                    return;

                mSecondaryAllocator.Deallocate(pointer);
            }

        private:

            using StorageType = std::aligned_storage_t<sizeof(T), alignof(T)>;

            /**
             * \brief Returns a pointer to the beginning of the inline storage buffer
             *
             * \return Pointer to the start of the inline buffer
             */
            T* GetInlineBegin()
            {
                return reinterpret_cast<T*>(&mInlineData[0]);
            }

            /**
             * \brief Returns a pointer to the end of the inline storage buffer
             *
             * \return Pointer one past the last inline element
             */
            T* GetInlineEnd()
            {
                return GetInlineBegin() + InlineSize;
            }

            SizeType mInlineCount; /*!< Number of currently allocated inline elements */
            StorageType mInlineData[InlineSize]; /*!< Inline storage buffer */
            SecondaryAllocatorType mSecondaryAllocator; /*!< Secondary allocator used when inline capacity is exceeded */
        };
    };

    /**
     * \brief Alias for a SizedInlineAllocator using 32-bit size indices
     *
     * \tparam InlineSize Number of inline elements available before heap allocation is required
     */
    template <Uint32 InlineSize>
    using LinearAllocator = SizedInlineAllocator<InlineSize, 32>;

    /**
     * \brief Alias for a SizedInlineAllocator using 64-bit size indices
     *
     * \tparam InlineSize Number of inline elements available before heap allocation is required
     */
    template <Uint32 InlineSize>
    using LargeLinearAllocator = SizedInlineAllocator<InlineSize, 64>;
}

#endif // COCKTAIL_CORE_MEMORY_ALLOCATOR_SIZEDLINEARALLOCATOR_HPP
