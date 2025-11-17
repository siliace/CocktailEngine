#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class MemoryInputStream : public InputStream<TAllocator>
    {
    public:

        using SizeType = typename InputStream<TAllocator>::SizeType;

        MemoryInputStream(const Byte* data, SizeType length) :
            mData(data),
            mSize(length),
            mCursor(0)
        {
            /// Nothing
        }

        SizeType Read(Byte* data, SizeType length) override
        {
            SizeType available = mSize - mCursor;
            SizeType copyLength = std::min(available, length);

            std::memcpy(data, mData + mCursor, copyLength);

            mCursor += copyLength;

            return copyLength;
        }

        bool HasCursor() const override
        {
            return true;
        }

        Uint64 Seek(Uint64 position) override
        {
            mCursor = std::min(position, static_cast<Uint64>(mSize));

            return mCursor;
        }

        Uint64 Tell() const override
        {
            return mCursor;
        }

        void Rewind() override
        {
            mCursor = 0;
        }

        SizeType GetSize() const override
        {
            return mSize;
        }

        bool IsEof() const override
        {
            return mCursor == mSize;
        }

    private:

        const Byte* mData;
        SizeType mSize;
        Uint64 mCursor;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
