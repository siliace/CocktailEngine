#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    template <typename TSrc, typename TDst>
    class TranscodingWriter : public Writer<TSrc>
    {
    public:

        using CharType = typename Writer<TSrc>::CharType;
        using SizeType = typename Writer<TSrc>::SizeType;
        using EncodingType = typename Writer<TSrc>::EncodingType;

        using InnerCharType = typename Writer<TDst>::CharType;
        using InnerSizeType = typename Writer<TDst>::SizeType;
        using InnerEncodingType = typename Writer<TDst>::EncodingType;

        explicit TranscodingWriter(Writer<TDst>& destination) :
            mDestination(&destination)
        {
            /// Nothing
        }

        void Write(const CharType* text, SizeType length) override
        {
            for (SizeType i = 0; i < length;)
            {
                Utf32Char codepoint;
                SizeType decoded = EncodingType::Decode(text + i, length - i, codepoint);
                if (decoded == 0)
                    ExceptionUtils::ThrowCodepointDecodingException(i);

                i += decoded;

                InnerCharType encodingBuffer[InnerEncodingType::MaxCodepoinTSrcLength];
                InnerSizeType encoded = InnerEncodingType::Encode(codepoint, encodingBuffer);
                if (encoded == 0)
                    ExceptionUtils::ThrowCodepoinTSrcException(codepoint);

                mDestination->Write(encodingBuffer, encoded);
            }
        }

        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        Writer<TDst>* mDestination;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP
