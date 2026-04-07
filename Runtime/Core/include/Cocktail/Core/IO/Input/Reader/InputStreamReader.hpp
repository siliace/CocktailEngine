#ifndef COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
    /**
     * \brief Adapter that reads encoded characters from an input byte stream
     *
     * This class adapts an InputStream to the Reader interface by interpreting
     * raw bytes as characters using the specified encoding.
     *
     * It does not perform any encoding conversion but rather reinterprets the
     * underlying byte stream as a sequence of characters of type CharType.
     *
     * \tparam TEncoding The charset encoding used to interpret the bytes
     * \tparam TAllocator The allocator used by the underlying input stream
     */
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class InputStreamReader : public Reader<TEncoding>
    {
    public:

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename Reader<TEncoding>::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename Reader<TEncoding>::SizeType;

        /**
         * \brief Encoding type used by this reader
         */
        using EncodingType = typename Reader<TEncoding>::EncodingType;

        /**
         * \brief Constructor
         *
         * \param inputStream The underlying input stream to read from
         */
        explicit InputStreamReader(InputStream<TAllocator>& inputStream) :
            mInner(&inputStream)
        {
            /// Nothing
        }

        /**
         * \brief Read a single character
         *
         * \param c The character read from the source
         *
         * \return True if a character was successfully read, false otherwise
         */
        bool Read(CharType& c) override
        {
            return Reader<TEncoding>::Read(c);
        }

        /**
         * \brief Read multiple characters into a buffer
         *
         * Reads raw bytes from the underlying stream and reinterprets them
         * as characters of type CharType.
         *
         * \param buffer Pointer to the destination buffer
         * \param length Maximum number of characters to read
         *
         * \return The number of characters actually read
         *
         * \note The number of bytes read from the underlying stream is
         *       length * sizeof(CharType)
         */
        SizeType Read(CharType* buffer, SizeType length) override
        {
            return mInner->Read(reinterpret_cast<Byte*>(buffer), length * sizeof(CharType)) / sizeof(CharType);
        }

        /**
         * \brief Transfer all remaining data to a writer
         *
         * \param writer The writer that will receive the data
         *
         * \return Total number of characters transferred
         */
        SizeType TransferTo(Writer<EncodingType>& writer) override
        {
            return Reader<TEncoding>::TransferTo(writer);
        }

        /**
         * \brief Indicates whether the reader supports cursor operations
         *
         * \return True if cursor operations are supported, false otherwise
         */
        bool HasCursor() const override
        {
            return mInner->HasCursor();
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position The position to seek to, in characters
         *
         * \return The new cursor position after seeking
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        Uint64 Seek(Uint64 position) override
        {
            return mInner->Seek(position / sizeof(CharType));
        }

        /**
         * \brief Reset the read cursor to the beginning of the source
         */
        void Rewind() override
        {
            mInner->Rewind();
        }

        /**
         * \brief Get the current cursor position
         *
         * \return The current position in the source, in characters
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        Uint64 Tell() const override
        {
            return mInner->Tell();
        }

        /**
         * \brief Get the total size of the data source
         *
         * \return Total size in characters
         */
        SizeType GetSize() const override
        {
            return mInner->GetSize() / sizeof(CharType);
        }

        /**
         * \brief Check if the end of the data source has been reached
         *
         * \return True if no more data can be read, false otherwise
         */
        bool IsEof() const override
        {
            return mInner->IsEof();
        }

    private:

        /** \brief Underlying input stream */
        InputStream<TAllocator>* mInner;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
