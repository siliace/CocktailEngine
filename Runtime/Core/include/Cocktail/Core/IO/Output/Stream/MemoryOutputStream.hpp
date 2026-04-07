#ifndef COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP
#define COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
    /**
     * \brief Output stream that writes bytes into a memory buffer
     *
     * MemoryOutputStream wraps a BasicByteArray and writes all incoming
     * bytes directly into it. This is useful for collecting data in memory
     * without performing file or network I/O.
     *
     * \tparam TAllocator Allocator type used by the underlying byte array
     */
    template <typename TAllocator = HeapAllocator>
    class MemoryOutputStream : public OutputStream<TAllocator>
    {
    public:

        /**
         * \brief Size type used for write operations
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Constructor
         *
         * Wraps a BasicByteArray to accumulate bytes in memory.
         *
         * \param output Reference to a BasicByteArray where data will be appended
         */
        explicit MemoryOutputStream(BasicByteArray<TAllocator>& output) :
            mOutput(output)
        {
            /// Nothing to initialize
        }

        /**
         * \brief Write bytes into the memory buffer
         *
         * Appends \p length bytes from \p data to the underlying BasicByteArray.
         *
         * \param data Pointer to the source bytes
         * \param length Number of bytes to write
         *
         * \return Number of bytes written (always equals \p length)
         */
        SizeType Write(const Byte* data, SizeType length) override
        {
            mOutput.Append(data, length);
            return length;
        }

        /**
         * \brief Flush the output
         *
         * MemoryOutputStream does not buffer data outside of the BasicByteArray,
         * so Flush has no effect.
         */
        void Flush() override
        {
            /// Nothing to do
        }

    private:

        BasicByteArray<TAllocator>& mOutput; ///< Destination memory buffer
    };
}

#endif // COCTAIL_CORE_IO_OUTPUT_STREAM_MEMORYOUTPUTSTREAM_HPP
