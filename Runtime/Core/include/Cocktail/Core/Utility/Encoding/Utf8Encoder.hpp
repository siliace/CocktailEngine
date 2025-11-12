#ifndef COCKTAIL_CORE_UTILITY_ENCODING_UTF8ENCODER_HPP
#define COCKTAIL_CORE_UTILITY_ENCODING_UTF8ENCODER_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief UTF-8 encoder implementation
     *
     * This encoder provides functions to encode and decode characters
     * between UTF-8 and UTF-32 representations. It assumes the character type is
     * an unsigned single-byte type (e.g. `Utf8Char`).
     *
     * \tparam TChar The character type used to represent UTF-8 characters
     * \tparam TSize The size type used for counting characters or lengths
     */
    template <typename TChar, typename TSize>
    struct Utf8Encoder
    {
        /**
         * \brief Alias for the character type used by the encoder
         *
         * This represents the raw type of an Utf8 character, usually `Utf8Char` or another single-byte unsigned type.
         */
        using CharType = TChar;

        /**
         * \brief Alias for the type used to represent sizes or lengths
         *
         * Typically `size_t`, `uint32_t`, or another unsigned integer type.
         */
        using SizeType = TSize;

        static_assert(sizeof(CharType) == 1, "UTF-8 CharType size should be 1 byte");
        static_assert(std::is_unsigned_v<CharType>, "UTF-8 CharType should be unsigned");

        /**
         * \brief Indicates whether the encoding supports surrogate pairs
         */
        static constexpr bool SupportSurrogate = true;

        /**
         * \brief Maximum number of bytes required to encode a single code point.
         *
         * For UTF-8, each code point can be encoded using up to 4 characters
         */
        static constexpr SizeType MaxCodepointEncodingLength = 4;

        /**
         * \brief Determines the number of bytes required to represent the first UTF-8 character in a string
         *
         * Inspects the leading byte of the given UTF-8 sequence to determine the number of characters that form the
         * first character.
         *
         * \param string Pointer to the beginning of the UTF-8 encoded string
         *
         * \return The number of character in the first UTF-8 character, or 0 if invalid
         */
        static SizeType GetCharCount(const CharType* string)
        {
            if (string[0] < 0x80)
                return 1;

            if ((string[0] & 0xE0) == 0xC0)
            {
                if ((string[1] & 0xC0) != 0x80)
                    return 0;

                return 2;
            }

            if ((string[0] & 0xF0) == 0xE0)
            {
                if ((string[1] & 0xC0) != 0x80 || (string[2] & 0xC0) != 0x80)
                    return 0;

                return 3;
            }
            if ((string[0] & 0xF8) == 0xF0)
            {
                if ((string[1] & 0xC0) != 0x80 || (string[2] & 0xC0) != 0x80 || (string[3] & 0xC0) != 0x80)
                    return 0;

                return 4;
            }

            return 0;
        }

        /**
         * \brief Counts the number of UTF-8 characters in a buffer of known length
         *
         * Iterates through the buffer, using \ref GetCharCount to determine the byte length of each character.
         * Invalid byte sequences are skipped, and the count reflects valid UTF-8 characters only.
         *
         * \param string Pointer to the UTF-8 encoded data
         * \param length Number of bytes in the input buffer
         *
         * \return The number of valid UTF-8 characters in the buffer
         */
        static SizeType GetCharCount(const CharType* string, SizeType length)
        {
            SizeType count = 0;
            for (SizeType i = 0; i < length; i += GetCharCount(string + i))
                count += 1;

            return count;
        }

        /**
         * \brief Decodes a UTF-8 sequence into a single UTF-32 code point
         *
         * Reads up to 4 bytes from the input sequence and converts them into a UTF-32 character.
         * If the sequence is invalid, the function returns 0.
         *
         * \param in Pointer to the UTF-8 sequence to decode
         * \param out Reference to a UTF-32 variable to store the decoded code point
         *
         * \return The number of bytes read from \p in, or 0 if decoding failed
         */
        static SizeType Decode(const CharType* in, Utf32Char& out)
        {
            if (in[0] < 0x80)
            {
                out = in[0];
                return 1;
            }

            if ((in[0] & 0xE0) == 0xC0)
            {
                if ((in[1] & 0xC0) != 0x80)
                    return 0;

                out = (in[0] & 0x1F) << 6 | in[1] & 0x3F;
                return 2;
            }

            if ((in[0] & 0xF0) == 0xE0)
            {
                if ((in[1] & 0xC0) != 0x80 || (in[2] & 0xC0) != 0x80)
                    return 0;

                out = (in[0] & 0x0F) << 12 | (in[1] & 0x3F) << 6 | in[2] & 0x3F;
                return 3;
            }
            if ((in[0] & 0xF8) == 0xF0)
            {
                if ((in[1] & 0xC0) != 0x80 || (in[2] & 0xC0) != 0x80 || (in[3] & 0xC0) != 0x80)
                    return 0;

                out = (in[0] & 0x07) << 18 | (in[1] & 0x3F) << 12 | (in[2] & 0x3F) << 6 | in[3] & 0x3F;
                return 4;
            }

            return out;
        }

        /**
         * \brief Encodes a UTF-32 code point into a UTF-8 sequence
         *
         * Converts a single Unicode code point (UTF-32) into its UTF-8 representation.
         * Returns the number of bytes written, or 0 if the code point is invalid.
         *
         * \param in The UTF-32 code point to encode
         * \param out Pointer to the output buffer (must have space for up to 4 bytes)
         *
         * \return The number of bytes written to \p out, or 0 if the input is invalid
         */
        static unsigned int Encode(Utf32Char in, CharType* out)
        {
            if (in <= 0x7F)
            {
                out[0] = static_cast<CharType>(in);
                return 1;
            }

            if (in <= 0x7FF)
            {
                out[0] = 0xC0 | in >> 6;
                out[1] = 0x80 | in & 0x3F;
                return 2;
            }

            if (in <= 0xFFFF)
            {
                out[0] = 0xE0 | in >> 12;
                out[1] = 0x80 | in >> 6 & 0x3F;
                out[2] = 0x80 | in & 0x3F;
                return 3;
            }

            if (in <= 0x10FFFF)
            {
                out[0] = 0xF0 | in >> 18;
                out[1] = 0x80 | in >> 12 & 0x3F;
                out[2] = 0x80 | in >> 6 & 0x3F;
                out[3] = 0x80 | in & 0x3F;
                return 4;
            }

            return 0;
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_ENCODING_UTF8ENCODER_HPP
