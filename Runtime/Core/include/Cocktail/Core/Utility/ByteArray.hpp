#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
    /**
     * \brief Utility class to own and manage a memory area
     */
    template <typename TAllocator = HeapAllocator>
    class COCKTAIL_CORE_API ByteArray
    {
    public:

        using SizeType = typename TAllocator::SizeType;
        using ElementAllocator = typename TAllocator::template ForType<Uint8>;

        /**
         * \brief Default constructor
         */
        ByteArray() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Create a ByteArray by allocating and filling its own memory area.
         *
         * \param size The size of the memory area to create
         * \param value The value to fill the memory with
         */
        explicit ByteArray(SizeType size, Uint8 value = 0) :
            mSize(size)
        {
            assert(mSize);

            mData = MakeUniqueWithAllocator<Uint8[], ElementAllocator>(mSize);
            std::memset(mData.Get(), value, mSize);
        }

        /**
         * \brief Constructor
         *
         * Create a ByteArray by copying an existing memory area.
         *
         * \param data A pointer to the memory area to copy
         * \param length The length to copy
         */
        ByteArray(const void* data, SizeType length)
        {
            assert(data != nullptr);
            assert(length > 0);

            mSize = length;
            mData = MakeUniqueWithAllocator<Uint8[]>(mAllocator, mSize);

            Memory::Copy(mData.Get(), data, length);
        }

        /**
         * \brief Copy constructor
         *
         * Create a ByteArray by copying ownership of the one of another ByteArray.
         *
         * \param other The ByteArray to copy
         */
        ByteArray(const ByteArray& other) :
            mSize(0)
        {
            *this = other;
        }

        /**
         * \brief Move constructor
         *
         * Create a ByteArray by taking ownership of the one of another ByteArray.
         *
         * \param other The ByteArray to move
         */
        ByteArray(ByteArray&& other) noexcept :
            mSize(0)
        {
            *this = std::move(other);
        }

        /**
         * \brief Assignment operator
         *
         * Assign this ByteArray to a copy of another one.
         *
         * \param other The ByteArray to copy
         * 
         * \return This
         */
        ByteArray& operator=(const ByteArray& other)
        {
            if (this != &other)
            {
                mData.Reset();
                mSize = other.mSize;

                if (mSize)
                {
                    mData = MakeUniqueWithAllocator<Uint8[], ElementAllocator>(mAllocator, mSize);
                    Memory::Copy(mData.Get(), other.GetData(), mSize);
                }
            }

            return *this;
        }

        /**
         * \brief Assignment operator
         * Assign this ByteArray to a move of another one
         * \param other The ByteArray to move
         * \return This
         */
        ByteArray& operator=(ByteArray&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mData = std::move(other.mData);

            return *this;
        }
        ByteArray& Append(const ByteArray& other)
        {
            return Insert(mSize, other);
        }

        ByteArray& Append(const ByteArrayView<SizeType>& other)
        {
            return Insert(mSize, other);
        }

        ByteArray& Append(const void* data, SizeType length)
        {
            return Insert(mSize, data, length);
        }

        ByteArray& Prepend(const ByteArray& other)
        {
            return Insert(0, other);
        }

        ByteArray& Prepend(const ByteArrayView<SizeType>& other)
        {
            return Insert(0, other);
        }

        ByteArray& Prepend(const void* data, SizeType length)
        {
            return Insert(0, data, length);
        }

        ByteArray& Insert(SizeType where, const ByteArray& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        ByteArray& Insert(SizeType where, const ByteArrayView<SizeType>& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        ByteArray& Insert(SizeType where, const void* data, SizeType length)
        {
            assert(where <= mSize);
            assert(data != nullptr);
            assert(length > 0);

            SizeType fullSize = mSize + length;
            UniquePtr<Uint8[]> fullData = MakeUniqueWithAllocator<Uint8[], ElementAllocator>(fullSize);

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

        ByteArray Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        ByteArray Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length < mSize);
            return ByteArray(mData.Get() + offset, length);
        }

        ByteArray& Remove(SizeType offset)
        {
            return Remove(offset, mSize - offset);
        }

        ByteArray& Remove(SizeType offset, SizeType length)
        {
            assert(offset + length < mSize);

            SizeType fullSize = mSize - length;
            UniquePtr<Uint8[]> fullData = MakeUniqueWithAllocator<Uint8[], ElementAllocator>(fullSize);

            if (offset == 0)
            {
                Memory::Copy(fullData.Get(), mData.Get() + length, fullSize);
            }
            else if (offset + length == mSize - 1)
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

        void Resize(SizeType newSize, Uint8 value)
        {
            if (newSize == mSize)
                return;

            UniquePtr<Uint8[]> newData = MakeUniqueWithAllocator<Uint8[], ElementAllocator>(newSize);
            if (newSize > mSize)
            {
                Memory::Copy(newData.Get(), mData.Get(), mSize);
                std::memset(newData.Get() + mSize, value, newSize - mSize);
            }
            else
            {
                Memory::Copy(newData.Get(), mData.Get(), newSize);
            }

            mSize = newSize;
            mData = std::move(newData);
        }

        Uint8& At(SizeType index)
        {
            assert(index < mSize);
            return mData[index];
        }

        const Uint8& At(SizeType index) const
        {
            assert(index < mSize);
            return mData[index];
        }

        bool IsEmpty() const
        {
            return mSize == 0;
        }

        SizeType GetSize() const
        {
            return mSize;
        }

        Uint8* GetData()
        {
            return mData.Get();
        }

        const Uint8* GetData() const
        {
            return mData.Get();
        }

        bool operator==(const ByteArray& rhs) const
        {
            return *this == ByteArrayView(rhs);
        }

        bool operator==(const ByteArrayView<SizeType>& rhs) const
        {
            return ByteArrayView(*this) == ByteArrayView(rhs);
        }

        bool operator!=(const ByteArray& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator!=(const ByteArrayView<SizeType>& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize;
        UniquePtr<Uint8[], AllocatorAwareDeleter<Uint8, ElementAllocator>> mData;
        ElementAllocator mAllocator;
    };
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAY_HPP
