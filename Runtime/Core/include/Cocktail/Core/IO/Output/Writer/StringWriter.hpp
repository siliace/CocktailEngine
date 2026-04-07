#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Writer that accumulates written text in memory
     *
     * StringWriter provides a Writer interface that stores all written text
     * in an internal string buffer. It can be used for building strings dynamically
     * without writing to a file or stream.
     *
     * \tparam TEncoding The character encoding to use
     * \tparam TAllocator The allocator type for the internal string buffer
     */
    template <typename TEncoding = Encoders::Text, typename TAllocator = SizedHeapAllocator<32>>
    class StringWriter : public Writer<TEncoding>
    {
    public:

        /**
         * \brief Character type used by the writer
         */
        using CharType = typename Writer<TEncoding>::CharType;

        /**
         * \brief Size type for write operations
         */
        using SizeType = typename Writer<TEncoding>::SizeType;

        /**
         * \brief Write characters into the internal string buffer
         *
         * Appends \p length characters from \p text to the internal buffer.
         *
         * \param text Pointer to characters to write
         * \param length Number of characters to write
         */
        void Write(const CharType* text, SizeType length) override
        {
            mBuffer.Append(text, length);
        }

        /**
         * \brief Flush the writer
         *
         * No action is needed for StringWriter since data is kept in memory.
         */
        void Flush() override
        {
            /// Nothing
        }

        /**
         * \brief Get the accumulated text as a string
         *
         * Returns a copy of the internal buffer containing all text written so far.
         *
         * \return The accumulated string
         */
        BasicString<TEncoding, TAllocator> ToString() const
        {
            return mBuffer;
        }

    private:

        BasicString<TEncoding, TAllocator> mBuffer; ///< Internal buffer storing written characters
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
