#ifndef COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP

#include <Cocktail/Core/IO/Input/InputAccumulator.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class BufferedReader : public Reader<TEncoding>, InputAccumulator<typename TEncoding::CharType, TAllocator>
    {
    public:

        using CharType = typename Reader<TEncoding>::CharType;

        using SizeType = typename Reader<TEncoding>::SizeType;

        using EncodingType = typename Reader<TEncoding>::EncodingType;

        static constexpr SizeType DefaultBufferSize = 8192;

        explicit BufferedReader(Reader<TEncoding>& source, SizeType bufferSize = DefaultBufferSize) :
            InputAccumulator<CharType, TAllocator>(bufferSize),
            mSource(&source)
        {
            /// Nothing
        }

        SizeType Read(CharType* text, SizeType length) override
        {
            return InputAccumulator<CharType, TAllocator>::Extract(text, length);
        }

        Uint64 Seek(Uint64 position) override
        {
            InputAccumulator<CharType, TAllocator>::Invalidate();
            return mSource->Seek(position);
        }

        bool HasCursor() const override
        {
            return mSource->HasCursor();
        }

        Uint64 Tell() const override
        {
            return mSource->Tell();
        }

        SizeType GetSize() const override
        {
            return mSource->GetSize();
        }

        void Rewind() override
        {
            InputAccumulator<CharType, TAllocator>::Invalidate();
            mSource->Rewind();
        }

        bool IsEof() const override
        {
            return InputAccumulator<CharType, TAllocator>::IsInvalid() && mSource->IsEof();
        }

    private:

        SizeType DoAdvance(CharType* buffer, SizeType bufferSize) override
        {
            return mSource->Read(buffer, bufferSize);
        }

        Reader<TEncoding>* mSource;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
