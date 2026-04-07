#ifndef COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP

#include <Cocktail/Core/IO/Input/InputAccumulator.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
    /**
     * \brief Buffered reader that adds an intermediate read buffer to another reader
     *
     * This class wraps an existing Reader and introduces an internal buffer
     * to reduce the number of calls to the underlying data source.
     *
     * It relies on InputAccumulator to manage buffering and extraction of data.
     *
     * \tparam TEncoding The charset encoding used to read the text
     * \tparam TAllocator The allocator used for the internal buffer
     */
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class BufferedReader : public Reader<TEncoding>, InputAccumulator<typename TEncoding::CharType, TAllocator>
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
         * \brief Default buffer size used by the reader
         */
        static constexpr SizeType DefaultBufferSize = 8192;

        /**
         * \brief Constructor
         *
         * \param source The underlying reader to read data from
         * \param bufferSize Size of the internal buffer
         */
        explicit BufferedReader(Reader<TEncoding>& source, SizeType bufferSize = DefaultBufferSize) :
            InputAccumulator<CharType, TAllocator>(bufferSize),
            mSource(&source)
        {
            /// Nothing
        }

        /**
         * \brief Read multiple characters into a buffer
         *
         * Extracts characters from the internal buffer. If necessary,
         * additional data is fetched from the underlying reader.
         *
         * \param text Pointer to the destination buffer
         * \param length Maximum number of characters to read
         *
         * \return The number of characters actually read
         */
        SizeType Read(CharType* text, SizeType length) override
        {
            return InputAccumulator<CharType, TAllocator>::Extract(text, length);
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * Invalidates the internal buffer before delegating to the source reader.
         *
         * \param position The position to seek to
         *
         * \return The new cursor position after seeking
         */
        Uint64 Seek(Uint64 position) override
        {
            InputAccumulator<CharType, TAllocator>::Invalidate();
            return mSource->Seek(position);
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
         * \brief Get the current cursor position
         *
         * \return The current position in the source
         */
        Uint64 Tell() const override
        {
            return mSource->Tell();
        }

        /**
         * \brief Get the total size of the data source
         *
         * \return Total size in characters
         */
        SizeType GetSize() const override
        {
            return mSource->GetSize();
        }

        /**
         * \brief Reset the read cursor to the beginning of the source
         *
         * Invalidates the internal buffer before rewinding the source reader.
         */
        void Rewind() override
        {
            InputAccumulator<CharType, TAllocator>::Invalidate();
            mSource->Rewind();
        }

        /**
         * \brief Check if the end of the data source has been reached
         *
         * The end is reached when the internal buffer is empty and the
         * underlying source reader has reached its end.
         *
         * \return True if no more data can be read, false otherwise
         */
        bool IsEof() const override
        {
            return InputAccumulator<CharType, TAllocator>::IsInvalid() && mSource->IsEof();
        }

    private:

        /**
         * \brief Fetch more data from the underlying reader
         *
         * This function is called by InputAccumulator when more data is required.
         *
         * \param buffer Destination buffer
         * \param bufferSize Maximum number of characters to read
         *
         * \return Number of characters read from the source
         */
        SizeType DoAdvance(CharType* buffer, SizeType bufferSize) override
        {
            return mSource->Read(buffer, bufferSize);
        }

        /** \brief Underlying reader */
        Reader<TEncoding>* mSource;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
