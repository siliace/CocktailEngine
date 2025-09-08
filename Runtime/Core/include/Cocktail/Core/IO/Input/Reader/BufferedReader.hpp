#ifndef COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP

#include <memory>

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
	/**
	 * \brief Buffered reader that optimizes character and block reading operations.
	 *
	 * This class wraps an existing Reader and adds an internal buffer to reduce
	 * the number of direct accesses to the underlying data source (file, stream, etc.).
	 * It supports reading single characters, blocks of data, entire lines, or the remaining content.
	 */
	class COCKTAIL_CORE_API BufferedReader : public Reader
	{
	public:

		/**
		 * \brief Default internal buffer size (8 KB).
		 */
		static constexpr std::size_t DefaultBufferSize = 8192;

		/**
		 * \brief Constructs a BufferedReader using an existing Reader
		 *
		 * \param reader The underlying data source to read from
		 * \param bufferSize Size of the internal buffer used for reading
		 */
		explicit BufferedReader(Reader& reader, std::size_t bufferSize = DefaultBufferSize);

		/**
		 * \brief Reads a single character from the source
		 *
		 * \param c Reference to store the character read
		 *
		 * \return True if a character was successfully read, false if no more data is available
		 */
		bool Read(TextChar& c) override;

		/**
		 * \brief Reads a block of characters into the provided buffer from the source
		 *
		 * \param text Pointer to the destination buffer
		 * \param length Maximum number of characters to read
		 *
		 * \return The number of characters actually read
		 */
		std::size_t Read(TextChar* text, std::size_t length) override;

		/**
		 * \brief Reads a full line terminated by '\n' or '\r\n'
		 *
		 * \return The line read, without the line ending. Returns an empty string if EOF is reached
		 */
		String ReadLine();

		/**
		 * \brief Reads all remaining content available in the source
		 *
		 * \return A string containing all the remaining data
		 */
		String ReadAll();

		/**
		 * \brief Indicates whether the underlying stream supports a cursor/seek operations
		 *
		 * \return True if the reader has a seekable cursor, false otherwise
		 */
		bool HasCursor() const override;

		/**
		 * \brief Moves the cursor to a specific position in the stream
		 *
		 * \param position Absolute position to move to
		 *
		 * \return The new cursor position
		 */
		Uint64 Seek(Uint64 position) override;

		/**
		 * \brief Resets the cursor to the beginning of the stream
		 */
		void Rewind() override;

		/**
		 * \brief Returns the current cursor position in the stream
		 * \return The current position
		 */
		Uint64 Tell() const override;

		/**
		 * \brief Returns the total size of the underlying stream, if available
		 *
		 * \return The size in characters or 0 if the size cannot be determined
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief Checks whether the end of the stream has been reached
		 *
		 * \return True if no more data is available, false otherwise
		 */
		bool IsEof() const override;

		/**
		 * \brief Get the encoding mode used to read text
		 * The encoding mode will always be the one used by the underlying source.
		 *
		 * \return The encoding mode
		 */
		EncodingMode GetEncodingMode() const override;

	private:

		/**
		 * \brief Fills the internal buffer from the underlying reader
		 *
		 * \return The number of characters loaded into the buffer
		 */
		std::size_t Advance();

		/**
		 * \brief Searches for a line ending in the buffer up to a specified limit
		 *
		 * \param limit Maximum number of characters to check
		 *
		 * \return The index of the line end if found, otherwise an empty Optional
		 */
		Optional<std::size_t> FindLineEnd(std::size_t limit) const;

		Reader& mIn; /*!< The source Reader to read from */
		std::size_t mPos; /*!< The index of current read position. Must be <= mLimit */
		std::size_t mLimit;
		std::size_t mBufferSize;
		String mLineBuffer;
		std::unique_ptr<TextChar[]> mBuffer;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
