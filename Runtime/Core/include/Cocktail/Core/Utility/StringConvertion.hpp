#ifndef COCKTAIL_CORE_UTILITY_STRINGCONVERTION_HPP
#define COCKTAIL_CORE_UTILITY_STRINGCONVERTION_HPP

#include <memory>

#include <Cocktail/Core/Export.hpp>

#define CK_ANSI_TO_TEXT(__String) ::Ck::AnsiToTextConverter().Get(__String)
#define CK_TEXT_TO_ANSI(__String) ::Ck::TextToAnsiConverter().Get(__String)

#define CK_UTF8_TO_TEXT(__String) ::Ck::Utf8ToTextConverter().Get(__String)
#define CK_TEXT_TO_UTF8(__String) ::Ck::TextToUtf8Converter().Get(__String)

#define CK_UTF32_TO_TEXT(__String) ::Ck::Utf32ToTextConverter().Get(__String)
#define CK_TEXT_TO_UTF32(__String) ::Ck::TextToUtf32Converter().Get(__String)

namespace Ck
{
    namespace Detail
    {
        template <typename TChar, Uint32 TSmallStringLength = 1024>
        struct BaseCharsetConverter
        {
        protected:

            TChar* AllocateOutputBuffer(std::size_t size)
            {
                if (size + 1 <= SharedBufferSize)
                    return SharedBuffer;

                if (size + 1 >= mBufferSize)
                {
                    mBufferSize = size;
                    mBuffer = std::make_unique<TChar[]>(size + 1);
                }

                return mBuffer.get();
            }

        private:

            static constexpr std::size_t SharedBufferSize = TSmallStringLength + 1;
            TChar SharedBuffer[SharedBufferSize];

            std::size_t mBufferSize = 0;
            std::unique_ptr<TChar[]> mBuffer;
        };
    }

    class AnsiConversionException;
    class Utf8ConversionException;
    class Utf16ConversionException;
    class Utf32ConversionException;

    /**
     * \class TextToAnsiConverter
     *
     * \brief Converts TextChar character strings to ANSI (char) strings
     */
    class COCKTAIL_CORE_API TextToAnsiConverter : public Detail::BaseCharsetConverter<AnsiChar>
	{
    public:

        /**
         * \brief Converters a given null terminated TextChar string to a null-terminated ANSI string
         *
         * \param string Null-terminated TextChar string to convert
         *
         * \return The converted ANSI string
         *
         * \throws AnsiConversionException If a character is outside ASCII range (0-127)
         */
        AnsiChar* Get(const TextChar* string);

        /**
         * \brief Converters a given TextChar string to a null-terminated ANSI string
         *
         * \param string TextChar string to convert
         * \param length The length of the source TextChar string
         *
         * \return The converted ANSI string
         *
         * \throws AnsiConversionException If a character is outside ASCII range (0-127)
         */
    	AnsiChar* Get(const TextChar* string, Uint32 length);
    };

    /**
     * \class AnsiToTextConverter
     *
     * \brief Converts ANSI strings to TextChar strings
     */
    class COCKTAIL_CORE_API AnsiToTextConverter : public Detail::BaseCharsetConverter<TextChar>
	{
    public:

        /**
         * \brief Converts an ANSI null-terminated string to a TextChar null-terminated string
         *
         * \param string Source ANSI string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const AnsiChar* string);

        /**
         * \brief Converts an ANSI string to a TextChar null-terminated string
         *
         * \param string Source ANSI string
         * \param length The length of the source ANSI string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const AnsiChar* string, Uint32 length);
    };

    class COCKTAIL_CORE_API TextToUtf8Converter : public Detail::BaseCharsetConverter<Utf8Char>
    {
    public:

        /**
         * \brief Converts an TextChar null-terminated string to an UTF8 null-terminated string
         *
         * \param string Source TextChar string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        Utf8Char* Get(const TextChar* string);

        /**
         * \brief Converts an TextChar null-terminated string to an UTF8 null-terminated string
         *
         * \param string Source TextChar string
         * \param length The length of the source UTF8 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        Utf8Char* Get(const TextChar* string, Uint32 length);
    };

    class COCKTAIL_CORE_API Utf8ToTextConverter : public Detail::BaseCharsetConverter<TextChar>
    {
    public:

        /**
         * \brief Converts an UTF8 null-terminated string to a TextChar null-terminated string
         *
         * \param string Source UTF8 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const Utf8Char* string);

        /**
         * \brief Converts an UTF8 string to a TextChar null-terminated string
         *
         * \param string Source UTF8 string
         * \param length The length of the source UTF8 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const Utf8Char* string, Uint32 length);
    };

    class COCKTAIL_CORE_API TextToUtf32Converter : public Detail::BaseCharsetConverter<Utf32Char>
    {
    public:

        /**
         * \brief Converts an TextChar null-terminated string to an UTF32 null-terminated string
         *
         * \param string Source TextChar string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        Utf32Char* Get(const TextChar* string);

        /**
         * \brief Converts an TextChar null-terminated string to an UTF32 null-terminated string
         *
         * \param string Source TextChar string
         * \param length The length of the source UTF32 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        Utf32Char* Get(const TextChar* string, Uint32 length);
    };

    class COCKTAIL_CORE_API Utf32ToTextConverter : public Detail::BaseCharsetConverter<TextChar>
    {
    public:

        /**
         * \brief Converts an UTF32 null-terminated string to a TextChar null-terminated string
         *
         * \param string Source UTF32 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const Utf32Char* string);

        /**
         * \brief Converts an UTF32 string to a TextChar null-terminated string
         *
         * \param string Source UTF32 string
         * \param length The length of the source UTF32 string
         *
         * \return Pointer to the internal converted null-terminated buffer
         */
        TextChar* Get(const Utf32Char* string, Uint32 length);
    };
}

#endif // COCKTAIL_CORE_UTILITY_STRINGCONVERTION_HPP
