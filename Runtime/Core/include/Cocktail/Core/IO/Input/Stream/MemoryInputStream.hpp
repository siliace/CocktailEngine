#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>

namespace Ck
{
    /**
     * \brief Input stream that reads bytes from a memory buffer
     *
     * MemoryInputStream allows reading raw bytes from a fixed memory region.
     * It provides full cursor support, including seeking, telling, and rewinding.
     *
     * \tparam TAllocator Allocator type (not used for the memory itself, only for compatibility)
     */
    template <typename TAllocator = SizedHeapAllocator<32>>
    class MemoryInputStream : public InputStream<TAllocator>
    {
    public:

        /**
         * \brief Size type used for reading operations
         */
        using SizeType = typename InputStream<TAllocator>::SizeType;

        /**
         * \brief Constructor
         *
         * Wraps an existing memory buffer as a read-only input stream.
         *
         * \param data Pointer to the memory buffer
         * \param length Number of bytes in the buffer
         */
        MemoryInputStream(const Byte* data, SizeType length) :
            mData(data),
            mSize(length),
            mCursor(0)
        {
            /// Nothing to initialize
        }

        /**
         * \brief Read bytes from the memory buffer
         *
         * Copies up to \p length bytes from the current cursor position
         * into \p data.
         *
         * \param data Destination buffer
         * \param length Maximum number of bytes to read
         *
         * \return Number of bytes actually read
         */
        SizeType Read(Byte* data, SizeType length) override
        {
            SizeType available = mSize - mCursor;
            SizeType copyLength = std::min(available, length);

            Memory::Copy(data, mData + mCursor, copyLength);

            mCursor += copyLength;

            return copyLength;
        }

        /**
         * \brief Check if the stream supports cursor operations
         *
         * \return Always true for MemoryInputStream
         */
        bool HasCursor() const override
        {
            return true;
        }

        /**
         * \brief Move the read cursor to a specific position
         *
         * \param position New position in the buffer
         *
         * \return The new cursor position
         */
        Uint64 Seek(Uint64 position) override
        {
            mCursor = std::min(position, static_cast<Uint64>(mSize));
            return mCursor;
        }

        /**
         * \brief Get the current cursor position
         *
         * \return Cursor position in bytes
         */
        Uint64 Tell() const override
        {
            return mCursor;
        }

        /**
         * \brief Reset the read cursor to the beginning
         */
        void Rewind() override
        {
            mCursor = 0;
        }

        /**
         * \brief Get the total size of the memory buffer
         *
         * \return Number of bytes in the buffer
         */
        SizeType GetSize() const override
        {
            return mSize;
        }

        /**
         * \brief Check if the end of the memory buffer has been reached
         *
         * \return True if the cursor is at the end of the buffer
         */
        bool IsEof() const override
        {
            return mCursor == mSize;
        }

    private:

        const Byte* mData; ///< Pointer to the memory buffer
        SizeType mSize; ///< Total size of the buffer in bytes
        Uint64 mCursor; ///< Current read position in the buffer
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
