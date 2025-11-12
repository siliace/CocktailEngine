#ifndef COCKTAIL_CORE_UTILITY_ENCODING_ASCIIENCODER_HPP
#define COCKTAIL_CORE_UTILITY_ENCODING_ASCIIENCODER_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief ASCII encoder implementation
     *
     * This encoder provides functions to encode and decode characters
     * between ASCII and UTF-32 representations. It assumes the character type is
     * a single-byte type (e.g. `char` or `unsigned char`).
     *
     * \tparam TChar The character type used to represent ASCII characters
     * \tparam TSize The size type used for counting characters or lengths
     */
    template <typename TChar, typename TSize>
    struct AsciiEncoder
    {
        /**
         * \brief Alias for the character type used by the encoder
         *
         * This represents the raw type of an ASCII character, usually `char` or `unsigned char`.
         */
        using CharType = TChar;

        /**
         * \brief Alias for the type used to represent sizes or lengths
         *
         * Typically `size_t`, `uint32_t`, or another unsigned integer type.
         */
        using SizeType = TSize;

        static_assert(sizeof(CharType) == 1, "Ascii char should be encoded on 1 byte type");

        /**
         * \brief Indicates whether the encoding supports surrogate pairs
         *
         * Since ASCII encoding only covers code points in the range [0, 127],
         * surrogate pairs are not supported.
         */
        static constexpr bool SupportSurrogate = false;

        /**
         * \brief Maximum number of bytes required to encode a single code point.
         *
         * For ASCII, each code point is encoded in exactly one byte
         */
        static constexpr SizeType MaxCodepointEncodingLength = 1;

        /**
         * \brief Returns the number of ASCII characters represented by the given string
         *
         * This function assumes that the input string is valid and always
         * returns 1 value since each ASCII character occupies one character.
         *
         * \param string Pointer to the input string
         *
         * \return Always returns 1
         */
        static SizeType GetCharCount(const CharType* string)
        {
            return 1;
        }

        /**
         * \brief Returns the number of ASCII characters represented by a string of known length
         *
         * In ASCII encoding, the number of characters is equal to the number of characters.
         *
         * \param string Pointer to the input string
         * \param length Number of character in the input string
         *
         * \return The number of ASCII characters, equal to \p length
         */
        static SizeType GetCharCount(const CharType* string, SizeType length)
        {
            return length;
        }

        /**
         * \brief Decodes a single ASCII character into a UTF-32 code point
         *
         * \param in Pointer to the input ASCII character
         * \param out Reference to the output UTF-32 character
         *
         * \return The number of character read from the input (always 1)
         */
        static SizeType Decode(const CharType* in, Utf32Char& out)
        {
            out = static_cast<Utf32Char>(in[0]);
            return 1;
        }

        /**
         * \brief Encodes a UTF-32 character into an ASCII character
         *
         * This function converts a single UTF-32 code point into an ASCII character.
         * Only code points in the range [0, 127] are valid in ASCII. If the input
         * code point is outside this range, the function returns 0 to indicate failure.
         *
         * \param in  The input UTF-32 code point
         * \param out Pointer to the output buffer (must have at least one character available)
         *
         * \return The number of character written to \p out (1 on success, 0 on failure)
         */
        static unsigned int Encode(Utf32Char in, CharType* out)
        {
            if (in > 127)
                return 0;

            out[0] = static_cast<CharType>(in);
            return 1;
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_ENCODING_ASCIIENCODER_HPP
