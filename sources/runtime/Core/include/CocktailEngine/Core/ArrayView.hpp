#ifndef COCKTAILENGINE_CORE_ARRAYVIEW_HPP
#define COCKTAILENGINE_CORE_ARRAYVIEW_HPP

#include <CocktailEngine/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <CocktailEngine/Core/Utility/ExceptionUtils.hpp>
#include <CocktailEngine/Core/Utility/Optional.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <type_traits>

namespace Ck
{
    template <typename, typename>
    class Array;

    /**
     * \brief A lightweight, non-owning, read-only view over a contiguous sequence of elements
     *
     * ArrayView references memory owned by someone else (an Array, a C array, ...). It never
     * allocates, copies elements or modifies them. The viewed memory must outlive the view
     * and must not be reallocated while the view is in use.
     *
     * \tparam E The type of the viewed elements
     * \tparam TSizeType The type used to represent sizes and indices
     */
    template <typename E, typename TSizeType = HeapAllocator::SizeType>
    class ArrayView
    {
    public:

        class ConstIterator;
        using Iterator = ConstIterator;

        /**
         * \brief The type of the viewed elements
         */
        using ElementType = E;

        /**
         * \brief The type used to represent sizes and indices
         */
        using SizeType = TSizeType;

        /**
         * \brief Read-only random-access iterator used to traverse an ArrayView
         */
        class ConstIterator
        {
        public:

            using value_type = E;
            using difference_type = std::make_signed_t<SizeType>;
            using pointer = const E*;
            using reference = const E&;
            using iterator_category = std::random_access_iterator_tag;

            /**
             * \brief Constructs an iterator at an index in a view
             *
             * \param owner The view to iterate through
             * \param firstIndex The initial index
             */
            explicit ConstIterator(const ArrayView& owner, SizeType firstIndex = 0) noexcept :
                mOwner(&owner),
                mIndex(firstIndex)
            {
                /// Nothing
            }

            ConstIterator(const ConstIterator&) = default;
            ConstIterator& operator=(const ConstIterator&) noexcept = default;

            /**
             * \brief Moves the iterator backward by a number of elements
             */
            ConstIterator& Rewind(SizeType count = 1)
            {
                mIndex -= count;
                return *this;
            }

            /**
             * \brief Moves the iterator forward by a number of elements
             */
            ConstIterator& Advance(SizeType count = 1)
            {
                mIndex += count;
                return *this;
            }

            /**
             * \brief Checks whether the iterator points to an element in its view
             */
            bool IsValid() const
            {
                return mIndex < mOwner->GetSize();
            }

            /**
             * \brief Returns the referenced element
             */
            const E& GetValue() const
            {
                return (*mOwner)[mIndex];
            }

            /**
             * \brief Returns an iterator positioned on the previous element
             */
            ConstIterator Previous() const
            {
                ConstIterator previous(*this);
                previous.Rewind();
                return previous;
            }

            /**
             * \brief Returns an iterator positioned on the next element
             */
            ConstIterator Next() const
            {
                ConstIterator next(*this);
                next.Advance();
                return next;
            }

            bool operator==(const ConstIterator& rhs) const
            {
                return mOwner == rhs.mOwner && mIndex == rhs.mIndex;
            }

            bool operator!=(const ConstIterator& rhs) const
            {
                return !(*this == rhs);
            }

            bool operator<(const ConstIterator& rhs) const
            {
                return mIndex < rhs.mIndex;
            }

            bool operator<=(const ConstIterator& rhs) const
            {
                return !(rhs < *this);
            }

            bool operator>(const ConstIterator& rhs) const
            {
                return rhs < *this;
            }

            bool operator>=(const ConstIterator& rhs) const
            {
                return !(*this < rhs);
            }

            ConstIterator operator+(difference_type count) const
            {
                ConstIterator iterator(*this);
                iterator += count;
                return iterator;
            }

            ConstIterator& operator+=(difference_type count)
            {
                mIndex = static_cast<SizeType>(static_cast<difference_type>(mIndex) + count);
                return *this;
            }

            friend ConstIterator operator+(difference_type count, ConstIterator iterator)
            {
                return iterator + count;
            }

            ConstIterator operator-(difference_type count) const
            {
                ConstIterator iterator(*this);
                iterator -= count;
                return iterator;
            }

            ConstIterator& operator-=(difference_type count)
            {
                return *this += -count;
            }

            difference_type operator-(const ConstIterator& rhs) const
            {
                return static_cast<difference_type>(mIndex) - static_cast<difference_type>(rhs.mIndex);
            }

            ConstIterator& operator++()
            {
                return Advance();
            }

            ConstIterator operator++(int)
            {
                ConstIterator current(*this);
                Advance();
                return current;
            }

            ConstIterator& operator--()
            {
                return Rewind();
            }

            ConstIterator operator--(int)
            {
                ConstIterator current(*this);
                Rewind();
                return current;
            }

            const E* operator->() const
            {
                return mOwner->GetData() + mIndex;
            }

            const E& operator*() const
            {
                return GetValue();
            }

        private:

            const ArrayView* mOwner;
            SizeType mIndex;
        };

        /**
         * \brief Default constructor
         *
         * Creates an empty view pointing to no data.
         */
        constexpr ArrayView() noexcept :
            mData(nullptr),
            mSize(0)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a view from a pointer and an element count
         *
         * \param data A pointer to the first element, may be null only if \p size is 0
         * \param size The number of elements in the view
         */
        constexpr ArrayView(const E* data, SizeType size) noexcept :
            mData(data),
            mSize(size)
        {
            assert(data != nullptr || size == 0);
        }

        /**
         * \brief Constructs a view over a C array
         *
         * \tparam N The number of elements of the C array
         *
         * \param elements The C array to view
         */
        template <std::size_t N>
        constexpr ArrayView(const E (&elements)[N]) noexcept :
            mData(elements),
            mSize(static_cast<SizeType>(N))
        {
            /// Nothing
        }

        /**
         * \brief Constructs a view over the whole content of an Array
         *
         * \tparam TAllocator The allocator type used by the Array
         *
         * \param array The Array to view
         */
        template <typename TAllocator>
        ArrayView(const Array<E, TAllocator>& array) noexcept :
            ArrayView(array.GetData(), static_cast<SizeType>(array.GetSize()))
        {
            /// Nothing
        }

        /**
         * \brief Returns a const reference to the element at the given index
         *
         * \param index The zero-based index of the element
         *
         * \return A const reference to the element
         *
         * \throws OutOfRange If \p index is outside the valid range
         */
        const E& At(SizeType index) const
        {
            if (index >= mSize)
                ExceptionUtils::ThrowOutOfRange(index, mSize);

            return mData[index];
        }

        /**
         * \brief Attempts to access the element at the given index
         *
         * \param index The zero-based index of the element
         *
         * \return An Optional referencing the element, or empty if \p index is out of range
         */
        Optional<const E&> TryAt(SizeType index) const
        {
            if (index >= mSize)
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(mData[index]);
        }

        /**
         * \brief Returns a const reference to the first element
         *
         * \throws Exception If the view is empty
         */
        const E& First() const
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mData[0];
        }

        /**
         * \brief Attempts to access the first element
         *
         * \return An Optional referencing the first element, or empty if the view is empty
         */
        Optional<const E&> TryFirst() const
        {
            if (IsEmpty())
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(mData[0]);
        }

        /**
         * \brief Returns a const reference to the last element
         *
         * \throws Exception If the view is empty
         */
        const E& Last() const
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mData[mSize - 1];
        }

        /**
         * \brief Attempts to access the last element
         *
         * \return An Optional referencing the last element, or empty if the view is empty
         */
        Optional<const E&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const E&>::Empty();

            return Optional<const E&>::Of(mData[mSize - 1]);
        }

        /**
         * \brief Returns a sub-view starting at the given index and extending to the end
         *
         * \param offset The zero-based index of the first element of the sub-view
         *
         * \return A view over the requested range
         *
         * \throws OutOfRange If \p offset is greater than GetSize()
         */
        ArrayView Slice(SizeType offset) const
        {
            if (offset > mSize)
                ExceptionUtils::ThrowOutOfRange(offset, mSize);

            return ArrayView(mData + offset, mSize - offset);
        }

        /**
         * \brief Returns a sub-view starting at the given index with at most \p count elements
         *
         * If \p count exceeds the remaining elements, the sub-view is clamped.
         *
         * \param offset The zero-based index of the first element of the sub-view
         * \param count The maximum number of elements of the sub-view
         *
         * \return A view over the requested range
         *
         * \throws OutOfRange If \p offset is greater than GetSize()
         */
        ArrayView Slice(SizeType offset, SizeType count) const
        {
            if (offset > mSize)
                ExceptionUtils::ThrowOutOfRange(offset, mSize);

            return ArrayView(mData + offset, std::min(count, static_cast<SizeType>(mSize - offset)));
        }

        /**
         * \brief Finds the first occurrence of an element
         *
         * \param element The element to search for
         * \param start The index to start searching from
         *
         * \return The index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindIndex(const E& element, SizeType start = 0) const
        {
            for (SizeType i = start; i < mSize; ++i)
            {
                if (mData[i] == element)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the last occurrence of an element
         *
         * \param element The element to search for
         *
         * \return The index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLastIndex(const E& element) const
        {
            for (SizeType i = mSize; i > 0; --i)
            {
                if (mData[i - 1] == element)
                    return Optional<SizeType>::Of(i - 1);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Checks whether the view contains an element
         *
         * \param element The element to search for
         *
         * \return True if at least one element is equal to \p element
         */
        bool Contains(const E& element) const
        {
            return !FindIndex(element).IsEmpty();
        }

        /**
         * \brief Checks whether this view starts with the elements of another view
         *
         * \param prefix The expected beginning
         *
         * \return True if the first elements of this view are equal to \p prefix
         */
        bool StartsWith(ArrayView prefix) const
        {
            return prefix.mSize <= mSize && Slice(0, prefix.mSize) == prefix;
        }

        /**
         * \brief Checks whether this view ends with the elements of another view
         *
         * \param suffix The expected ending
         *
         * \return True if the last elements of this view are equal to \p suffix
         */
        bool EndsWith(ArrayView suffix) const
        {
            return suffix.mSize <= mSize && Slice(mSize - suffix.mSize) == suffix;
        }

        /**
         * \brief Returns an iterator on the first element
         *
         * If the view is empty, the returned iterator is invalid and equal to the end iterator.
         */
        ConstIterator GetIterator() const
        {
            return ConstIterator(*this);
        }

        /**
         * \brief Returns an iterator on the last element
         *
         * If the view is empty, the returned iterator is invalid.
         */
        ConstIterator GetLastIterator() const
        {
            return ConstIterator(*this, GetSize() - 1);
        }

        /**
         * \brief Returns an iterator following the last element
         */
        ConstIterator GetEndIterator() const
        {
            return ConstIterator(*this, GetSize());
        }

        /**
         * \brief Checks whether this view is empty
         */
        constexpr bool IsEmpty() const noexcept
        {
            return mSize == 0;
        }

        /**
         * \brief Returns the number of elements in the view
         */
        constexpr SizeType GetSize() const noexcept
        {
            return mSize;
        }

        /**
         * \brief Returns a pointer to the first element, or nullptr for a default constructed view
         */
        constexpr const E* GetData() const noexcept
        {
            return mData;
        }

        /**
         * \brief Returns a const reference to the element at the given index without bounds checking
         */
        const E& operator[](SizeType index) const
        {
            assert(index < mSize);
            return mData[index];
        }

        /**
         * \brief Compares two views element by element
         */
        friend bool operator==(const ArrayView& lhs, const ArrayView& rhs)
        {
            if (lhs.mSize != rhs.mSize)
                return false;

            if (lhs.mData == rhs.mData)
                return true;

            for (SizeType i = 0; i < lhs.mSize; ++i)
            {
                if (!(lhs.mData[i] == rhs.mData[i]))
                    return false;
            }

            return true;
        }

        /**
         * \brief Compares two views element by element
         */
        friend bool operator!=(const ArrayView& lhs, const ArrayView& rhs)
        {
            return !(lhs == rhs);
        }

    private:

        const E* mData;
        SizeType mSize;
    };

    /**
     * \brief Returns an iterator on the first element of an ArrayView
     */
    template <typename E, typename TSizeType>
    typename ArrayView<E, TSizeType>::ConstIterator begin(const ArrayView<E, TSizeType>& view)
    {
        return view.GetIterator();
    }

    /**
     * \brief Returns an iterator following the last element of an ArrayView
     */
    template <typename E, typename TSizeType>
    typename ArrayView<E, TSizeType>::ConstIterator end(const ArrayView<E, TSizeType>& view)
    {
        return view.GetEndIterator();
    }
}

#endif // COCKTAILENGINE_CORE_ARRAYVIEW_HPP
