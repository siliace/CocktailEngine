#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename>
    class BasicByteArray;

    /**
     * \brief Utility class to access to a memory area
     */
    template <typename TSizeType>
    class BasicByteArrayView
    {
    public:

        using SizeType = TSizeType;

        /**
         * \brief Default constructor
         *
         * Creates an empty view. \ref IsEmpty() will return true.
         */
        BasicByteArrayView() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a view over an existing memory block
         *
         * \param data Pointer to the memory area
         * \param size Number of bytes in the view
         *
         * \note The view does not copy the memory. The caller must ensure
         *       the memory remains valid for the lifetime of the view.
         */
        BasicByteArrayView(const Byte* data, SizeType size) :
            mSize(size),
            mData(data)
        {
            assert(mData != nullptr);
        }

        /**
         * \brief Constructs a view from a \ref BasicByteArray
         *
         * \tparam TAllocator Allocator type of the byte array
         *
         * \param byteArray The byte array to view
         * \param offset Starting offset in the array (default 0)
         *
         * \note The view does not copy the memory.
         */
        template <typename TAllocator>
        BasicByteArrayView(const BasicByteArray<TAllocator>& byteArray, SizeType offset = 0) :
            BasicByteArrayView(byteArray, offset, byteArray.GetSize() - offset)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a view from a \ref BasicByteArray with offset and length
         *
         * \param byteArray The byte array to view
         * \param offset Starting offset in the array
         * \param length Number of bytes to include in the view
         *
         * \note The view does not copy the memory.
         */
        template <typename TAllocator>
        BasicByteArrayView(const BasicByteArray<TAllocator>& byteArray, SizeType offset, SizeType length)
        {
            assert(offset + length <= byteArray.GetSize());

            mSize = length;
            mData = byteArray.GetData() + offset;
        }

        /**
         * \brief Copy constructor
         *
         * Creates a new view referencing the same memory area as \p other.
         */
        BasicByteArrayView(const BasicByteArrayView& other) :
            mSize(other.mSize),
            mData(other.mData)
        {
            /// Nothing
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of the view state from \p other. The memory is not moved,
         * only the view metadata.
         */
        BasicByteArrayView(BasicByteArrayView&& other) noexcept :
            mSize(0),
            mData(nullptr)
        {
            *this = std::move(other);
        }

        /**
         * \brief Copy assignment
         *
         * Copies the view metadata from another view. Does not copy the underlying memory.
         *
         * \param other The view to copy from
         *
         * \return Reference to this view
         */
        BasicByteArrayView& operator=(const BasicByteArrayView& other)
        {
            if (this != &other)
            {
                mSize = other.mSize;
                mData = other.mData;
            }

            return *this;
        }

        /**
         * \brief Move assignment
         *
         * Moves the view metadata from another view. The memory is not moved,
         * only the view metadata.
         *
         * \param other The view to move from
         *
         * \return Reference to this view
         */
        BasicByteArrayView& operator=(BasicByteArrayView&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mData = std::exchange(other.mData, nullptr);

            return *this;
        }

        /**
         * \brief Returns a subview starting at \p offset
         *
         * \param offset Index from which to start the slice
         *
         * \return A new \ref BasicByteArrayView representing the slice
         */
        BasicByteArrayView Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        /**
         * \brief Returns a subview starting at \p offset with given \p length
         *
         * \param offset Index from which to start the slice
         * \param length Number of bytes to include in the slice
         *
         * \return A new \ref BasicByteArrayView representing the slice
         *
         * \note \p offset + \p length must be within the bounds of the view
         */
        BasicByteArrayView Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length < mSize);

            BasicByteArrayView view;
            view.mSize = length;
            view.mData = mData + offset;

            return view;
        }

        /**
         * \brief Returns a reference to the byte at \p index
         *
         * \param index Index of the byte to access
         *
         * \return Reference to the byte
         *
         * \note Asserts if \p index >= \ref GetSize()
         */
        const Byte& At(SizeType index) const
        {
            assert(index < mSize);
            return mData[index];
        }

        /**
         * \brief Checks if the view is empty
         *
         * \return \c true if the view contains zero bytes, \c false otherwise
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Returns the number of bytes in the view
         *
         * \return Size of the view in bytes
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Returns a pointer to the first byte in the view
         *
         * \return Pointer to the memory area
         */
        const Byte* GetData() const
        {
            return mData;
        }

        /**
         * \brief Compares the view to a \ref BasicByteArray
         *
         * \tparam TAllocator Allocator type of the byte array
         *
         * \param rhs The byte array to compare against
         *
         * \return \c true if the memory contents are identical, \c false otherwise
         */
        template <typename TAllocator>
        bool operator==(const BasicByteArray<TAllocator>& rhs) const
        {
            return *this == ByteArrayView(rhs);
        }

        /**
         * \brief Compares the view to another view
         *
         * \param rhs The view to compare against
         *
         * \return \c true if the memory contents are identical, \c false otherwise
         */
        bool operator==(const BasicByteArrayView& rhs) const
        {
            if (GetSize() != rhs.GetSize())
                return false;

            for (SizeType i = 0; i < GetSize(); i++)
            {
                if (At(i) != rhs.At(i))
                    return false;
            }

            return true;
        }

        /**
         * \brief Compares the view to a \ref BasicByteArray for inequality
         *
         * \tparam TAllocator Allocator type of the byte array
         *
         * \param rhs The byte array to compare against
         *
         * \return \c true if the memory contents differ, \c false otherwise
         */
        template <typename TAllocator>
        bool operator!=(const BasicByteArray<TAllocator>& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Compares the view to another view for inequality
         *
         * \param rhs The view to compare against
         *
         * \return \c true if the memory contents differ, \c false otherwise
         */
        bool operator!=(const BasicByteArrayView& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize; /*!< Number of bytes in the view */
        const Byte* mData; /*!< Pointer to the memory area */
    };

    /**
     * \brief Default view type using HeapAllocator size type
     */
    using ByteArrayView = BasicByteArrayView<HeapAllocator::SizeType>;

    /**
     * \brief View type using LargeHeapAllocator size type
     */
    using LargeByteArrayView = BasicByteArrayView<LargeHeapAllocator::SizeType>;
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
