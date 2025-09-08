#ifndef COCKTAIL_CORE_IO_INPUT_READER_TEXTREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_TEXTREADER_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/IO/EncodingMode.hpp>

namespace Ck
{
	/**
     * \brief Base interface for custom input characters streams
	 */
	class Reader
	{
	public:

        /**
         * \brief Destructor
         */
        virtual ~Reader() = default;

		/**
         * \brief Read a single character from the reader
         *
         * \param c The character to read
         *
         * \return True if the character has been read, false if the reader has reached its end
         */
        virtual bool Read(TextChar& c) = 0;

		/**
		 * \brief Read characters from the reader
         * Reading the reader might not totally fill the full \p length of the \p buffer if the reader does not contain enough data.
         * After reading, the reader's reading position must be advanced by the amount of bytes actually read.
         *
		 * \param text A buffer where copy data read from the reader
		 * \param length The number of bytes to read from the reader
         *
         * \return The number of characters actually read
		 */
		virtual std::size_t Read(TextChar* text, std::size_t length) = 0;

		/**
         * \brief Tell whether the reader supports cursor
         * Some implementations of the Reader interface might not support cursors methods (e.g Seek, Rewind and Tell).
         * If these methods are called on a reader that does not support them, an exception is thrown
         *
         * \return True if the stream has a cursor, false otherwise
         */
        virtual bool HasCursor() const = 0;

        /**
         * \brief Change the current reading position
         *
         * \param position The position to seek to from the beginning of the reader
         *
         * \return The position actually sought
         */
        virtual Uint64 Seek(Uint64 position) = 0;

        /**
         * \brief Moves the file position indicator to the beginning of the given file reader
         * The function is equivalent to <pre>Reader::Seek(0);</pre>
         *
         * \see Reader::Seek
         */
        virtual void Rewind() = 0;

        /**
         * \brief Get the current reading position in the reader
         *
         * \return The current position
         */
        virtual Uint64 Tell() const = 0;

        /**
         * \brief Get the size (in bytes) of the reader
         *
         * \return The total amount of bytes available in the reader
         */
        virtual std::size_t GetSize() const = 0;

        /**
         * \brief Tell whether the reader has its end and thus cannot read anything
         *
         * \return True if the reader is at its end, false otherwise
         */
        virtual bool IsEof() const = 0;

		/**
		 * \brief Get the encoding mode used to read text
		 * Depending on the system and the encoding mode used, conversion
		 * between two encoding modes might occur and hurt performances.
		 *
		 * \return The encoding mode
		 */
		virtual EncodingMode GetEncodingMode() const = 0;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_TEXTREADER_HPP
