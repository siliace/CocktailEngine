#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
    /**
     * \brief Input stream that reads data from a chunk of memory
     */
    class COCKTAIL_CORE_API MemoryInputStream : public InputStream
    {
    public:
        
        /**
         * \brief Constructor
         * \param data Pointer to the data in memory
         * \param length Size of the data, in bytes
         */
        MemoryInputStream(const void* data, std::size_t length);

        /**
         * \brief 
         * \return 
         */
        bool HasCursor() const override;

        /**
         * \brief Read data from the stream
         * Reading the stream might not totally fill the full \p length of the \p buffer if the stream does not contains enough data.
         * After reading, the stream's reading position must be advanced by the amount of bytes actually read.
         * \param data Buffer where copy data read from the stream
         * \param length The number of bytes to read from the stream
         * \return The number of bytes actually read
         */
        std::size_t Read(void* data, std::size_t length) override;
        
        /**
         * \brief Change the current reading position
         * \param position The position to seek to from the beginning of the stream
         * \return The position actually sought
         */
        Uint64 Seek(Uint64 position) override;

        /**
         * \brief Get the current reading position in the stream
         * \return The current position
         */
        Uint64 Tell() const override;
		
		/**
		 * \brief Moves the file position indicator to the beginning of the given file stream
		 * The function is equivalent to <pre>InputStream::Seek(0);</pre>
		 * \see InputStream::Seek
		 */
        void Rewind() override;
    	
        /**
         * \brief Get the size (in bytes) of the stream
         * \return The total amount of bytes available in the stream
         */
        std::size_t GetSize() const override;

        /**
         * \brief 
         * \return 
         */
        bool IsEof() const override;

    private:

        const unsigned char* mData;
        std::size_t mSize;
        Uint64 mCursor;
    };
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_MEMORYINPUTSTREAM_HPP
