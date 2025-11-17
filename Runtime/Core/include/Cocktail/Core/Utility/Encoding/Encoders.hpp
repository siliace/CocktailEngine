#ifndef COCKTAIL_CORE_UTILITY_ENCODING_ENCODERS_HPP
#define COCKTAIL_CORE_UTILITY_ENCODING_ENCODERS_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/Encoding/AsciiEncoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf16Encoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf32Encoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf8Encoder.hpp>

namespace Ck
{
    /**
     * \class Encoders
     *
     * \brief Provides utilities for converting between various text encodings
     *
     * This class offers static methods to convert strings and byte arrays
     * between different character encodings (ASCII, UTF-8, UTF-16, UTF-32).
     * Conversion is performed codepoint-by-codepoint using encoder types.
     */
    class Encoders
    {
    public:

        template <typename T>
        using CharType = typename T::CharType;

        template <typename T>
        using SizeType = typename T::SizeType;

        /**
         * \brief ASCII encoder alias
         */
        using Ascii = AsciiEncoder<AnsiChar, Uint32>;

        /**
         * \brief UTF-8 alias
         */
        using Utf8 = Utf8Encoder<Utf8Char, Uint32>;

        /**
         * \brief UTF-16 encoder alias
         */
        using Utf16 = Utf16Encoder<Utf16Char, Uint32>;

        /**
         * \brief UTF-32 encoder alias
         */
        using Utf32 = Utf32Encoder<Utf32Char, Uint32>;

#ifdef COCKTAIL_OS_WINDOWS
        /**
         * \brief Platform specific text encoder alias
         */
        using Text = Utf16Encoder<WildChar, Uint32>;
#else
        /**
         * \brief Platform specific text encoder alias
         */
        using Text = Utf8Encoder<Utf8Char, Uint32>;
#endif

        /**
         * \brief Converts a string view to a byte array using the specified destination encoder
         *
         * \tparam TDst Encoder used to encode the output bytes
         * \tparam TStringView Type of the string view providing characters
         *
         * \param stringView The view of the string to convert
         *
         * \return A ByteArray containing encoded bytes
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TDst, typename TStringView>
        static ByteArray GetBytes(TStringView stringView)
        {
            using StringViewEncodingType = typename TStringView::EncodingType;
            return GetBytes<StringViewEncodingType, TDst>(stringView.GetData(), 0, stringView.GetLength());
        }

        /**
         * \brief Converts raw characters from a source encoding to encoded bytes in a destination encoding
         *
         * \tparam TSrc Encoder describing the source character format
         * \tparam TDst Encoder used to produce the output bytes
         *
         * \param characters Pointer to the source characters
         * \param first Starting index to begin decoding
         * \param length Number of source characters to process
         *
         * \return ByteArray containing encoded representation
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TSrc, typename TDst>
        static ByteArray GetBytes(const CharType<TSrc>* characters, SizeType<TSrc> first, SizeType<TSrc> length)
        {
            ByteArray bytes;
            for (SizeType<TSrc> i = first; i < length;)
            {
                CharType<TDst> buffer[TDst::MaxCodepointEncodingLength];
                auto [decoded, encoded] = ConvertCodepoint<TSrc, TDst>(characters + i, length - i, buffer);
                if (decoded == 0)
                    ExceptionUtils::ThrowCodepointDecodingException(i);

                bytes.Append(buffer, encoded);

                i += decoded;
            }

            return bytes;
        }

        /**
         * \brief Converts encoded bytes into an array of destination characters
         *
         * \tparam TSrc Encoder used to interpret the input bytes
         * \tparam TDst Encoder producing the output characters
         *
         * \param bytes A view over a byte array
         *
         * \return An Array of characters in the destination encoding
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TSrc, typename TDst>
        static Array<CharType<TDst>> GetCharacters(ByteArrayView bytes)
        {
            return GetCharacters<TSrc, TDst>(bytes.GetData(), bytes.GetSize());
        }

        /**
         * \brief Converts raw bytes into an array of destination-encoded characters
         *
         * \tparam TSrc Decoder for the source bytes
         * \tparam TDst Encoder for the output characters
         *
         * \param bytes Pointer to the byte buffer
         * \param size Number of bytes to read
         *
         * \return Array of decoded characters
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TSrc, typename TDst>
        static Array<CharType<TDst>> GetCharacters(const Byte* bytes, SizeType<TSrc> size)
        {
            Array<CharType<TDst>> characters;
            for (SizeType<TSrc> i = 0; i < size;)
            {
                CharType<TDst> buffer[TDst::MaxCodepointEncodingLength];

                auto offset = reinterpret_cast<const CharType<TSrc>*>(bytes) + i;
                auto [decoded, encoded] = ConvertCodepoint<TSrc, TDst>(offset, size - i, buffer);
                if (decoded == 0)
                    ExceptionUtils::ThrowCodepointDecodingException(i);

                characters.Append(buffer, encoded);

                i += decoded;
            }

            return characters;
        }

        /**
         * \brief Converts encoded bytes into a strongly typed string
         *
         * \tparam TSrc Encoder used to decode the byte input
         * \tparam TString String type to construct
         *
         * \param bytes Byte array view containing encoded text
         *
         * \return Constructed string containing decoded characters
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TSrc, typename TString>
        static TString GetString(ByteArrayView bytes)
        {
            return GetString<TSrc, TString>(bytes.GetData(), bytes.GetSize());
        }

        /**
         * \brief Converts encoded bytes into a string of the specified type.
         *
         * \tparam TSrc Encoder used to read input bytes.
         * \tparam TString Output string type storing characters and encoder
         *
         * \param bytes Pointer to encoded byte data
         * \param size Number of bytes available
         *
         * \return Constructed string containing decoded characters
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         * \throws CodepointDecodingException if a codepoint cannot be decoded
         */
        template <typename TSrc, typename TString>
        static TString GetString(const Byte* bytes, SizeType<TSrc> size)
        {
            using StringCharType = typename TString::CharType;
            using StringEncodingType = typename TString::EncodingType;

            TString string;
            string.mCharacters = GetCharacters<TSrc, StringEncodingType>(bytes, size);
            string.mCharacters.Add(static_cast<StringCharType>('\0'));

            return string;
        }

    private:

        /**
         * \brief Converts a single codepoint from source encoding to destination encoding
         *
         * \tparam TSrc Decoder for the source data
         * \tparam TDst Encoder for the output data
         *
         * \param source Pointer to characters encoded in the source format
         * \param length The number of characters available in \p source
         * \param destination Output buffer for encoded destination characters
         * \return A pair {decodedSize, encodedSize}.
         *
         * \throws CodepointEncodingException if a codepoint cannot be encoded
         */
        template <typename TSrc, typename TDst>
        static auto ConvertCodepoint(const CharType<TSrc>* source, SizeType<TSrc> length, CharType<TDst>* destination)
        {
            Utf32Char codepoint;
            SizeType<TDst> encoded = 0;
            SizeType<TSrc> decoded = TSrc::Decode(source, length, codepoint);
            if (decoded != 0)
            {
                encoded = TDst::Encode(codepoint, destination);
                if (encoded == 0)
                    ExceptionUtils::ThrowCodepointEncodingException(decoded);
            }

            return std::make_pair(decoded, encoded);
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_ENCODING_ENCODERS_HPP
