#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    /**
     * \brief Writer that transcodes text from one encoding to another
     *
     * TranscodingWriter adapts a source Writer interface (\p TSrc) to a destination
     * Writer with a different encoding (\p TDst). Each character from the source
     * encoding is decoded to a Unicode codepoint and then re-encoded using the
     * destination encoding before being written.
     *
     * \tparam TSrc Source encoding of characters to write
     * \tparam TDst Destination encoding to which text is transcoded
     */
    template <typename TSrc, typename TDst>
    class TranscodingWriter : public Writer<TSrc>
    {
    public:

        /**
         * \brief Character type of the source encoding
         */
        using CharType = typename Writer<TSrc>::CharType;

        /**
         * \brief Size type used for source write operations
         */
        using SizeType = typename Writer<TSrc>::SizeType;

        /**
         * \brief Source encoding type
         */
        using EncodingType = typename Writer<TSrc>::EncodingType;

        /**
         * \brief Character type of the destination encoding
         */
        using InnerCharType = typename Writer<TDst>::CharType;

        /**
         * \brief Size type for destination write operations
         */
        using InnerSizeType = typename Writer<TDst>::SizeType;

        /**
         * \brief Destination encoding type
         */
        using InnerEncodingType = typename Writer<TDst>::EncodingType;

        /**
         * \brief Constructor
         *
         * Wraps a destination writer and enables transcoding from source to destination encoding.
         *
         * \param destination Writer that will receive transcoded text
         */
        explicit TranscodingWriter(Writer<TDst>& destination) :
            mDestination(&destination)
        {
            /// Nothing to initialize beyond the pointer
        }

        /**
         * \brief Write text after transcoding to the destination encoding
         *
         * Decodes characters from the source encoding into codepoints, then encodes
         * them using the destination encoding and writes them to the destination writer.
         *
         * \param text Pointer to source characters
         * \param length Number of source characters to write
         *
         * \throws ExceptionUtils::ThrowCodepointDecodingException if a character cannot be decoded
         * \throws ExceptionUtils::ThrowCodepointEncodingException if a codepoint cannot be encoded
         */
        void Write(const CharType* text, SizeType length) override
        {
            for (SizeType i = 0; i < length;)
            {
                Utf32Char codepoint;
                SizeType decoded = EncodingType::Decode(text + i, length - i, codepoint);
                if (decoded == 0)
                    ExceptionUtils::ThrowCodepointDecodingException(i);

                i += decoded;

                InnerCharType encodingBuffer[InnerEncodingType::MaxCodepointEncodingLength];
                InnerSizeType encoded = InnerEncodingType::Encode(codepoint, encodingBuffer);
                if (encoded == 0)
                    ExceptionUtils::ThrowCodepointEncodingException(codepoint);

                mDestination->Write(encodingBuffer, encoded);
            }
        }

        /**
         * \brief Flush the destination writer
         *
         * Ensures all transcoded characters are written to the underlying destination.
         */
        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        Writer<TDst>* mDestination; ///< Destination writer to which transcoded characters are written
    };

    /**
     * \brief Transcoding writer to ASCII encoding
     */
    template <typename TDst>
    using AsciiTranscodingReader = TranscodingWriter<TDst, Encoders::Ascii>;

    /**
     * \brief Transcoding writer to UTF-8 encoding
     */
    template <typename TDst>
    using Utf8TranscodingReader = TranscodingWriter<TDst, Encoders::Utf8>;

    /**
     * \brief Transcoding writer to UTF-16 encoding
     */
    template <typename TDst>
    using Utf16TranscodingReader = TranscodingWriter<TDst, Encoders::Utf16>;

    /**
     * \brief Transcoding writer to UTF-32 encoding
     */
    template <typename TDst>
    using Utf32TranscodingReader = TranscodingWriter<TDst, Encoders::Utf32>;
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSCODINGWRITER_HPP
