#ifndef COCKTAIL_CORE_UTILITY_ENCODING_UTF32ENCODER_HPP
#define COCKTAIL_CORE_UTILITY_ENCODING_UTF32ENCODER_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief UTF-32 encoder implementation
     *
     * This structure provides static methods to encode and decode characters
     * between UTF-32 representations. It assumes that the character type is
     * a 4-byte unsigned integer (e.g., `Utf32Char`).
     *
     * \tparam TChar The character type used to represent UTF-32 code points
     * \tparam TSize The type used for counting characters or lengths
     */
    template <typename TChar, typename TSize>
    struct Utf32Encoder
    {
        /**
         * \brief Alias for the character type used by the encoder
         *
         * Represents a single UTF-32 code point, usually `Utf32Char` or another 4-byte unsigned type.
         */
        using CharType = TChar;

        /**
         * \brief Alias for the type used to represent sizes or lengths
         *
         * Typically `size_t`, `uint32_t`, or another unsigned integer type.
         */
        using SizeType = TSize;

        static_assert(sizeof(CharType) == 4, "UTF32 CharType size should be 4 byte");
        static_assert(std::is_unsigned_v<CharType>, "UTF32 CharType should be unsigned");

        /**
         * \brief Indicates whether the encoding supports surrogate pairs
         *
         * UTF-32 encodes all Unicode code points directly in one 4-byte value,
         * so surrogate pairs are not used.
         */
        static constexpr bool SupportSurrogate = false;

        /**
         * \brief Maximum number of character required to encode a single code point
         *
         * For UTF-32, each code point is encoded in exactly one character.
         */
        static constexpr SizeType MaxCodepointEncodingLength = 1;

        /**
         * \brief Returns the number of UTF-32 code points represented by the given pointer
        *
         * This function assumes that the input string is valid and always
         * returns 1 value since each UTF-32 character occupies one character.
         *
         * \param string Pointer to a UTF-32 code point
         *
         * \return Always returns 1
         */
        static SizeType GetCharCount(const CharType* string)
        {
            return 1;
        }

        /**
         * \brief Returns the number of UTF-32 characters represented by a string of known length
         *
         * In UTF-32 encoding, the number of characters is equal to the number of characters.
         *
         * \param string Pointer to the input string
         * \param length Number of bytes in the input string
         *
         * \return The number of UTF-32 characters, equal to \p length
         */
        static SizeType GetCharCount(const CharType* string, SizeType length)
        {
            return length;
        }

        /**
         * \brief Decodes a single UTF-32 character into a UTF-32 code point
         *
         * This function is just a copy of a character to another one since no decoding is needed.
         *
         * \param in Pointer to the input UTF-32 character
         * \param out Reference to the output UTF-32 character
         *
         * \return The number of character read from the input (always 1)
         */
        static Utf32Char Decode(const CharType* in, Utf32Char& out)
        {
            out = static_cast<Utf32Char>(in[0]);
            return 1;
        }

        /**
         * \brief Encodes a UTF-32 character into an UTF-32 character
         *
         * This function is just a copy of a character to another one since no encoding is needed.
         *
         * \param in The input UTF-32 code point
         * \param out Pointer to the output buffer (must have at least one character available)
         *
         * \return The number of character written to \p out (always 1)
         */
        static unsigned int Encode(Utf32Char in, CharType* out)
        {
            out[0] = static_cast<CharType>(in);
            return 1;
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_ENCODING_UTF32ENCODER_HPP
