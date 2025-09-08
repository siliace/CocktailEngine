#ifndef COCKTAIL_CORE_UTILITY_CHARACTERS_HPP
#define COCKTAIL_CORE_UTILITY_CHARACTERS_HPP

#include <string>

#include <Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp>
#include <Cocktail/Core/Utility/CharUtils.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
    /**
     * \class StringUtils
     * \brief Utility class for character and character-sequence operations
     *
     * This templated class provides static helper functions for comparing,
     * searching and inspecting character sequences. It supports both
     * case-sensitive and case-insensitive operations.
     *
     * \tparam TChar Character type (e.g. AnsiChar, WildChar and likely most of the time TextChar)
     * \tparam TSize Unsigned integer type used for sizes and indexing
     */
    template <typename TChar, typename TSize = IndexSizeToSizeType<32>::Type>
    class StringUtils
    {
    public:

        static_assert(std::is_unsigned_v<TSize>, "TSize should be an unsigned integer");

        using CharType = TChar; /*!< Alias for character type */
        using SizeType = TSize; /*!< Alias for size/index type */

        /**
         * \brief Compares two characters for equality
         *
         * \param lhs Left-hand side character
         * \param rhs Right-hand side character
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if characters are equal under chosen comparison mode
         */
        static bool Equal(CharType lhs, CharType rhs, bool caseSensitive)
        {
            return caseSensitive ? lhs == rhs : CharUtils<TextChar>::ToLower(lhs) == CharUtils<TextChar>::ToLower(rhs);
        }

        /**
         * \brief Compares two character buffers for equality
         *
         * \param lhs Pointer to the first buffer=
         * \param rhs Pointer to the second buffer=
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if both sequences are exactly equal
         */
        static bool Equal(const CharType* lhs, const CharType* rhs, bool caseSensitive = true)
        {
            return Equal(lhs, GetLength(lhs), rhs, GetLength(rhs), caseSensitive);
        }

        /**
         * \brief Compares two character buffers for equality
         *
         * \param lhs Pointer to the first buffer
         * \param lhsLength Number of characters in the first buffer
         * \param rhs Pointer to the second buffer
         * \param rhsLength Number of characters in the second buffer
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if both sequences are exactly equal
         */
        static bool Equal(const CharType* lhs, SizeType lhsLength, const CharType* rhs, SizeType rhsLength, bool caseSensitive = true)
        {
            return Compare(lhs, lhsLength, rhs, rhsLength, caseSensitive) == 0;
        }

        /**
         * \brief Compares two character sequences lexicographically
         *
         * \param lhs Pointer to the first buffer
         * \param rhs Pointer to the second buffer
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return 0 if equal, negative if lhs < rhs, positive if lhs > rhs.
         */
        static int Compare(const CharType* lhs, const CharType* rhs, bool caseSensitive = true)
        {
            return Compare(lhs, GetLength(lhs), rhs, GetLength(rhs), caseSensitive);
        }

        /**
         * \brief Compares two character sequences lexicographically
         *
         * \param lhs Pointer to the first buffer
         * \param lhsLength Length of the first buffer
         * \param rhs Pointer to the second buffer
         * \param rhsLength Length of the second buffer
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return 0 if equal, negative if lhs < rhs, positive if lhs > rhs.
         */
        static int Compare(const CharType* lhs, SizeType lhsLength, const CharType* rhs, SizeType rhsLength, bool caseSensitive = true)
        {
            SizeType minLen = std::min(lhsLength, rhsLength);

            for (SizeType i = 0; i < minLen; ++i)
            {
                if (!Equal(lhs[i], rhs[i], caseSensitive))
                    return lhs[i] < rhs[i] ? -1 : 1;
            }

            if (lhsLength < rhsLength)
                return -1;

            if (lhsLength > rhsLength)
                return 1;

            return 0;
        }

        /**
         * \brief Computes the length of a null-terminated character string
         *
         * \param string Pointer to the sequence terminated by '\\0'
         *
         * \return The number of characters before the null terminator
         */
        static SizeType GetLength(const CharType* string)
        {
            assert(string != nullptr);

            SizeType length = 0;
            while (string[length] != '\0')
                ++length;

            return length;
        }

        /**
         * \brief Finds the first occurrence of a character
         *
         * \param haystack Pointer to the character buffer
         * \param length Length of the buffer
         * \param needle The character to find
         * \param startIndex Index to begin searching from
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindFirst(const CharType* haystack, SizeType length, CharType needle, SizeType startIndex = 0, bool caseSensitive = true)
        {
            assert(haystack && needle);

            for (SizeType i = startIndex; i < length; i++)
            {
                if (Equal(haystack[i], needle, caseSensitive))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the first occurrence of a substring
         *
         * \param haystack Pointer to the character buffer
         * \param needle Pointer to the substring buffer
         * \param startIndex Index to begin searching from
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindFirst(const CharType* haystack, const CharType* needle, SizeType startIndex = 0, bool caseSensitive = true)
        {
            return FindFirst(haystack, GetLength(haystack), needle, GetLength(needle), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a substring
         *
         * \param haystack Pointer to the character buffer
         * \param haystackLength Length of the buffer
         * \param needle Pointer to the substring buffer
         * \param needleLength Length of the substring
         * \param startIndex Index to begin searching from
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindFirst(const CharType* haystack, SizeType haystackLength, const CharType* needle, SizeType needleLength, SizeType startIndex = 0, bool caseSensitive = true)
        {
            assert(haystack && needle);

            if (needleLength == 0 || needleLength > haystackLength + startIndex)
                return Optional<SizeType>::Empty();

            for (SizeType i = startIndex; i <= haystackLength - needleLength; ++i)
            {
                SizeType j = 0;
                while (j < needleLength && Equal(haystack[i + j], needle[j], caseSensitive))
                    ++j;

                if (j != needleLength)
                    continue;

                return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the last occurrence of a character
         *
         * \param haystack Pointer to the buffer to search in
         * \param length Length of the buffer
         * \param needle Character to find
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindLast(const CharType* haystack, SizeType length, CharType needle, bool caseSensitive = true)
        {
            assert(haystack && needle);

            for (SizeType i = length - 1; i >= 0; --i)
            {
                if (Equal(haystack[i], needle, caseSensitive))
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Finds the last occurrence of a substring
         *
         * \param haystack Pointer to the buffer to search in
         * \param needle Pointer to the substring
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindLast(const CharType* haystack, const CharType* needle, bool caseSensitive = true)
        {
            return FindLast(haystack, GetLength(haystack), needle, GetLength(needle), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a substring
         *
         * \param haystack Pointer to the buffer to search in
         * \param haystackLength Length of haystack buffer
         * \param needle Pointer to the substring
         * \param needleLength Length of substring
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return Index wrapped in Optional, or Optional::Empty() if not found
         */
        static Optional<SizeType> FindLast(const CharType* haystack, SizeType haystackLength, const CharType* needle, SizeType needleLength, bool caseSensitive = true)
        {
            assert(haystack && needle);

            if (needleLength == 0 || needleLength > haystackLength)
                return Optional<SizeType>::Empty();

            SizeType i = haystackLength - needleLength;

            while (true)
            {
                SizeType j = 0;
                while (j < needleLength && Equal(haystack[i + j], needle[j], caseSensitive))
                    ++j;

                if (j == needleLength)
                    return Optional<SizeType>::Of(i);

                if (i == 0)
                    break;

                --i;
            }

            return Optional<SizeType>::Empty();
        }

        /**
         * \brief Determines whether a sequence begins with a given prefix
         *
         * \param haystack Pointer to the sequence
         * \param needle Pointer to prefix buffer
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if haystack starts with needle.
         */
        static bool StartsWith(const CharType* haystack, const CharType* needle, bool caseSensitive = true)
        {
            return StartsWith(haystack, GetLength(haystack), needle, GetLength(needle), caseSensitive);
        }

        /**
         * \brief Determines whether a sequence begins with a given prefix
         *
         * \param haystack Pointer to the sequence
         * \param haystackLength Length of the sequence
         * \param needle Pointer to prefix buffer
         * \param needleLength Length of prefix
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if haystack starts with needle.
         */
        static bool StartsWith(const CharType* haystack, SizeType haystackLength, const CharType* needle, SizeType needleLength, bool caseSensitive = true)
        {
            if (haystackLength < needleLength)
                return false;

            for (SizeType i = 0; i < needleLength; i++)
            {
                if (!Equal(haystack[i], needle[i], caseSensitive))
                    return false;
            }

            return true;
        }

        /**
         * \brief Determines whether a sequence ends with a given suffix
         *
         * \param haystack Pointer to the sequence
         * \param needle Pointer to suffix buffer
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if haystack ends with needle.
         */
        static bool EndsWith(const CharType* haystack, const CharType* needle, bool caseSensitive = true)
        {
            return EndsWith(haystack, GetLength(haystack), needle, GetLength(needle), caseSensitive);
        }

        /**
         * \brief Determines whether a sequence ends with a given suffix
         *
         * \param haystack Pointer to the sequence
         * \param haystackLength Length of the sequence
         * \param needle Pointer to suffix buffer
         * \param needleLength Length of suffix
         * \param caseSensitive If false, case of letters is ignored
         *
         * \return True if haystack ends with needle.
         */
        static bool EndsWith(const CharType* haystack, SizeType haystackLength, const CharType* needle, SizeType needleLength, bool caseSensitive = true)
        {
            if (haystackLength < needleLength)
                return false;

            for (SizeType i = 0; i < needleLength; i++)
            {
                if (!Equal(haystack[haystackLength - needleLength + i], needle[i], caseSensitive))
                    return false;
            }

            return true;
        }

        static bool IsPureAnsi(const CharType* string)
        {
            return IsPureAnsi(string, GetLength(string));
        }

        static bool IsPureAnsi(const CharType* string, SizeType length)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return true;
            }
            else
            {
                for (SizeType i = 0; i < length; ++i)
                {
                    if (string[i] > 127)
                        return false;
                }

                return true;
            }
        }

        /**
         * \brief Tell whether a string contains only numeric characters
         * 
         * \param string The string to check
         * \param length Length of the \p string
         *
         * \return True if \p string contains only numeric characters, false otherwise
         */
        static bool IsNumeric(const CharType* string, SizeType length)
        {
            bool isRelative = false;
            const bool startsWithSight = string[0] == '-' || string[0] == '+';
            for (SizeType i = startsWithSight ? 1 : 0; i < length; ++i)
            {
                bool isNumeric = CharUtils<CharType>::IsDigit(string[i]);

                if (string[i] == '.')
                {
                    if (isRelative)
                        return false;

                	isRelative = true;
                }
                else if (!isNumeric)
                {
                    return false;
                }
            }

            return true;
        }

        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        static Optional<T> TryToInteger(const CharType* string, int base = 0)
        {
            using Unsigned = std::make_unsigned_t<T>;

            while (CharUtils<CharType>::IsBlank(*string))
                ++string;

            bool negative = false;
            if (*string == CharType('+') || *string == CharType('-'))
            {
                if (*string == CharType('-'))
                    negative = true;

                ++string;
            }

            if (base == 0)
            {
                if (string[0] == CharType('0'))
                {
                    if (CharUtils<CharType>::ToLower(string[1]) == CharType('x'))
                    {
                        base = 16;
                        string += 2;
                    }
                    else
                    {
                        base = 8;
                        ++string;
                    }
                }
                else
                {
                    base = 10;
                }
            }

            Unsigned result = 0;
            const Unsigned limit = std::numeric_limits<Unsigned>::max() / base;
            for (; *string; ++string)
            {
                int digit = CharUtils<CharType>::DigitValue(*string);
                if (digit < 0 || digit >= base)
                    break;

                if (result > limit || (result == limit && Unsigned(digit) > std::numeric_limits<Unsigned>::max() % base))
                    return Optional<T>::Empty();

                result = result * base + digit;
            }

            return Optional<T>::Of(negative ? -static_cast<T>(result) : static_cast<T>(result));
        }

        template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
        static Optional<T> TryToFloatingPoint(const CharType* string)
        {
            if constexpr (std::is_same_v<T, float>)
            {
                if constexpr (std::is_same_v<CharType, AnsiChar>)
                {
                    AnsiChar* end;
                    T value = std::strtof(string, &end);
                    if (end == string)
                        return Optional<T>::Empty();

                    return Optional<T>::Of(value);
                }
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                if constexpr (std::is_same_v<CharType, AnsiChar>)
                {
                    AnsiChar* end;
                    T value = std::strtod(string, &end);
                    if (end == string)
                        return Optional<T>::Empty();

                    return Optional<T>::Of(value);
                }
            }

            return Optional<T>::Empty();
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_CHARACTERS_HPP
