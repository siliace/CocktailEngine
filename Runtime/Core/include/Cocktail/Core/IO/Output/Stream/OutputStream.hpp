#ifndef COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    /**
     * \brief Abstract base class for writing raw bytes to a destination
     *
     * OutputStream defines a generic interface for writing raw byte data
     * to different outputs, such as files, memory buffers, or network streams.
     * It provides low-level write operations and a flush mechanism.
     *
     * \tparam TAllocator Allocator type used by implementations if needed
     */
    template <typename TAllocator = HeapAllocator>
    class OutputStream
    {
    public:

        /**
         * \brief Size type used for write operations
         */
        using SizeType = typename TAllocator::SizeType;

        /**
         * \brief Destructor
         *
         * Virtual destructor to allow proper cleanup of derived classes.
         */
        virtual ~OutputStream() = default;

        /**
         * \brief Write bytes to the output
         *
         * Writes up to \p length bytes from the buffer \p data to the
         * underlying destination.
         *
         * \param data Pointer to the source bytes
         * \param length Number of bytes to write
         *
         * \return Number of bytes actually written. Derived classes may write
         *         fewer bytes than requested.
         */
        virtual SizeType Write(const Byte* data, SizeType length) = 0;

        /**
         * \brief Flush the output
         *
         * Ensures that any buffered or pending data is written to the
         * underlying destination. After this call, no data should remain
         * pending in the stream.
         */
        virtual void Flush() = 0;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUTSTREAM_HPP
