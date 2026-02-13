#ifndef COCKTAIL_CORE_UTILITY_BasicByteArray_HPP
#define COCKTAIL_CORE_UTILITY_BasicByteArray_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
    /**
     * \brief Utility class owning and managing a contiguous memory area of bytes
     *
     * BasicByteArray is a lightweight owning container that manages a dynamically
     * allocated contiguous block of memory.
     *
     * It provides:
     * - Ownership semantics (deep copy on copy, transfer on move)
     * - Byte-level memory manipulation
     * - Insert, append, prepend, slice and remove operations
     *
     * This container does not implement capacity growth strategies
     * (every modification reallocates the full memory block).
     *
     * \tparam TAllocator Allocator type used to allocate the internal memory.
     *         Must provide:
     *         - SizeType
     *         - template ForType<T>
     */
    template <typename TAllocator>
    class BasicByteArray
    {
    public:

        using SizeType = typename TAllocator::SizeType;
        using ElementAllocatorType = typename TAllocator::template ForType<Byte>;

        /**
         * \brief Default constructor
         *
         * Creates an empty BasicByteArray.
         */
        BasicByteArray() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Construct and allocate a memory block
         *
         * Allocates \p size bytes and fills them with \p value.
         *
         * \param size Number of bytes to allocate
         * \param value Initialization value (default: 0)
         *
         * \pre size > 0
         */
        explicit BasicByteArray(SizeType size, Byte value = 0) :
            mSize(size)
        {
            assert(mSize);

            mData = MakeUniqueWithAllocator<Byte[], ElementAllocatorType>(mAllocator, mSize);
            Memory::Set(mData.Get(), value, mSize);
        }

        /**
         * \brief Construct from an existing memory block
         *
         * Allocates a new memory block and copies \p length elements
         * from \p data.
         *
         * If T is not void, the number of copied bytes is:
         * length * sizeof(T)
         *
         * \tparam T Source data type
         * \param data Pointer to the source memory
         * \param length Number of elements to copy
         *
         * \pre data != nullptr
         * \pre length > 0
         */
        template <typename T>
        BasicByteArray(const T* data, SizeType length)
        {
            assert(data != nullptr);
            assert(length > 0);

            if constexpr (!std::is_void_v<T>)
                length *= sizeof(T);

            mSize = length;
            mData = MakeUniqueWithAllocator<Byte[]>(mAllocator, mSize);

            Memory::Copy(mData.Get(), data, mSize);
        }

        /**
         * \brief Copy constructor
         *
         * Performs a deep copy of the memory owned by \p other.
         *
         * \param other The BasicByteArray to copy
         */
        BasicByteArray(const BasicByteArray& other) :
            mSize(0)
        {
            *this = other;
        }

        /**
         * \brief Move constructor
         *
         * Transfers ownership of the memory owned by \p other.
         * After the move, \p other becomes empty.
         *
         * \param other The BasicByteArray to move
         */
        BasicByteArray(BasicByteArray&& other) noexcept :
            mSize(0)
        {
            *this = std::move(other);
        }

        /**
         * \brief Copy assignment operator
         *
         * Replaces the contents of this BasicByteArray with a deep
         * copy of \p other.
         *
         * \param other The BasicByteArray to copy
         *
         * \return *this
         */
        BasicByteArray& operator=(const BasicByteArray& other)
        {
            if (this != &other)
            {
                mData.Reset();
                mSize = other.mSize;

                if (mSize)
                {
                    mData = MakeUniqueWithAllocator<Byte[], ElementAllocatorType>(mAllocator, mSize);
                    Memory::Copy(mData.Get(), other.GetData(), mSize);
                }
            }

            return *this;
        }

        /**
         * \brief Move assignment operator
         *
         * Transfers ownership of the memory owned by \p other.
         * After the move, \p other becomes empty.
         *
         * \param other The BasicByteArray to move
         *
         * \return *this
         */
        BasicByteArray& operator=(BasicByteArray&& other) noexcept
        {
            if (this == &other)
                return *this;

            Clear();

            if constexpr (ElementAllocatorType::PropagateOnContainerMove)
            {
                mSize = std::exchange(other.mSize, 0);
                mData = std::exchange(other.mData, nullptr);
                mAllocator = std::move(other.mAllocator);
            }
            else
            {
                if (!other.IsEmpty())
                    Append(other);
            }

            return *this;
        }

        /**
         * \brief Append another BasicByteArray
         *
         * Equivalent to Insert(GetSize(), other).
         *
         * \param other Array to append
         *
         * \return *this
         */
        BasicByteArray& Append(const BasicByteArray& other)
        {
            return Insert(mSize, other);
        }

        /**
         * \brief Append a BasicByteArrayView
         *
         * \param other View to append
         *
         * \return *this
         */
        BasicByteArray& Append(const BasicByteArrayView<SizeType>& other)
        {
            return Insert(mSize, other);
        }

        /**
         * \brief Append raw bytes
         *
         * \param data Pointer to data
         * \param length Number of bytes
         *
         * \return *this
         *
         * \pre data != nullptr
         * \pre length > 0
         */
        BasicByteArray& Append(const Byte* data, SizeType length)
        {
            return Insert(mSize, data, length);
        }

        /**
         * \brief Prepend another BasicByteArray
         *
         * Equivalent to Insert(0, other).
         *
         * \param other Array to prepend
         *
         * \return *this
         */
        BasicByteArray& Prepend(const BasicByteArray& other)
        {
            return Insert(0, other);
        }

        /**
         * \brief Prepend a BasicByteArrayView
         *
         * \param other View to prepend
         *
         * \return *this
         */
        BasicByteArray& Prepend(const BasicByteArrayView<SizeType>& other)
        {
            return Insert(0, other);
        }

        /**
         * \brief Prepend raw bytes
         *
         * \param data Pointer to data
         * \param length Number of
         *
         * \return *this
         */
        BasicByteArray& Prepend(const Byte* data, SizeType length)
        {
            return Insert(0, data, length);
        }

        /**
         * \brief Insert another BasicByteArray at a given position
         *
         * \param where Insertion offset
         * \param other Array to insert
         *
         * \return *this
         *
         * \pre where <= GetSize()
         */
        BasicByteArray& Insert(SizeType where, const BasicByteArray& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        /**
         * \brief Insert a BasicByteArrayView at a given position
         *
         * \param where Insertion offset
         * \param other View to insert
         *
         * \return *this
         */
        BasicByteArray& Insert(SizeType where, const BasicByteArrayView<SizeType>& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        /**
         * \brief Insert raw bytes at a given position
         *
         * Allocates a new memory block and rebuilds the full array.
         *
         * \param where Insertion offset
         * \param data Pointer to data
         * \param length Number of bytes
         *
         * \return *this
         *
         * \pre where <= GetSize()
         * \pre data != nullptr
         * \pre length > 0
         */
        BasicByteArray& Insert(SizeType where, const Byte* data, SizeType length)
        {
            assert(where <= mSize);
            assert(data != nullptr);
            assert(length > 0);

            SizeType fullSize = mSize + length;
            UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocatorType>> fullData = MakeUniqueWithAllocator<Byte[], ElementAllocatorType>(mAllocator, fullSize);

            if (mSize)
            {
                if (where == 0)
                {
                    Memory::Copy(fullData.Get(), data, length);
                    Memory::Copy(fullData.Get() + length, mData.Get(), mSize);
                }
                else if (where == mSize)
                {
                    Memory::Copy(fullData.Get(), mData.Get(), mSize);
                    Memory::Copy(fullData.Get() + mSize, data, length);
                }
                else
                {
                    SizeType offset = 0;
                    Memory::Copy(fullData.Get() + offset, mData.Get(), where);

                    offset += where;
                    Memory::Copy(fullData.Get() + offset, data, length);

                    offset += length;
                    Memory::Copy(fullData.Get() + offset, mData.Get() + where, mSize - where);
                }
            }
            else
            {
                Memory::Copy(fullData.Get(), data, fullSize);
            }

            mSize = fullSize;
            mData = std::move(fullData);

            return *this;
        }

        /**
         * \brief Return a slice from offset to end
         *
         * \param offset Starting offset
         *
         * \return A new BasicByteArray containing the slice
         *
         * \pre offset <= GetSize()
         */
        BasicByteArray Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        /**
         * \brief Return a slice of the memory
         *
         * \param offset Starting offset
         * \param length Number of bytes
         *
         * \return A new BasicByteArray containing the slice
         *
         * \pre offset + length <= GetSize()
         */
        BasicByteArray Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length <= mSize);

            return BasicByteArray(mData.Get() + offset, length);
        }

        /**
         * \brief Remove bytes from offset to end
         *
         * \param offset Starting offset
         *
         * \return *this
         *
         * \pre offset <= GetSize()
         */
        BasicByteArray& Remove(SizeType offset)
        {
            return Remove(offset, mSize - offset);
        }

        /**
         * \brief Remove a portion of the memory
         *
         * Allocates a new memory block and rebuilds the array.
         *
         * \param offset Starting offset
         * \param length Number of bytes to remove
         *
         * \return *this
         *
         * \pre offset + length <= GetSize()
         */
        BasicByteArray& Remove(SizeType offset, SizeType length)
        {
            assert(offset + length <= mSize);

            SizeType fullSize = mSize - length;
            UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocatorType>> fullData = MakeUniqueWithAllocator<Byte[], ElementAllocatorType>(mAllocator, fullSize);

            if (offset == 0)
            {
                Memory::Copy(fullData.Get(), mData.Get() + length, fullSize);
            }
            else if (offset + length == mSize)
            {
                Memory::Copy(fullData.Get(), mData.Get(), fullSize);
            }
            else
            {
                Memory::Copy(fullData.Get(), mData.Get(), offset);
                Memory::Copy(fullData.Get() + offset, mData.Get() + offset + length, fullSize - offset);
            }

            mSize = fullSize;
            mData = std::move(fullData);

            return *this;
        }

        /**
         * \brief Resize the memory block
         *
         * If newSize > current size, new bytes are initialized with \p value.
         *
         * \param newSize New size in bytes
         * \param value Initialization value for new bytes
         */
        void Resize(SizeType newSize, Byte value = 0)
        {
            if (newSize == mSize)
                return;

            if (newSize == 0)
            {
                mSize = 0;
                mData = nullptr;
                return;
            }

            auto newData = MakeUniqueWithAllocator<Byte[], ElementAllocatorType>(mAllocator, newSize);
            if (newSize > mSize)
            {
                Memory::Copy(newData.Get(), mData.Get(), mSize);
                Memory::Set(newData.Get() + mSize, value, newSize - mSize);
            }
            else
            {
                Memory::Copy(newData.Get(), mData.Get(), newSize);
            }

            mSize = newSize;
            mData = std::move(newData);
        }

        /**
         * \brief Clears the array, removing all elements
         */
        void Clear()
        {
            mSize = 0;
            mData.Reset();
        }

        /**
         * \brief Access a byte at a given index
         *
         * \param index Index to access
         *
         * \return Reference to the byte
         *
         * \pre index < GetSize()
         */
        Byte& At(SizeType index)
        {
            assert(index < mSize);
            return mData[index];
        }

        /**
         * \brief Const access to a byte
         *
         * \param index Index to access
         *
         * \return Const reference to the byte
         */
        const Byte& At(SizeType index) const
        {
            assert(index < mSize);
            return mData[index];
        }

        /**
         * \brief Check whether the array is empty
         *
         * \return True if size is zero
         */
        bool IsEmpty() const
        {
            return mSize == 0;
        }

        /**
         * \brief Get the size in bytes
         *
         * \return Number of bytes stored
         */
        SizeType GetSize() const
        {
            return mSize;
        }

        /**
         * \brief Get mutable pointer to the data
         *
         * \return Pointer to internal memory
         */
        Byte* GetData()
        {
            return mData.Get();
        }

        /**
         * \brief Get const pointer to the data
         *
         * \return Const pointer to internal memory
         */
        const Byte* GetData() const
        {
            return mData.Get();
        }

        /**
         * \brief Equality comparison
         *
         * Two arrays are equal if they have the same size
         * and identical byte contents.
         */
        bool operator==(const BasicByteArray& rhs) const
        {
            return *this == BasicByteArrayView(rhs);
        }

        /**
         * \brief Equality comparison with a view
         */
        bool operator==(const BasicByteArrayView<SizeType>& rhs) const
        {
            return BasicByteArrayView(*this) == BasicByteArrayView(rhs);
        }

        /**
         * \brief Inequality comparison
         */
        bool operator!=(const BasicByteArray& rhs) const
        {
            return !(*this == rhs);
        }

        /**
         * \brief Inequality comparison with a view
         */
        bool operator!=(const BasicByteArrayView<SizeType>& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize; /*!< Size in bytes */
        UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocatorType>> mData; /*!< Owned memory */
        ElementAllocatorType mAllocator; /*!< Allocator instance */
    };

    /**
     * \brief Default heap-based byte array
     */
    using ByteArray = BasicByteArray<HeapAllocator>;

    /**
     * \brief Heap-based byte array optimized for large allocations
     */
    using LargeByteArray = BasicByteArray<LargeHeapAllocator>;
}

#endif // COCKTAIL_CORE_UTILITY_BasicByteArray_HPP
