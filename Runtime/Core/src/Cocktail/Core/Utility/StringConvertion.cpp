#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_EXCEPTION_FROM(AnsiConversionException, RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(Utf8ConversionException, RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(Utf16ConversionException, RuntimeException);
    COCKTAIL_DECLARE_EXCEPTION_FROM(Utf32ConversionException, RuntimeException);

    AnsiChar* TextToAnsiConverter::Get(const TextChar* string)
    {
        return Get(string, StringUtils<TextChar>::GetLength(string));
    }

    AnsiChar* TextToAnsiConverter::Get(const TextChar* string, Uint32 length)
    {
        AnsiChar* out = this->AllocateOutputBuffer(length);

        for (Uint32 i = 0; i < length; i++)
        {
            TextChar wild = string[i];
            if (wild < 0 || wild > 127)
                throw AnsiConversionException(CK_TEXT("Non-ascii character detected"));

            out[i] = static_cast<AnsiChar>(wild);
        }

        out[length] = '\0';

        return out;
    }

    TextChar* AnsiToTextConverter::Get(const AnsiChar* string)
    {
        return Get(string, StringUtils<AnsiChar>::GetLength(string));
    }

    TextChar* AnsiToTextConverter::Get(const AnsiChar* string, Uint32 length)
    {
        TextChar* out = this->AllocateOutputBuffer(length);

        for (Uint32 i = 0; i < length; i++)
            out[i] = static_cast<WildChar>(string[i]);

        out[length] = CK_TEXT('\0');

        return out;
    }

    Utf8Char* TextToUtf8Converter::Get(const TextChar* string)
    {
        throw NotImplementedException();
    }

    Utf8Char* TextToUtf8Converter::Get(const TextChar* string, Uint32 length)
    {
        throw NotImplementedException();
    }

    TextChar* Utf8ToTextConverter::Get(const Utf8Char* string)
    {
        throw NotImplementedException();
    }

    TextChar* Utf8ToTextConverter::Get(const Utf8Char* string, Uint32 length)
    {
        throw NotImplementedException();
    }

    Utf32Char* TextToUtf32Converter::Get(const TextChar* string)
    {
        return Get(string, StringUtils<TextChar>::GetLength(string));
    }

    Utf32Char* TextToUtf32Converter::Get(const TextChar* string, Uint32 length)
    {
        Uint32 i = 0, j = 0;
        Utf32Char* out = this->AllocateOutputBuffer(length);

        while (i < length && j < length)
        {
            TextChar text = string[i++];

            if (text < 0xD800 || text > 0xDFFF)
            {
                out[j++] = static_cast<Utf32Char>(text);
            }
            else if (text >= 0xD800 && text <= 0xDBFF)
            {
                if (i < length)
                {
                    uint16_t nextUtf16 = string[i];
                    if (nextUtf16 >= 0xDC00 && nextUtf16 <= 0xDFFF)
                    {
                        Utf32Char utf32 = 0x10000 + (((text - 0xD800) << 10) | (nextUtf16 - 0xDC00));
                        out[j++] = utf32;
                        i++;
                    }
                    else
                    {
                        throw Utf32ConversionException();
                    }
                }
                else
                {
                    throw Utf32ConversionException();
                }
            }
            else
            {
                throw Utf32ConversionException();
            }
        }

        return out;
    }

    TextChar* Utf32ToTextConverter::Get(const Utf32Char* string)
    {
        return Get(string, StringUtils<Utf32Char>::GetLength(string));
    }

    TextChar* Utf32ToTextConverter::Get(const Utf32Char* string, Uint32 length)
    {
        Uint32 i = 0, j = 0;
        TextChar* out = this->AllocateOutputBuffer(length);

        while (i < length)
        {
            Utf32Char codepoint = string[i++];

            if (codepoint <= 0xFFFF)
            {
                if (codepoint >= 0xD800 && codepoint <= 0xDFFF)
                throw Utf32ConversionException();

                if (j >= length)
                    throw Utf32ConversionException();

                out[j++] = static_cast<TextChar>(codepoint);
            }
            else if (codepoint <= 0x10FFFF)
            {
                if (j + 2 > length)
                    throw Utf32ConversionException();

                codepoint -= 0x10000;
                out[j++] = static_cast<TextChar>(0xD800 + (codepoint >> 10));      
                out[j++] = static_cast<TextChar>(0xDC00 + (codepoint & 0x3FF));  
            }
            else
            {
                throw Utf32ConversionException();
            }
        }

        return out;
    }
}
