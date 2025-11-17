#ifndef COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP
#define COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP

#include <Cocktail/Core/IO/Output/Writer/Writer.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
    template <typename TEncoding = Encoders::Text>
    class LineWriter : public Writer<TEncoding>
    {
        using StringUtils = StringUtils<Encoders::CharType<TEncoding>, Encoders::SizeType<TEncoding>>;

    public:

        using CharType = typename Writer<TEncoding>::CharType;

        using SizeType = typename Writer<TEncoding>::SizeType;

        explicit LineWriter(Writer<TEncoding>& destination) :
            mDestination(&destination)
        {
            /// Nothing
        }

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

        void Write(const CharType* text)
        {
            Write(text, StringUtils::GetLength(text));
        }

        void WriteLine(const CharType* text)
        {
            WriteLine(text, StringUtils::GetLength(text));
        }

        void Write(const CharType* text, SizeType length) override
        {
            mDestination->Write(text, length);
        }

        void WriteLine(const CharType* text, SizeType length)
        {
            mDestination->Write(text, length);
            NextLine();
        }

        void Write(BasicStringView<TEncoding> string)
        {
            Write(string.GetData(), string.GetLength());
        }

        void WriteLine(BasicStringView<TEncoding> string)
        {
            WriteLine(string.GetData(), string.GetLength());
        }

        void Write(bool boolean)
        {
            static auto True = BasicStringView<TEncoding>("true");
            static auto False = BasicStringView<TEncoding>("false");
            Write(boolean ? True : False);
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
        void Write(T value)
        {
            Write(TranslatorCast<T, String>(value));
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
        void WriteLine(T value)
        {
            WriteLine(TranslatorCast<T, String>(value));
        }

        template <typename... TArgs>
        void Print(BasicStringView<TEncoding> format, TArgs&&... args)
        {
            Write(
                BasicString<TEncoding>::Format(format, std::forward<TArgs>(args)...)
            );
        }

        template <typename... TArgs>
        void PrintLine(BasicStringView<TEncoding> format, TArgs&&... args)
        {
            WriteLine(
                BasicString<TEncoding>::Format(format, std::forward<TArgs>(args)...)
            );
        }

        void Flush() override
        {
            mDestination->Flush();
        }

    private:

        Writer<TEncoding>* mDestination;
    };
}

#endif // COCKTAIL_CORE_IO_OUTPUT_WRITER_TRANSLATORWRITER_HPP
