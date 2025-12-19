#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP

#include <Cocktail/Core/Export.hpp>

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

        BasicByteArrayView() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        BasicByteArrayView(const Byte* data, SizeType size) :
            mSize(size),
            mData(data)
        {
            assert(mData != nullptr);
        }

        template <typename TAllocator>
        BasicByteArrayView(const BasicByteArray<TAllocator>& byteArray, SizeType offset = 0) :
            BasicByteArrayView(byteArray, offset, byteArray.GetSize() - offset)
        {
            /// Nothing
        }

        template <typename TAllocator>
        BasicByteArrayView(const BasicByteArray<TAllocator>& byteArray, SizeType offset, SizeType length)
        {
            assert(offset + length <= byteArray.GetSize());

            mSize = length;
            mData = byteArray.GetData() + offset;
        }

        BasicByteArrayView(const BasicByteArrayView& other) :
            mSize(other.mSize),
            mData(other.mData)
        {
            /// Nothing
        }

        BasicByteArrayView(BasicByteArrayView&& other) noexcept :
            mSize(0),
            mData(nullptr)
        {
            *this = std::move(other);
        }

        BasicByteArrayView& operator=(const BasicByteArrayView& other)
        {
            if (this != &other)
            {
                mSize = other.mSize;
                mData = other.mData;
            }

            return *this;
        }

        BasicByteArrayView& operator=(BasicByteArrayView&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mData = std::exchange(other.mData, nullptr);

            return *this;
        }

        BasicByteArrayView Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        BasicByteArrayView Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length < mSize);

            BasicByteArrayView view;
            view.mSize = length;
            view.mData = mData + offset;

            return view;
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

        const Byte* GetData() const
        {
            return mData;
        }

        template <typename TAllocator>
        bool operator==(const BasicByteArray<TAllocator>& rhs) const
        {
            return *this == ByteArrayView(rhs);
        }

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

        template <typename TAllocator>
        bool operator!=(const BasicByteArray<TAllocator>& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator!=(const BasicByteArrayView& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize;
        const Byte* mData;
    };

    using ByteArrayView = BasicByteArrayView<HeapAllocator::SizeType>;
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
