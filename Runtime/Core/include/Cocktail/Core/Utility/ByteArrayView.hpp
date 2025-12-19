#ifndef COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
#define COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    template <typename>
    class ByteArray;

    /**
     * \brief Utility class to access to a memory area
     */
    template <typename TSizeType = SizeTypeOf<64>>
    class COCKTAIL_CORE_API ByteArrayView
    {
    public:

        using SizeType = TSizeType;

        ByteArrayView() :
            mSize(0),
            mData(nullptr)
        {
            /// Nothing
        }

        ByteArrayView(const Byte* data, SizeType size) :
            mSize(size),
            mData(data)
        {
            assert(mData != nullptr);
        }

        template <typename TAllocator>
        ByteArrayView(const ByteArray<TAllocator>& byteArray, SizeType offset) :
            ByteArrayView(byteArray, offset, byteArray.GetSize() - offset)
        {
            /// Nothing
        }

        template <typename TAllocator>
        ByteArrayView(const ByteArray<TAllocator>& byteArray, SizeType offset, SizeType length)
        {
            assert(offset + length <= byteArray.GetSize());

            mSize = length;
            mData = byteArray.GetData() + offset;
        }

        ByteArrayView(ByteArrayView&& other) noexcept
        {
            *this = std::move(other);
        }

        ByteArrayView& operator=(ByteArrayView&& other) noexcept
        {
            mSize = std::exchange(other.mSize, 0);
            mData = std::exchange(other.mData, nullptr);

            return *this;
        }

        ByteArrayView Slice(SizeType offset) const
        {
            return Slice(offset, mSize - offset);
        }

        ByteArrayView Slice(SizeType offset, SizeType length) const
        {
            assert(offset + length < mSize);

            ByteArrayView view;
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
        bool operator==(const ByteArray<TAllocator>& rhs) const
        {
            return *this == ByteArrayView(rhs);
        }

        bool operator==(const ByteArrayView& rhs) const
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
        bool operator!=(const ByteArray<TAllocator>& rhs) const
        {
            return !(*this == rhs);
        }

        bool operator!=(const ByteArrayView& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        SizeType mSize;
        const Byte* mData;
    };
}

#endif // COCKTAIL_CORE_UTILITY_BYTEARRAYVIEW_HPP
