#ifndef COCKTAIL_CORE_UTILITY_CHARUTILS_HPP
#define COCKTAIL_CORE_UTILITY_CHARUTILS_HPP

#include <cctype>
#include <cwctype>

#include <Cocktail/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief Utility class for character type operations
     *
     * This template class provides type-safe, character-related utility functions
     * for multiple character types (AnsiChar, WildChar, Utf8Char). It offers
     * static methods to test whether a character is a digit or alphanumeric.
     *
     * \tparam TChar The character type. Must be one of: AnsiChar, WildChar, or Utf8Char
     */
    template <typename TChar>
    class CharUtils
    {
    public:

        /**
         * \brief Alias for the template character type
         */
        using CharType = TChar;

        static_assert(std::is_same_v<CharType, AnsiChar> || std::is_same_v<CharType, WildChar>|| std::is_same_v<CharType, Utf8Char>);

        /**
         * \brief Checks if the given character is alphanumeric (letter or digit)
         *
         * \param character The character to test
         *
         * \return true if the character is alphanumeric, false otherwise
         */
        static bool IsAlphaNumeric(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isalnum(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswalnum(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isalnum(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a letter
         *
         * \param character The character to test
         *
         * \return true if the character is a letter, false otherwise
         */
        static bool IsAlpha(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isalpha(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswalpha(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isalpha(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a blank
         *
         * \param character The character to test
         *
         * \return true if the character is a blank, false otherwise
         */
        static bool IsBlank(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isblank(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswblank(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isblank(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a control character
         *
         * \param character The character to test
         *
         * \return true if the character is a control character, false otherwise
         */
        static bool IsControl(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::iscntrl(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswcntrl(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::iscntrl(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a digit (0–9)
         *
         * \param character The character to test
         *
         * \return true if the character is a digit, false otherwise
         */
        static bool IsDigit(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isdigit(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswdigit(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isdigit(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is graphic
         *
         * \param character The character to test
         *
         * \return true if the character is graphic, false otherwise
         */
        static bool IsGraphic(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isgraph(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswgraph(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isgraph(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a lowercase letter
         *
         * \param character The character to test
         *
         * \return true if the character is a lowercase letter, false otherwise
         */
        static bool IsLower(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::islower(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswlower(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::islower(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is printable
         *
         * \param character The character to test
         *
         * \return true if the character is printable, false otherwise
         */
        static bool IsPrintable(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isprint(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswprint(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isprint(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a punctuation character
         *
         * \param character The character to test
         *
         * \return true if the character is a punctuation character, false otherwise
         */
        static bool IsPunctuation(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::ispunct(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswpunct(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::ispunct(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a space character
         *
         * \param character The character to test
         *
         * \return true if the character is a space character, false otherwise
         */
        static bool IsSpace(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isspace(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswspace(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isspace(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is an uppercase letter
         *
         * \param character The character to test
         *
         * \return true if the character is an uppercase letter, false otherwise
         */
        static bool IsUpper(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isupper(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswupper(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isupper(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Checks if the given character is a hexadecimal digit character
         *
         * \param character The character to test
         *
         * \return true if the character is a hexadecimal digit character, false otherwise
         */
        static bool IsHexadecimalDigit(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::isxdigit(static_cast<int>(character)) != 0;
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::iswxdigit(character) != 0;
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::isxdigit(static_cast<int>(character)) != 0;
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Transform an uppercase letter to its lowercase counterpart
         *
         * If the \p character is not a letter, the returned value is \p character unchanged.
         *
         * \param character The character to transform in lowercase
         *
         * \return The lowercase representation
         */
        static CharType ToLower(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::tolower(static_cast<int>(character));
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::towlower(character);
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::tolower(static_cast<int>(character));
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Transform a lowercase letter to its uppercase counterpart
         *
         * If the \p character is not a letter, the returned value is \p character unchanged.
         *
         * \param character The character to transform in uppercase
         *
         * \return The uppercase representation
         */
        static CharType ToUpper(CharType character)
        {
            if constexpr (std::is_same_v<CharType, AnsiChar>)
            {
                return ::toupper(static_cast<int>(character));
            }
            else if constexpr (std::is_same_v<CharType, WildChar>)
            {
                return ::towupper(character);
            }
            else if constexpr (std::is_same_v<CharType, Utf8Char>)
            {
                return ::toupper(static_cast<int>(character));
            }

            COCKTAIL_UNREACHABLE();
        }

        /**
         * \brief Convert a digit character to its real value
         *
         * This function also handle hexadecimal digits.
         * If \p character is not a digit char or a hexadecimal digit letter, the returned value is -1.
         *
         * \param character The character to convert
         *
         * \return The value of the character or -1
         */
        static int DigitValue(CharType character)
        {
            if (character >= CharType('0') && character <= CharType('9'))
                return character - CharType('0');

            if (character >= CharType('a') && character <= CharType('f'))
                return character - CharType('a') + 10;

            if (character >= CharType('A') && character <= CharType('F'))
                return character - CharType('A') + 10;

            return -1;
        }
    };
}

#endif // COCKTAIL_CORE_UTILITY_CHARUTILS_HPP
