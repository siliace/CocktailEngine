#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP

#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>

namespace Ck
{
    /**
     * \brief Writer that encodes characters and writes them to a binary output stream
     *
     * OutputStreamWriter adapts a character-based Writer interface to a byte-based
     * OutputStream. Characters are written directly as raw bytes using the encoding
     * specified by the template parameter.
     *
     * \tparam TEncoding The character encoding used to represent text (e.g., Utf8, Ascii)
     * \tparam TAllocator Allocator type used by the underlying OutputStream
     */
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
    class OutputStreamWriter : public Writer<TEncoding>
    {
    public:

        /**
         * \brief Character type of the encoding
         */
        using CharType = typename Writer<TEncoding>::CharType;

        /**
         * \brief Size type used for write operations
         */
        using SizeType = typename Writer<TEncoding>::SizeType;

        /**
         * \brief Encoding type used by this writer
         */
        using EncodingType = typename Writer<TEncoding>::EncodingType;

        /**
         * \brief Constructor
         *
         * Wraps a binary output stream to allow writing text using the specified encoding.
         *
         * \param destination The output stream that will receive the encoded bytes
         */
        explicit OutputStreamWriter(OutputStream<TAllocator>& destination) :
            mDestination(&destination)
        {
            /// Nothing to initialize beyond the pointer
        }

        /**
         * \brief Write characters to the underlying output stream
         *
         * Converts \p length characters starting at \p text into bytes and writes
         * them to the underlying OutputStream. The conversion is done according to
         * the encoding type TEncoding.
         *
         * \param text Pointer to the characters to write
         * \param length Number of characters to write
         */
        void Write(const CharType* text, SizeType length) override
        {
            mDestination->Write(reinterpret_cast<const Byte*>(text), length * sizeof(CharType));
        }

        /**
         * \brief Flush the underlying output stream
         *
         * Ensures all buffered bytes are written to the destination.
         */
        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        OutputStream<TAllocator>* mDestination; ///< Underlying byte-based output stream
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_OUTPUTSTREAMWRITER_HPP
