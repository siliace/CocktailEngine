#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>

namespace Ck
{
    /**
     * \brief Base class to implement to read binary data from various data sources
     *
     * This abstract class defines a generic interface for reading raw byte data
     * from different sources (files, memory buffers, streams, etc.).
     *
     * Unlike Reader, this class operates on raw bytes instead of encoded characters.
     *
     * \tparam TAllocator The allocator used to define the size type
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class InputStream
    {
    public:

        /**
         * \brief Size type associated with the allocator
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Destructor
         */
        virtual ~InputStream() = default;

        /**
         * \brief Read bytes into a buffer
         *
         * Reads up to \p length bytes from the data source and stores them
         * into the buffer pointed to by \p buffer.
         *
         * \param buffer Pointer to the destination buffer
         * \param length Maximum number of bytes to read
         *
         * \return The number of bytes actually read. This value may be less than
         *         \p length if the end of the source is reached.
         */
        virtual SizeType Read(Byte* buffer, SizeType length) = 0;

        /**
         * \brief Transfer all remaining data to an output stream
         *
         * Reads data from the current position until the end of the source
         * and writes it to the given \p outputStream.
         *
         * \param outputStream The output stream that will receive the data
         *
         * \return Total number of bytes transferred
         *
         * \note Writes to the output stream may be partial, and are handled accordingly.
         */
        virtual SizeType TransferTo(OutputStream<TAllocator>& outputStream)
        {
            const SizeType TransferBufferSize = 8196;

            SizeType transferred = 0;
            Byte transferBuffer[TransferBufferSize];
            while (!IsEof())
            {
                SizeType size = Read(transferBuffer, TransferBufferSize);

                /// Writes in output streams might be incomplete
                for (SizeType i = 0; i < size;)
                    i += outputStream.Write(transferBuffer + i, size - i);

                transferred += size;
            }

            return transferred;
        }

        /**
         * \brief Indicates whether the stream supports cursor operations
         *
         * Cursor operations include seeking and telling the current position.
         *
         * \return True if cursor operations are supported, false otherwise
         */
        virtual bool HasCursor() const = 0;

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position The position to seek to, in bytes
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
         * \return The current position in the source, in bytes
         *
         * \warning Behavior is undefined if HasCursor() returns false
         */
        virtual Uint64 Tell() const = 0;

        /**
         * \brief Get the total size of the data source
         *
         * \return Total size in bytes
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

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_INPUTSTREAM_HPP
