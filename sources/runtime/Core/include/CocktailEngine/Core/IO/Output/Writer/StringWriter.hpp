#ifndef COCKTAILENGINE_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
#define COCKTAILENGINE_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP

#include <CocktailEngine/Core/IO/Output/Writer/Writer.hpp>
#include <CocktailEngine/Core/String.hpp>

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
         * \brief Type of the string being written
         */
        using StringType = BasicString<TEncoding, TAllocator>;

        /**
         * \brief Type of the string being written
         */
        using StringViewType = StringType::View;

        /**
         * \brief Write a single character into the internal string buffer
         *
         * \param c The character to write
         */
        void Write(CharType c)
        {
            mBuffer.Append(&c, 1);
        }

        /**
         * \brief Write null-terminated characters
         *
         * \param text Pointer to characters to write
         */
        void Write(const CharType* text)
        {
            using StringUtils = StringUtils<CharType, SizeType>;

            mBuffer.Append(text, StringUtils::GetLength(text));
        }

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
         * \brief Write a string into the internal string buffer
         *
         * \param string The string to write
         */
        void Write(StringType string)
        {
            mBuffer.Append(string.GetData(), string.GetLength());
        }

        /**
         * \brief Write a string view into the internal string buffer
         *
         * \param string The string view to write
         */
        void Write(StringViewType string)
        {
            mBuffer.Append(string.GetData(), string.GetLength());
        }

        /**
         * \brief Discard the content written so far
         *
         * This method can be used to recycle the writer to avoid recurrent memory allocations.
         */
        void Clear()
        {
            mBuffer.Clear();
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
        const BasicString<TEncoding, TAllocator>& ToString() const
        {
            return mBuffer;
        }

    private:

        BasicString<TEncoding, TAllocator> mBuffer; ///< Internal buffer storing written characters
    };
}

#endif // COCKTAILENGINE_CORE_IO_OUTPUT_WRITER_STRINGWRITER_HPP
