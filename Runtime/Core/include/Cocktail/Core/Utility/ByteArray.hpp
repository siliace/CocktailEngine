#ifndef COCKTAIL_CORE_UTILITY_BasicByteArray_HPP
#define COCKTAIL_CORE_UTILITY_BasicByteArray_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <Cocktail/Core/Memory/UniquePtr.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
    /**
     * \brief Utility class to own and manage a memory area
     */
    template <typename TAllocator>
    class BasicByteArray
    {
    public:

        using SizeType = typename TAllocator::SizeType;
        using ElementAllocator = typename TAllocator::template ForType<Byte>;

        /**
         * \brief Default constructor
         */
        BasicByteArray() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Create a BasicByteArray by allocating and filling its own memory area.
         *
         * \param size The size of the memory area to create
         * \param value The value to fill the memory with
         */
        explicit BasicByteArray(SizeType size, Byte value = 0) :
            mSize(size)
        {
            assert(mSize);

            mData = MakeUniqueWithAllocator<Byte[], ElementAllocator>(mAllocator, mSize);
            Memory::Set(mData.Get(), value, mSize);
        }

        /**
         * \brief Constructor
         *
         * Create a BasicByteArray by copying an existing memory area.
         *
         * \param data A pointer to the memory area to copy
         * \param length The length to copy
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
         * Create a BasicByteArray by copying ownership of the one of another BasicByteArray.
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
         * Create a BasicByteArray by taking ownership of the one of another BasicByteArray.
         *
         * \param other The BasicByteArray to move
         */
        BasicByteArray(BasicByteArray&& other) noexcept :
            mSize(0)
        {
            *this = std::move(other);
        }

        /**
         * \brief Assignment operator
         *
         * Assign this BasicByteArray to a copy of another one.
         *
         * \param other The BasicByteArray to copy
         * 
         * \return This
         */
        BasicByteArray& operator=(const BasicByteArray& other)
        {
            if (this != &other)
            {
                mData.Reset();
                mSize = other.mSize;

                if (mSize)
                {
                    mData = MakeUniqueWithAllocator<Byte[], ElementAllocator>(mAllocator, mSize);
                    Memory::Copy(mData.Get(), other.GetData(), mSize);
                }
            }

            return *this;
        }

        /**
         * \brief Assignment operator
         * Assign this BasicByteArray to a move of another one
         * \param other The BasicByteArray to move
         * \return This
         */
        BasicByteArray& operator=(BasicByteArray&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mData = std::move(other.mData);

            return *this;
        }
        BasicByteArray& Append(const BasicByteArray& other)
        {
            return Insert(mSize, other);
        }

        BasicByteArray& Append(const BasicByteArrayView<SizeType>& other)
        {
            return Insert(mSize, other);
        }

        BasicByteArray& Append(const Byte* data, SizeType length)
        {
            return Insert(mSize, data, length);
        }

        BasicByteArray& Prepend(const BasicByteArray& other)
        {
            return Insert(0, other);
        }

        BasicByteArray& Prepend(const BasicByteArrayView<SizeType>& other)
        {
            return Insert(0, other);
        }

        BasicByteArray& Prepend(const Byte* data, SizeType length)
        {
            return Insert(0, data, length);
        }

        BasicByteArray& Insert(SizeType where, const BasicByteArray& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        BasicByteArray& Insert(SizeType where, const BasicByteArrayView<SizeType>& other)
        {
            return Insert(where, other.GetData(), other.GetSize());
        }

        BasicByteArray& Insert(SizeType where, const Byte* data, SizeType length)
        {
            assert(where <= mSize);
            assert(data != nullptr);
            assert(length > 0);

            SizeType fullSize = mSize + length;
            UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocator>> fullData = MakeUniqueWithAllocator<Byte[], ElementAllocator>(mAllocator, fullSize);

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

        BasicByteArray Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        BasicByteArray Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length < mSize);
            return BasicByteArray(mData.Get() + offset, length);
        }

        BasicByteArray& Remove(SizeType offset)
        {
            return Remove(offset, mSize - offset);
        }

        BasicByteArray& Remove(SizeType offset, SizeType length)
        {
            assert(offset + length < mSize);

            SizeType fullSize = mSize - length;
            UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocator>> fullData = MakeUniqueWithAllocator<Byte[], ElementAllocator>(fullSize);

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

        void Resize(SizeType newSize, Byte value = 0)
        {
            if (newSize == mSize)
                return;

            auto newData = MakeUniqueWithAllocator<Byte[], ElementAllocator>(mAllocator, newSize);
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

        Byte& At(SizeType index)
        {
            assert(index < mSize);
            return mData[index];
        }

        const Byte& At(SizeType index) const
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

        Byte* GetData()
        {
            return mData.Get();
        }

        const Byte* GetData() const
        {
            return mData.Get();
        }

        bool operator==(const BasicByteArray& rhs) const
        {
            return *this == BasicByteArrayView(rhs);
        }

        bool operator==(const BasicByteArrayView<SizeType>& rhs) const
        {
            return BasicByteArrayView(*this) == BasicByteArrayView(rhs);
        }

        bool operator!=(const BasicByteArray& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator!=(const BasicByteArrayView<SizeType>& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize;
        UniquePtr<Byte[], AllocatorAwareDeleter<Byte, ElementAllocator>> mData;
        ElementAllocator mAllocator;
    };

    using ByteArray = BasicByteArray<HeapAllocator>;
    using LargeByteArray = BasicByteArray<LargeHeapAllocator>;
}

#endif // COCKTAIL_CORE_UTILITY_BasicByteArray_HPP
