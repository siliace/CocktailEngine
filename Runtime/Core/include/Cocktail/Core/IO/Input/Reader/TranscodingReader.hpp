#ifndef COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
    template <typename TSrc, typename TDst = Encoders::Text>
    class TranscodingReader : public Reader<TDst>
    {
    public:

        using CharType = typename Reader<TDst>::CharType;
        using SizeType = typename Reader<TDst>::SizeType;
        using EncodingType = typename Reader<TDst>::EncodingType;

        using SourceCharType = typename Reader<TSrc>::CharType;
        using SourceSizeType = typename Reader<TSrc>::SizeType;
        using SourceEncodingType = typename Reader<TSrc>::EncodingType;

        explicit TranscodingReader(Reader<TSrc>& source) :
            mSource(&source)
        {
            /// Nothing
        }

        bool Read(CharType& character) override
        {
            if (mInternalDecodeBuffer.Position == mInternalDecodeBuffer.Limit)
            {
                SourceSizeType read = mSource->Read(mInternalDecodeBuffer.Buffer, SourceEncodingType::MaxCodepoinTDstLength);
                if (read == 0)
                    return false;

                mInternalDecodeBuffer.Position = 0;
                mInternalDecodeBuffer.Limit = read;
            }

            Utf32Char codepoint;
            auto available = mInternalDecodeBuffer.Limit - mInternalDecodeBuffer.Position;
            SourceSizeType decoded = SourceEncodingType::Decode(mInternalDecodeBuffer.Buffer + mInternalDecodeBuffer.Position, available, codepoint);
            if (decoded == 0)
            {
                SizeType index = (mSource->HasCursor() ? mSource->Tell() : 0) + mInternalDecodeBuffer.Position;
                ExceptionUtils::ThrowCodepointDecodingException(index);
            }

            mInternalDecodeBuffer.Position += decoded;

            CharType encodeBuffer[EncodingType::MaxCodepoinTDstLength];
            SizeType encoded = EncodingType::Encode(codepoint, encodeBuffer);
            if (encoded == 0)
                ExceptionUtils::ThrowCodepointEncodingException(codepoint);

            character = encodeBuffer[0];

            return true;
        }

        SizeType Read(CharType* characters, SizeType size) override
        {
            assert(characters);

            SizeType total = 0;
            while (total < size)
            {
                if (CharType& c = characters[total]; !Read(c))
                    return total;

                ++total;
            }

            return total;
        }

        bool HasCursor() const override
        {
            return mSource->HasCursor();
        }

        Uint64 Seek(Uint64 position) override
        {
            return mSource->Seek(position * sizeof(CharType));
        }

        void Rewind() override
        {
            mSource->Rewind();
        }

        Uint64 Tell() const override
        {
            return mSource->Tell() * sizeof(SourceCharType) / sizeof(CharType);
        }

        SizeType GetSize() const override
        {
            return mSource->GetSize() * sizeof(SourceCharType) / sizeof(CharType);
        }

        bool IsEof() const override
        {
            return mSource->IsEof();
        }

    private:

        struct InternalDecodeBuffer
        {
            SourceSizeType Limit = 0;
            SourceSizeType Position = 0;
            SourceCharType Buffer[SourceEncodingType::MaxCodepoinTDstLength];
        };

        Reader<TSrc>* mSource;
        InternalDecodeBuffer mInternalDecodeBuffer;
    };

    template <typename TDst>
    using AsciiTranscodingReader = TranscodingReader<Encoders::Ascii, TDst>;

    template <typename TDst>
    using Utf8TranscodingReader = TranscodingReader<Encoders::Utf8, TDst>;

    template <typename TDst>
    using Utf16TranscodingReader = TranscodingReader<Encoders::Utf16, TDst>;

    template <typename TDst>
    using Utf32TranscodingReader = TranscodingReader<Encoders::Utf32, TDst>;
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP
