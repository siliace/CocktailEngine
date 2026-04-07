#ifndef COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/OutputAccumulator.hpp>
#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>

namespace Ck
{
    /**
     * \brief Output stream that writes bytes with internal buffering
     *
     * BufferedOutputStream wraps an underlying OutputStream and accumulates
     * data in an internal buffer before writing it to the destination.
     * This reduces the number of write operations and can significantly
     * improve performance for small writes.
     *
     * \tparam TAllocator Allocator type used for the internal buffer
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class BufferedOutputStream : public OutputStream<TAllocator>, OutputAccumulator<Byte, TAllocator>
    {
    public:

        /**
         * \brief Size type used for writing operations
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Default internal buffer size
         */
        static constexpr SizeType DefaultBufferSize = 8192;

        /**
         * \brief Constructor
         *
         * Wraps an existing OutputStream with a buffer of the given size.
         *
         * \param destination The underlying output stream
         * \param bufferSize Size of the internal buffer (defaults to 8192 bytes)
         */
        explicit BufferedOutputStream(OutputStream<TAllocator>& destination, SizeType bufferSize = DefaultBufferSize) :
            OutputAccumulator<Byte, TAllocator>(bufferSize),
            mDestination(&destination)
        {
            /// Nothing to initialize
        }

        /**
         * \brief Write bytes to the stream
         *
         * Appends the data to the internal buffer. The buffer is flushed
         * to the underlying stream when it becomes full or when \ref Flush() is called.
         *
         * \param buffer Source buffer
         * \param length Number of bytes to write
         *
         * \return Number of bytes written (always equals \p length)
         */
        SizeType Write(const Byte* buffer, SizeType length) override
        {
            return OutputAccumulator<Byte, TAllocator>::Append(buffer, length);
        }

        /**
         * \brief Flush the internal buffer
         *
         * Writes all accumulated data in the internal buffer to the
         * underlying stream immediately.
         */
        void Flush() override
        {
            OutputAccumulator<Byte, TAllocator>::Commit();
        }

    private:

        /**
         * \brief Commit a buffer to the underlying stream
         *
         * Called by OutputAccumulator when the internal buffer is full
         * or on flush.
         *
         * \param buffer Pointer to the buffer to commit
         * \param bufferSize Number of bytes to write
         */
        void DoCommit(const Byte* buffer, SizeType bufferSize) override
        {
            mDestination->Write(buffer, bufferSize);
        }

        OutputStream<TAllocator>* mDestination; ///< Underlying output stream
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_STREAM_BUFFEREDOUTPUTSTREAM_HPP
