#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP

#include <Cocktail/Core/IO/Output/OutputAccumulator.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    /**
     * \brief Buffered writer that writes characters to an underlying Writer
     *
     * BufferedWriter provides a buffer for character data before sending it
     * to the underlying Writer. This improves performance by reducing the
     * number of write operations for small data chunks.
     *
     * \tparam TEncoding Character encoding used by the writer
     * \tparam TAllocator Allocator type for internal buffering
     */
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class BufferedWriter : public Writer<TEncoding>, OutputAccumulator<typename TEncoding::CharType, TAllocator>
    {
    public:

        /**
         * \brief Character type of the writer
         */
        using CharType = typename Writer<TEncoding>::CharType;

        /**
         * \brief Size type used for write operations
         */
        using SizeType = typename Writer<TEncoding>::SizeType;

        /**
         * \brief Encoding type of the writer
         */
        using EncodingType = typename Writer<TEncoding>::EncodingType;

        /**
         * \brief Default size of the internal buffer
         */
        static constexpr SizeType DefaultBufferSize = 8192;

        /**
         * \brief Constructor
         *
         * Wraps a Writer and provides an internal buffer for more efficient writes.
         *
         * \param destination Reference to the Writer where data will eventually be written
         * \param bufferSize Size of the internal buffer (default is 8192 characters)
         */
        explicit BufferedWriter(Writer<TEncoding>& destination, SizeType bufferSize = DefaultBufferSize) :
            OutputAccumulator<CharType, TAllocator>(bufferSize),
            mDestination(&destination)
        {
            /// Nothing else to initialize
        }

        /**
         * \brief Destructor
         *
         * Automatically flushes any remaining buffered characters to the underlying Writer.
         */
        ~BufferedWriter() override
        {
            BufferedWriter::Flush();
        }

        /**
         * \brief Write characters to the buffer
         *
         * Appends \p length characters from \p text into the internal buffer.
         * If the buffer becomes full, it will be committed automatically.
         *
         * \param text Pointer to characters to write
         * \param length Number of characters to write
         */
        void Write(const CharType* text, SizeType length) override
        {
            OutputAccumulator<CharType, TAllocator>::Append(text, length);
        }

        /**
         * \brief Flush the buffer to the underlying writer
         *
         * Commits any buffered characters to the destination Writer.
         */
        void Flush() override
        {
            OutputAccumulator<CharType, TAllocator>::Commit();
        }

    private:

        /**
         * \brief Commit callback called by OutputAccumulator
         *
         * Writes the given characters to the underlying Writer.
         *
         * \param text Pointer to characters to commit
         * \param length Number of characters to commit
         */
        void DoCommit(const CharType* text, SizeType length) override
        {
            mDestination->Write(text, length);
        }

        Writer<TEncoding>* mDestination; ///< Underlying Writer where data is ultimately sent
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_BUFFEREDWRITER_HPP
