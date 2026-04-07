#ifndef COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
    /**
     * \brief Reader that transcodes characters from one encoding to another
     *
     * This class adapts a Reader with a source encoding to produce characters
     * in a different destination encoding.
     *
     * It reads characters from the underlying source, decodes them into Unicode
     * codepoints, and re-encodes them into the destination encoding.
     *
     * \tparam TSrc Source encoding type
     * \tparam TDst Destination encoding type
     */
    template <typename TSrc, typename TDst = Encoders::Text>
    class TranscodingReader : public Reader<TDst>
    {
    public:

        /**
         * \brief Character type associated with the destination encoding
         */
        using CharType = typename Reader<TDst>::CharType;

        /**
         * \brief Size type associated with the destination encoding
         */
        using SizeType = typename Reader<TDst>::SizeType;

        /**
         * \brief Encoding type used by this reader
         */
        using EncodingType = typename Reader<TDst>::EncodingType;

        /**
         * \brief Character type associated with the source encoding
         */
        using SourceCharType = typename Reader<TSrc>::CharType;

        /**
         * \brief Size type associated with the source encoding
         */
        using SourceSizeType = typename Reader<TSrc>::SizeType;

        /**
         * \brief Encoding type of the source
         */
        using SourceEncodingType = typename Reader<TSrc>::EncodingType;

        /**
         * \brief Constructor
         *
         * \param source The source reader providing input data
         */
        explicit TranscodingReader(Reader<TSrc>& source) :
            mSource(&source)
        {
            /// Nothing
        }

        /**
         * \brief Read a single character
         *
         * Reads data from the source, decodes it into a Unicode codepoint,
         * then encodes it into the destination encoding.
         *
         * \param character The character read from the source
         *
         * \return True if a character was successfully read, false otherwise
         *
         * \throw Throws if decoding or encoding fails
         */
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

        /**
         * \brief Read multiple characters into a buffer
         *
         * \param characters Pointer to the destination buffer
         * \param size Maximum number of characters to read
         *
         * \return The number of characters actually read
         */
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

        /**
         * \brief Indicates whether the reader supports cursor operations
         *
         * \return True if cursor operations are supported, false otherwise
         */
        bool HasCursor() const override
        {
            return mSource->HasCursor();
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position The position to seek to, in destination characters
         *
         * \return The new cursor position after seeking
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        Uint64 Seek(Uint64 position) override
        {
            return mSource->Seek(position * sizeof(CharType));
        }

        /**
         * \brief Reset the read cursor to the beginning of the source
         */
        void Rewind() override
        {
            mSource->Rewind();
        }

        /**
         * \brief Get the current cursor position
         *
         * \return The current position in the source, in destination characters
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        Uint64 Tell() const override
        {
            return mSource->Tell() * sizeof(SourceCharType) / sizeof(CharType);
        }

        /**
         * \brief Get the total size of the data source
         *
         * \return Total size in destination characters
         */
        SizeType GetSize() const override
        {
            return mSource->GetSize() * sizeof(SourceCharType) / sizeof(CharType);
        }

        /**
         * \brief Check if the end of the data source has been reached
         *
         * \return True if no more data can be read, false otherwise
         */
        bool IsEof() const override
        {
            return mSource->IsEof();
        }

    private:

        /**
         * \brief Internal buffer used for decoding source characters
         */
        struct InternalDecodeBuffer
        {
            SourceSizeType Limit = 0;
            SourceSizeType Position = 0;
            SourceCharType Buffer[SourceEncodingType::MaxCodepoinTDstLength];
        };

        /** \brief Source reader */
        Reader<TSrc>* mSource;

        /** \brief Internal decode buffer */
        InternalDecodeBuffer mInternalDecodeBuffer;
    };

    /**
     * \brief Transcoding reader from ASCII encoding
     */
    template <typename TDst>
    using AsciiTranscodingReader = TranscodingReader<Encoders::Ascii, TDst>;

    /**
     * \brief Transcoding reader from UTF-8 encoding
     */
    template <typename TDst>
    using Utf8TranscodingReader = TranscodingReader<Encoders::Utf8, TDst>;

    /**
     * \brief Transcoding reader from UTF-16 encoding
     */
    template <typename TDst>
    using Utf16TranscodingReader = TranscodingReader<Encoders::Utf16, TDst>;

    /**
     * \brief Transcoding reader from UTF-32 encoding
     */
    template <typename TDst>
    using Utf32TranscodingReader = TranscodingReader<Encoders::Utf32, TDst>;
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_TRANSCODINGREADER_HPP
