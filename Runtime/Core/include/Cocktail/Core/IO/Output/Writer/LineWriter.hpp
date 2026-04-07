#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
    /**
     * \brief Writer that adds line-oriented output functionality
     *
     * LineWriter wraps another Writer and provides convenient methods to
     * write lines of text, optionally appending line breaks automatically.
     * It supports writing raw text, strings, numbers, booleans, and
     * formatted output.
     *
     * \tparam TEncoding The character encoding to use
     */
    template <typename TEncoding = Encoders::Text>
    class LineWriter : public Writer<TEncoding>
    {
        using StringUtils = StringUtils<Encoders::CharType<TEncoding>, Encoders::SizeType<TEncoding>>;

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
         * \brief Wraps an existing writer
         *
         * \param destination Writer that will receive the output
         */
        explicit LineWriter(Writer<TEncoding>& destination) :
            mDestination(&destination)
        {
            /// Nothing
        }

        /**
         * \brief Write a line break
         *
         * Appends a newline character (`\n`) or Windows-style CRLF (`\r\n`)
         * depending on the platform.
         */
        void NextLine()
        {
            CharType lineBreak[] = {
#ifdef COCKTAIL_OS_WINDOWS
                '\r',
#endif
                '\n'
            };
            mDestination->Write(lineBreak, lineBreak[0] != '\n' ? 2 : 1);
        }

        /**
         * \brief Write a string without a line break
         *
         * \param text Null-terminated character array
         */
        void Write(const CharType* text)
        {
            Write(text, StringUtils::GetLength(text));
        }

        /**
         * \brief Write a string followed by a line break
         *
         * \param text Null-terminated character array
         */
        void WriteLine(const CharType* text)
        {
            WriteLine(text, StringUtils::GetLength(text));
        }

        /**
         * \brief Write a string with specified length
         *
         * \param text Pointer to characters
         * \param length Number of characters to write
         */
        void Write(const CharType* text, SizeType length) override
        {
            mDestination->Write(text, length);
        }

        /**
         * \brief Write a string with specified length followed by a line break
         *
         * \param text Pointer to characters
         * \param length Number of characters to write
         */
        void WriteLine(const CharType* text, SizeType length)
        {
            mDestination->Write(text, length);
            NextLine();
        }

        /**
         * \brief Write a BasicStringView without a line break
         */
        void Write(BasicStringView<TEncoding> string)
        {
            Write(string.GetData(), string.GetLength());
        }

        /**
         * \brief Write a BasicStringView followed by a line break
         */
        void WriteLine(BasicStringView<TEncoding> string)
        {
            WriteLine(string.GetData(), string.GetLength());
        }

        /**
         * \brief Write a boolean value as "true" or "false"
         *
         * \param boolean Boolean value to write
         */
        void Write(bool boolean)
        {
            static auto True = BasicStringView<TEncoding>("true");
            static auto False = BasicStringView<TEncoding>("false");
            Write(boolean ? True : False);
        }

        /**
         * \brief Write an arithmetic value as a string
         *
         * Uses TranslatorCast to convert numbers to strings before writing.
         *
         * \tparam T Type of value (must be arithmetic)
         * \param value Value to write
         */
        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
        void Write(T value)
        {
            Write(TranslatorCast<T, String>(value));
        }

        /**
         * \brief Write an arithmetic value followed by a line break
         *
         * \tparam T Type of value (must be arithmetic)
         * \param value Value to write
         */
        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
        void WriteLine(T value)
        {
            WriteLine(TranslatorCast<T, String>(value));
        }

        /**
         * \brief Write formatted output without a line break
         *
         * \tparam TArgs Types of format arguments
         * \param format Format string
         * \param args Arguments to format
         */
        template <typename... TArgs>
        void Print(BasicStringView<TEncoding> format, TArgs&&... args)
        {
            Write(BasicString<TEncoding>::Format(format, std::forward<TArgs>(args)...));
        }

        /**
         * \brief Write formatted output followed by a line break
         *
         * \tparam TArgs Types of format arguments
         * \param format Format string
         * \param args Arguments to format
         */
        template <typename... TArgs>
        void PrintLine(BasicStringView<TEncoding> format, TArgs&&... args)
        {
            WriteLine(BasicString<TEncoding>::Format(format, std::forward<TArgs>(args)...));
        }

        /**
         * \brief Flush the underlying writer
         */
        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        Writer<TEncoding>* mDestination; ///< Underlying writer that receives output
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP
