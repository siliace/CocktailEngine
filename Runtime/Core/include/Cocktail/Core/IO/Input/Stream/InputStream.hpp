#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief Base interface for custom input bytes streams
     */
    class InputStream
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~InputStream() = default;

        /**
         * \brief Read data from the stream
         * Reading the stream might not totally fill the full \p length of the \p buffer if the stream does not contains enough data.
         * After reading, the stream's reading position must be advanced by the amount of bytes actually read.
         * \param buffer A buffer where copy data read from the stream
         * \param length The number of bytes to read from the stream
         * \return The number of bytes actually read
         */
        virtual std::size_t Read(void* buffer, std::size_t length) = 0;

        /**
         * \brief Tell whether the stream supports cursor.
         * Some implementations of the InputStream interface might not support cursors methods (e.g Seek, Rewind and Tell).
         * If these methods are called on a stream that does not support them, an exception is thrown
         * \return True if the stream has a cursor, false otherwise
         */
        virtual bool HasCursor() const = 0;
        
        /**
         * \brief Change the current reading position
         * \param position The position to seek to from the beginning of the stream
         * \return The position actually sought
         */
        virtual Uint64 Seek(Uint64 position) = 0;

        /**
		 * \brief Moves the file position indicator to the beginning of the given file stream
		 * The function is equivalent to <pre>InputStream::Seek(0);</pre>
      	 * \see InputStream::Seek
		 */
		virtual void Rewind() = 0;

        /**
         * \brief Get the current reading position in the stream
         * \return The current position
         */
        virtual Uint64 Tell() const = 0;

        /**
         * \brief Get the size (in bytes) of the stream
         * \return The total amount of bytes available in the stream
         */
        virtual std::size_t GetSize() const = 0;

        /**
         * \brief Tell whether the stream has its end and thus cannot read anything
         * \return True if the stream is at its end, false otherwise
         */
        virtual bool IsEof() const = 0;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP