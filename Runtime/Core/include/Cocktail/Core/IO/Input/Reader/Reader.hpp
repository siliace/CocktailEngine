#ifndef COCKTAIL_CORE_IO_INPUT_READER_READER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_READER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    template <typename>
    class Writer;

    /**
     * \brief Base class to implement to read text from various data sources
     *
     * This abstract class defines a generic interface for reading text data
     * from different sources (files, memory buffers, streams, etc.).
     * The encoding used to interpret the text is defined by the template parameter.
     *
     * \tparam TEncoding The charset encoding used to read the text
     */
    template <typename TEncoding = Encoders::Text>
    class Reader
    {
    public:

        /**
         * \brief Encoding type used by this reader
         */
        using EncodingType = TEncoding;

        /**
         * \brief Character type associated with the encoding
         */
        using CharType = typename EncodingType::CharType;

        /**
         * \brief Size type associated with the encoding
         */
        using SizeType = typename EncodingType::SizeType;

        /**
         * \brief Destructor
         */
        virtual ~Reader() = default;

        /**
         * \brief Read a single character
         *
         * \param c The character read from the source
         *
         * \return True if a character was successfully read, false otherwise (e.g. end of stream)
         */
        virtual bool Read(CharType& c)
        {
            return Read(&c, 1) == 1;
        }

        /**
         * \brief Read multiple characters into a buffer
         *
         * Reads up to \p length characters from the data source and stores them
         * into the buffer pointed to by \p text.
         *
         * \param text Pointer to the destination buffer
         * \param length Maximum number of characters to read
         *
         * \return The number of characters actually read. This value may be less than
         *         \p length if the end of the source is reached.
         */
        virtual SizeType Read(CharType* text, SizeType length) = 0;

        /**
         * \brief Transfer all remaining data to a writer
         *
         * Reads data from the current position until the end of the source
         * and writes it to the given \p writer.
         *
         * \param writer The writer that will receive the data
         *
         * \return Total number of characters transferred
         */
        virtual SizeType TransferTo(Writer<EncodingType>& writer)
        {
            const SizeType TransferBufferSize = 8196;

            SizeType transferred = 0;
            CharType transferBuffer[TransferBufferSize];
            while (!IsEof())
            {
                SizeType size = Read(transferBuffer, TransferBufferSize);
                writer.Write(transferBuffer, size);

                transferred += size;
            }

            return transferred;
        }

        /**
         * \brief Indicates whether the reader supports cursor operations
         *
         * Cursor operations include seeking and telling the current position.
         *
         * \return True if cursor operations are supported, false otherwise
         */
        virtual bool HasCursor() const = 0;

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position The position to seek to, in characters or bytes depending on implementation
         *
         * \return The new cursor position after seeking
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        virtual Uint64 Seek(Uint64 position) = 0;

        /**
         * \brief Reset the read cursor to the beginning of the source
         *
         * Equivalent to calling Seek(0) for implementations that support cursors
         */
        virtual void Rewind() = 0;

        /**
         * \brief Get the current cursor position
         *
         * \return The current position in the source
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        virtual Uint64 Tell() const = 0;

        /**
         * \brief Get the total size of the data source
         *
         * \return Total size in characters
         */
        virtual SizeType GetSize() const = 0;

        /**
         * \brief Check if the end of the data source has been reached
         *
         * \return True if no more data can be read, false otherwise
         */
        virtual bool IsEof() const = 0;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_READER_HPP
