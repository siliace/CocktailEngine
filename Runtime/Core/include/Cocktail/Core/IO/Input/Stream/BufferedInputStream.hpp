#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/InputAccumulator.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
    /**
     * \brief Input stream that reads bytes with internal buffering
     *
     * BufferedInputStream wraps an underlying InputStream and accumulates
     * data in an internal buffer to reduce the number of read operations.
     * This is useful for improving performance when reading small amounts
     * of data repeatedly.
     *
     * \tparam TAllocator Allocator type used for the internal buffer
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class BufferedInputStream : public InputStream<TAllocator>, InputAccumulator<Byte, TAllocator>
    {
    public:

        /**
         * \brief Size type used for reading operations
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Default internal buffer size
         */
        static constexpr SizeType DefaultBufferSize = 8192;

        /**
         * \brief Constructor
         *
         * Wraps an existing InputStream with a buffer of the given size.
         *
         * \param source The underlying input stream
         * \param bufferSize Size of the internal buffer (defaults to 8192 bytes)
         */
        explicit BufferedInputStream(InputStream<TAllocator>& source, SizeType bufferSize = DefaultBufferSize) :
            InputAccumulator<Byte, TAllocator>(bufferSize),
            mSource(&source)
        {
            /// Nothing to initialize
        }

        /**
         * \brief Read bytes into a buffer
         *
         * Reads up to \p length bytes, using the internal buffer to minimize
         * direct reads from the underlying stream.
         *
         * \param buffer Destination buffer
         * \param length Maximum number of bytes to read
         *
         * \return Number of bytes actually read
         */
        SizeType Read(Byte* buffer, SizeType length) override
        {
            return InputAccumulator<Byte, TAllocator>::Extract(buffer, length);
        }

        /**
         * \brief Check if the stream supports cursor operations
         *
         * \return True if the underlying stream supports cursors
         */
        bool HasCursor() const override
        {
            return mSource->HasCursor();
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * Invalidates the internal buffer before delegating the seek
         * to the underlying stream.
         *
         * \param position New cursor position in bytes
         *
         * \return The new cursor position
         */
        Uint64 Seek(Uint64 position) override
        {
            InputAccumulator<Byte, TAllocator>::Invalidate();
            return mSource->Seek(position);
        }

        /**
         * \brief Reset the read cursor to the beginning
         *
         * Invalidates the internal buffer and rewinds the underlying stream.
         */
        void Rewind() override
        {
            InputAccumulator<Byte, TAllocator>::Invalidate();
            mSource->Rewind();
        }

        /**
         * \brief Get the current cursor position
         *
         * Combines the position of the underlying stream and the
         * current position in the internal buffer.
         *
         * \return Current position in bytes
         */
        Uint64 Tell() const override
        {
            return mSource->Tell() + InputAccumulator<Byte, TAllocator>::GetBufferPosition();
        }

        /**
         * \brief Get the total size of the underlying stream
         *
         * \return Size in bytes
         */
        SizeType GetSize() const override
        {
            return mSource->GetSize();
        }

        /**
         * \brief Check if the end of the stream has been reached
         *
         * \return True if the internal buffer is empty and the underlying stream is at EOF
         */
        bool IsEof() const override
        {
            return InputAccumulator<Byte, TAllocator>::IsInvalid() && mSource->IsEof();
        }

    private:

        /**
         * \brief Fill the internal buffer with data from the underlying stream
         *
         * \param buffer Destination buffer
         * \param bufferSize Maximum number of bytes to fill
         *
         * \return Number of bytes read from the underlying stream
         */
        SizeType DoAdvance(Uint8* buffer, SizeType bufferSize) override
        {
            return mSource->Read(buffer, bufferSize);
        }

        InputStream<TAllocator>* mSource; ///< Underlying input stream
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_BUFFEREDINPUTSTREAM_HPP
