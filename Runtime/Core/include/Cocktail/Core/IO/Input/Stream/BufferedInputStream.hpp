#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/InputAccumulator.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
    template <typename TAllocator = SizedHeapAllocator<32>>
    class BufferedInputStream : public InputStream<TAllocator>, InputAccumulator<Byte, TAllocator>
    {
    public:

        using SizeType = typename TAllocator::SizeType;

        static constexpr SizeType DefaultBufferSize = 8192;

        explicit BufferedInputStream(InputStream<TAllocator>& source, SizeType bufferSize = DefaultBufferSize) :
            InputAccumulator<Byte, TAllocator>(bufferSize),
            mSource(&source)
        {
            /// Nothing
        }

        SizeType Read(Byte* buffer, SizeType length) override
        {
            return InputAccumulator<Byte, TAllocator>::Extract(buffer, length);
        }

        bool HasCursor() const override
        {
            return mSource->HasCursor();
        }

        Uint64 Seek(Uint64 position) override
        {
            InputAccumulator<Byte, TAllocator>::Invalidate();
            return mSource->Seek(position);
        }

        void Rewind() override
        {
            InputAccumulator<Byte, TAllocator>::Invalidate();
            mSource->Rewind();
        }

        Uint64 Tell() const override
        {
            return mSource->Tell() + InputAccumulator<Byte, TAllocator>::GetBufferPosition();
        }

        SizeType GetSize() const override
        {
            return mSource->GetSize();
        }

        bool IsEof() const override
        {
            return InputAccumulator<Byte, TAllocator>::IsInvalid() && mSource->IsEof();
        }

    private:

        SizeType DoAdvance(Uint8* buffer, SizeType bufferSize) override
        {
            return mSource->Read(buffer, bufferSize);
        }

        InputStream<TAllocator>* mSource;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP
