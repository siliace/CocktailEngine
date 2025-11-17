#ifndef COCKTAIL_CORE_UTILITY_ENCODING_UTF16ENCODER_HPP
#define COCKTAIL_CORE_UTILITY_ENCODING_UTF16ENCODER_HPP

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief UTF-16 encoder implementation
     *
     * This encoder provides functions to encode and decode characters
     * between UTF-16 and UTF-32 representations. It assumes the character type is
     * an unsigned single-byte type (e.g. `Utf16Char`).
     *
     * \tparam TChar The character type used to represent UTF-16 characters
     * \tparam TSize The size type used for counting characters or lengths
     */
    template <typename TChar, typename TSize>
    struct Utf16Encoder
    {
        /**
         * \brief Alias for the character type used by the encoder
         *
         * This represents the raw type of an Utf16 character, usually `Utf16Char`.
         */
        using CharType = TChar;

        /**
         * \brief Alias for the type used to represent sizes or lengths
         *
         * Typically `size_t`, `uint32_t`, or another unsigned integer type.
         */
        using SizeType = TSize;

        static_assert(sizeof(CharType) == 2, "UTF-16 CharType size should be 2 byte");
        static_assert(std::is_unsigned_v<CharType>, "UTF-16 CharType should be unsigned");

        /**
         * \brief Indicates whether the encoding supports surrogate pairs
         */
        static constexpr bool SupportSurrogate = true;

        /**
         * \brief Maximum number of bytes required to encode a single code point.
         *
         * For UTF-16, each code point can be encoded using up to 2 characters
         */
        static constexpr SizeType MaxCodepointEncodingLength = 2;

        /**
         * \brief Determines the number of bytes required to represent the first UTF-16 character in a string
         *
         * Inspects the leading byte of the given UTF-16 sequence to determine the number of characters that form the
         * first character.
         *
         * \param string Pointer to the beginning of the UTF-16 encoded string
         *
         * \return The number of character in the first UTF-16 character, or 0 if invalid
         */
        static SizeType GetCharCount(const CharType* string)
        {
            CharType w1 = string[0];
            if (w1 >= 0xDC00 && w1 <= 0xDFFF)
                return 0;

            if (w1 >= 0xD800 && w1 <= 0xDBFF)
            {
                CharType w2 = string[1];
                if (w2 < 0xDC00 || w2 > 0xDFFF)
                    return 0;

                return 2;
            }

            return 1;
        }
        
        /**
         * \brief Counts the number of UTF-16 characters in a buffer of known length
         *
         * Iterates through the buffer, using \ref GetCharCount to determine the byte length of each character.
         * Invalid byte sequences are skipped, and the count reflects valid UTF-16 characters only.
         *
         * \param string Pointer to the UTF-16 encoded data
         * \param length Number of bytes in the input buffer
         *
         * \return The number of valid UTF-16 characters in the buffer
         */
        static SizeType GetCharCount(const CharType* string, SizeType length)
        {
            SizeType count = 0;
            for (SizeType i = 0; i < length; i += GetCharCount(string + i))
                count += 1;

            return count;
        }

        /**
         * \brief Decodes a UTF-16 sequence into a single UTF-32 code point
         *
         * Reads up to 4 bytes from the input sequence and converts them into a UTF-32 character.
         * If the sequence is invalid, the function returns 0.
         *
         * \param in Pointer to the UTF-16 sequence to decode
         * \param length The length of data available inside of \p in
         * \param out Reference to a UTF-32 variable to store the decoded code point
         *
         * \return The number of bytes read from \p in, or 0 if decoding failed
         */
        static SizeType Decode(const CharType* in, SizeType length, Utf32Char& out)
        {
            if (length < 1)
                return 0;

            CharType w1 = in[0];
            if (w1 >= 0xDC00 && w1 <= 0xDFFF)
                return 0;

            if (w1 >= 0xD800 && w1 <= 0xDBFF)
            {
                if (length < 2 || in[1] < 0xDC00 || in[1] > 0xDFFF)
                    return 0;

                out = (static_cast<Utf32Char>(w1 - 0xD800) << 10) + (in[1] - 0xDC00) + 0x10000;
                return 2;
            }

            out = static_cast<Utf32Char>(in[0]);
            return 1;
        }

        /**
         * \brief Encodes a UTF-32 code point into a UTF-16 sequence
         *
         * Converts a single Unicode code point (UTF-32) into its UTF-16 representation.
         * Returns the number of bytes written, or 0 if the code point is invalid.
         *
         * \param in The UTF-32 code point to encode
         * \param out Pointer to the output buffer (must have space for up to 4 bytes)
         *
         * \return The number of bytes written to \p out, or 0 if the input is invalid
         */
        static unsigned int Encode(Utf32Char in, CharType* out)
        {
            if (in <= 0xFFFF)
            {
                if (in >= 0xD800 && in <= 0xDFFF)
                    return 0;

                out[0] = static_cast<CharType>(in);
                return 1;
            }

            if (in <= 0x10FFFF)
            {
                in -= 0x10000;
                out[0] = 0xD800 | in >> 10;
                out[1] = 0xDC00 | in & 0x3FF;

                return 2;
            }

            return 0;
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_ENCODING_UTF16ENCODER_HPP
