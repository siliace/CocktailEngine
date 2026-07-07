#ifndef COCKTAIL_CORE_STRING_HPP
#define COCKTAIL_CORE_STRING_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/Encoding/Encoders.hpp>
#include <Cocktail/Core/Utility/StringFormater.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
    template <typename, typename>
    class BasicString;

    /**
     * \brief A lightweight, non-owning view over a character sequence encoded with a given encoding
     *
     * BasicStringView provides read-only access to a contiguous sequence of characters without
     * owning the underlying data. It is suitable for efficient parameter passing and substring
     * operations without heap allocation.
     *
     * \tparam TEncoding The encoding type defining the character type and size type
     */
    template <typename TEncoding>
    class BasicStringView
    {
    public:

        /**
         * \brief The encoding type used by this string view
         */
        using EncodingType = TEncoding;

        /**
         * \brief The character type used by this string view, as defined by the encoding
         */
        using CharType = typename EncodingType::CharType;

        /**
         * \brief The size type used to represent the length of this string view
         */
        using SizeType = typename EncodingType::SizeType;

        /**
         * \brief Utility constant representing an empty string view
         */
        static const BasicStringView Empty;

        /**
         * \brief Default constructor
         *
         * Creates an empty string view pointing to no data.
         */
        constexpr BasicStringView() :
            mString(nullptr),
            mLength(0)
        {
            /// Nothing
        }

        /**
         * \brief Constructs a string view from a BasicString
         *
         * Creates a non-owning view referencing the data held by \p string.
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param string The BasicString to create a view from
         */
        template <typename TAllocator>
        BasicStringView(const BasicString<TEncoding, TAllocator>& string) :
            BasicStringView(string.GetData(), string.GetLength())
        {
            /// Nothing
        }

        /**
         * \brief Constructs a string view from a null-terminated character sequence
         *
         * Creates a view referencing the characters pointed to by \p string.
         * The length is automatically determined by scanning for the null terminator.
         *
         * \param string A pointer to a null-terminated character array
         */
        constexpr BasicStringView(const CharType* string) :
            BasicStringView(string, StringUtils<CharType, SizeType>::GetLength(string))
        {
            /// Nothing
        }

        /**
         * \brief Constructs a string view from a character pointer and a length
         *
         * Creates a view referencing exactly \p length characters starting from \p string.
         *
         * \param string A pointer to the character data
         * \param length The number of characters in the view
         */
        constexpr BasicStringView(const CharType* string, SizeType length) :
            mString(string),
            mLength(length)
        {
            /// Nothing
        }

        /**
         * \brief Assigns a BasicString to this string view
         *
         * Replaces the current view with one referencing the data held by \p string.
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param string The BasicString to reference
         *
         * \return A copy of this BasicStringView after assignment
         */
        template <typename TAllocator>
        BasicStringView operator=(const BasicString<TEncoding, TAllocator>& string)
        {
            mString = string.GetData();
            mLength = string.GetLength();

            return *this;
        }

        /**
         * \brief Assigns a null-terminated character sequence to this string view
         *
         * Replaces the current view with one referencing the characters pointed to by \p string.
         * The length is automatically determined by scanning for the null terminator.
         *
         * \param string A pointer to a null-terminated character array
         *
         * \return A copy of this BasicStringView after assignment
         */
        BasicStringView operator=(const CharType* string)
        {
            mString = string;
            mLength = StringUtils<CharType, SizeType>::GetLength(string);

            return *this;
        }

        /**
         * \brief Returns a const reference to the character at the given index
         *
         * Provides read-only access to the character at position \p index.
         * If \p index is out of range, an OutOfRange exception is thrown.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return A const reference to the character at the specified index
         *
         * \throws OutOfRange If \p index is outside the valid range
         */
        const CharType& At(SizeType index) const
        {
            if (index >= mLength)
                ExceptionUtils::ThrowOutOfRange(index, mLength);

            return mString[index];
        }

        /**
         * \brief Attempts to access the character at the given index
         *
         * Returns an Optional containing a reference to the character at \p index if the index
         * is within bounds. If \p index is out of range, an empty Optional is returned instead.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return An Optional containing a reference to the character if valid,
         *         or an empty Optional otherwise
         */
        Optional<CharType&> TryAt(SizeType index)
        {
            if (index >= mLength)
                Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(mString[index]);
        }

        /**
         * \brief Attempts to access the character at the given index (const)
         *
         * Returns an Optional containing a const reference to the character at \p index if the index
         * is within bounds. If \p index is out of range, an empty Optional is returned instead.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return An Optional containing a const reference to the character if valid,
         *         or an empty Optional otherwise
         */
        Optional<const CharType&> TryAt(SizeType index) const
        {
            if (index >= mLength)
                Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(mString[index]);
        }

        /**
         * \brief Returns a reference to the first character in the view
         *
         * Provides direct access to the first character.
         * If the view is empty, an exception is thrown.
         *
         * \return A reference to the first character
         *
         * \throws Exception If the view is empty
         */
        CharType& First()
        {
            if (Optional<CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        /**
         * \brief Returns a const reference to the first character in the view
         *
         * Provides read-only access to the first character.
         * If the view is empty, an exception is thrown.
         *
         * \return A const reference to the first character
         *
         * \throws Exception If the view is empty
         */
        const CharType& First() const
        {
            if (Optional<const CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        /**
         * \brief Attempts to access the first character
         *
         * Returns an Optional containing a reference to the first character if the view
         * is not empty. Returns an empty Optional if the view is empty.
         *
         * \return An Optional containing a reference to the first character,
         *         or empty if the view is empty
         */
        Optional<CharType&> TryFirst()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(0));
        }

        /**
         * \brief Attempts to access the first character (const)
         *
         * Returns an Optional containing a const reference to the first character if the view
         * is not empty. Returns an empty Optional if the view is empty.
         *
         * \return An Optional containing a const reference to the first character,
         *         or empty if the view is empty
         */
        Optional<const CharType&> TryFirst() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(0));
        }

        /**
         * \brief Returns a reference to the last character in the view
         *
         * Provides direct access to the last character.
         * If the view is empty, an exception is thrown.
         *
         * \return A reference to the last character
         *
         * \throws Exception If the view is empty
         */
        CharType& Last()
        {
            if (Optional<CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        /**
         * \brief Returns a const reference to the last character in the view
         *
         * Provides read-only access to the last character.
         * If the view is empty, an exception is thrown.
         *
         * \return A const reference to the last character
         *
         * \throws Exception If the view is empty
         */
        const CharType& Last() const
        {
            if (Optional<const CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        /**
         * \brief Attempts to access the last character
         *
         * Returns an Optional containing a reference to the last character if the view
         * is not empty. Returns an empty Optional if the view is empty.
         *
         * \return An Optional containing a reference to the last character,
         *         or empty if the view is empty
         */
        Optional<CharType&> TryLast()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(GetLength() - 1));
        }

        /**
         * \brief Attempts to access the last character (const)
         *
         * Returns an Optional containing a const reference to the last character if the view
         * is not empty. Returns an empty Optional if the view is empty.
         *
         * \return An Optional containing a const reference to the last character,
         *         or empty if the view is empty
         */
        Optional<const CharType&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(GetLength() - 1));
        }

        /**
         * \brief Finds the first occurrence of a character in this view
         *
         * Searches for the first occurrence of \p search starting from \p startIndex.
         *
         * \param search The character to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(CharType search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mString, mLength, search, startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a string view in this view
         *
         * Searches for the first occurrence of the sequence \p search starting from \p startIndex.
         *
         * \param search The string view to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(BasicStringView search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a BasicString in this view
         *
         * Searches for the first occurrence of the sequence in \p search starting from \p startIndex.
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param search The BasicString to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        template <typename TAllocator>
        Optional<SizeType> FindFirst(const BasicString<TEncoding, TAllocator>& search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a null-terminated character sequence in this view
         *
         * Searches for the first occurrence of the null-terminated sequence \p search
         * starting from \p startIndex. The length is determined automatically.
         *
         * \param search A pointer to the null-terminated character sequence to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(const CharType* search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search, StringUtils<CharType, SizeType>::GetLength(search), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a character sequence in this view
         *
         * Searches for the first occurrence of exactly \p length characters from \p search
         * starting from \p startIndex.
         *
         * \param search A pointer to the character sequence to search for
         * \param length The length of the character sequence to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(const CharType* search, SizeType length, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mString, mLength, search, length, startIndex, caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a character in this view
         *
         * Searches for the last occurrence of \p search in the entire view.
         *
         * \param search The character to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(CharType search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mString, mLength, search, caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a string view in this view
         *
         * Searches for the last occurrence of the sequence \p search.
         *
         * \param search The string view to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(BasicStringView search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a BasicString in this view
         *
         * Searches for the last occurrence of the sequence in \p search.
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param search The BasicString to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        template <typename TAllocator>
        Optional<SizeType> FindLast(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a null-terminated character sequence in this view
         *
         * Searches for the last occurrence of the null-terminated sequence \p search.
         * The length is determined automatically.
         *
         * \param search A pointer to the null-terminated character sequence to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(const CharType* search, bool caseSensitive = true) const
        {
            return FindLast(search, StringUtils<CharType, SizeType>::GetLength(search), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a character sequence in this view
         *
         * Searches for the last occurrence of exactly \p length characters from \p search.
         *
         * \param search A pointer to the character sequence to search for
         * \param length The length of the character sequence to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(const CharType* search, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mString, mLength, search, length, caseSensitive);
        }

        /**
         * \brief Get the number of UTF codepoint available in this String
         * The number of codepoint may vary depending on the decoder used internally.
         * If the String type does not support surrogates, this function is the same as calling GetLength.
         *
         * \return The number of code point
         */
        SizeType CodepointCount() const
        {
            return IsEmpty() ? 0 : EncodingType::GetCharCount(GetData(), GetLength());
        }

        /**
         * \brief Get the n-th codepoint encoded in this string
         *
         * \param codepointIndex The index of the codepoint to get
         *
         * \return The codepoint
         *
         * \throw OutOfRange if \c codepointIndex is greater than CodepointCount()
         */
        Utf32Char CodepointAt(SizeType codepointIndex) const
        {
            if (auto codepoint = TryCodepointAt(codepointIndex); !codepoint.IsEmpty())
                return codepoint.Get();

            ExceptionUtils::ThrowOutOfRange(0, CodepointCount());
        }

        /**
         * \brief Attempts to retrieve the UTF-32 codepoint at the specified index
         *
         * Returns an Optional containing the codepoint at \p codepointIndex if it exists
         * and can be decoded. If the view is empty or the index is invalid, an empty Optional
         * is returned.
         *
         * \param codepointIndex The zero-based index of the codepoint to retrieve
         *
         * \return An Optional containing the UTF-32 codepoint if valid, or empty otherwise
         */
        Optional<Utf32Char> TryCodepointAt(SizeType codepointIndex) const
        {
            if (IsEmpty())
                return Optional<Utf32Char>::Empty();

            Optional<SizeType> offset = TryOffsetByCodepoint(0, codepointIndex);
            if (offset.IsEmpty())
                return Optional<Utf32Char>::Empty();

            Utf32Char out;
            if (EncodingType::Decode(GetData() + offset.Get(), GetLength() - offset.Get(), out) == 0)
                return Optional<Utf32Char>::Empty();

            return Optional<Utf32Char>::Of(out);
        }

        /**
         * \brief Returns the character offset corresponding to a number of codepoints
         *
         * Calculates the character offset in the view after moving \p codepointCount
         * codepoints starting from position \p start. If the resulting offset is invalid,
         * an exception is thrown.
         *
         * \param start The starting character index
         * \param codepointCount The number of codepoints to advance
         *
         * \return The character offset after advancing \p codepointCount codepoints
         *
         * \throws OutOfRange If the resulting offset is outside the valid range
         */
        SizeType OffsetByCodepoint(SizeType start, SizeType codepointCount) const
        {
            if (auto offset = TryOffsetByCodepoint(start, codepointCount); !offset.IsEmpty())
                return offset.Get();

            ExceptionUtils::ThrowOutOfRange(start, GetLength());
        }

        /**
         * \brief Attempts to compute the character offset after a number of codepoints
         *
         * Returns an Optional containing the character offset obtained by advancing
         * \p codepointCount codepoints starting from \p start. If an invalid codepoint
         * or end-of-view is reached, an empty Optional is returned.
         *
         * \param start The starting character index
         * \param codepointCount The number of codepoints to advance
         *
         * \return An Optional containing the resulting character offset, or empty if invalid
         */
        Optional<SizeType> TryOffsetByCodepoint(SizeType start, SizeType codepointCount) const
        {
            SizeType charOffset = start;
            for (SizeType i = 0; i < codepointCount; ++i)
            {
                SizeType charCount = EncodingType::GetCharCount(GetData() + charOffset);
                if (charCount == 0)
                    Optional<SizeType>::Empty();

                charOffset += charCount;
            }

            return Optional<SizeType>::Of(charOffset);
        }

        /**
         * \brief Get all codepoints of the string
         *
         * Construct an Array of filled by every codepoints encoded in this string.
         *
         * \return The array of codepoint
         */
        Array<Utf32Char> GetCodepoints() const
        {
            Array<Utf32Char> codepoints;
            codepoints.Reserve(CodepointCount());

            Utf32Char codepoint;
            for (SizeType i = 0; i < GetLength(); i += EncodingType::Decode(GetData() + i, GetLength() - i, codepoint))
                codepoints.Add(codepoint);

            return codepoints;
        }

        /**
         * \brief Returns a sub-view starting from the given index to the end
         *
         * Creates a new BasicStringView referencing the portion of this view
         * starting at \p start and extending to the end.
         *
         * \param start The zero-based starting index of the sub-view
         *
         * \return A BasicStringView over the requested range
         */
        BasicStringView SubStringView(SizeType start) const
        {
            return SubStringView(start, GetLength() - start);
        }

        /**
         * \brief Returns a sub-view starting from the given index with a specified length
         *
         * Creates a new BasicStringView referencing the portion of this view
         * starting at \p start and spanning at most \p length characters.
         * If \p length exceeds the remaining characters, the view is clamped.
         *
         * \param start The zero-based starting index of the sub-view
         * \param length The maximum number of characters to include
         *
         * \return A BasicStringView over the requested range
         */
        BasicStringView SubStringView(SizeType start, SizeType length) const
        {
            return View(GetData() + start, std::min(length, GetLength() - start));
        }

        /**
         * \brief Compares this view with another string view
         *
         * Performs a lexicographic comparison between this view and \p other.
         *
         * \param other The string view to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this view is less than \p other,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const BasicStringView& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Compares this view with a BasicString
         *
         * Performs a lexicographic comparison between this view and \p other.
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param other The BasicString to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this view is less than \p other,
         *         zero if they are equal, or a positive value if greater
         */
        template <typename TAllocator>
        int Compare(const BasicString<TEncoding, TAllocator>& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Compares this view with a null-terminated character sequence
         *
         * Performs a lexicographic comparison between this view and \p string.
         * The length of \p string is determined automatically.
         *
         * \param string A pointer to the null-terminated character sequence to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this view is less than \p string,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const CharType* string, bool caseSensitive = true) const
        {
            return Compare(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        /**
         * \brief Compares this view with a character sequence of given length
         *
         * Performs a lexicographic comparison between this view and \p length characters
         * starting from \p string.
         *
         * \param string A pointer to the character sequence to compare against
         * \param length The number of characters to compare
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this view is less than the sequence,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::Compare(mString, length, string, length, caseSensitive);
        }

        /**
         * \brief Checks whether this view starts with a given character
         *
         * \param search The character to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view starts with \p search, false otherwise
         */
        bool StartsWith(CharType search, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mString[0], search, caseSensitive);
        }

        /**
         * \brief Checks whether this view starts with a given string view
         *
         * \param search The string view to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view starts with \p search, false otherwise
         */
        bool StartsWith(BasicStringView search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this view starts with a given BasicString
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param search The BasicString to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view starts with \p search, false otherwise
         */
        template <typename TAllocator>
        bool StartsWith(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this view starts with a given character sequence
         *
         * \param string A pointer to the character sequence to check for at the beginning
         * \param length The length of the character sequence
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view starts with the given sequence, false otherwise
         */
        bool StartsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, string, length, caseSensitive);
        }

        /**
         * \brief Checks whether this view ends with a given character
         *
         * \param search The character to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view ends with \p search, false otherwise
         */
        bool EndsWith(CharType search, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mString[mLength - 1], search, caseSensitive);
        }

        /**
         * \brief Checks whether this view ends with a given string view
         *
         * \param search The string view to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view ends with \p search, false otherwise
         */
        bool EndsWith(BasicStringView search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this view ends with a given BasicString
         *
         * \tparam TAllocator The allocator type used by the source string
         *
         * \param search The BasicString to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view ends with \p search, false otherwise
         */
        template <typename TAllocator>
        bool EndsWith(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this view ends with a given character sequence
         *
         * \param string A pointer to the character sequence to check for at the end
         * \param length The length of the character sequence
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the view ends with the given sequence, false otherwise
         */
        bool EndsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mString, mLength, string, length, caseSensitive);
        }

        /**
         * \brief Checks whether this view is empty
         *
         * \return True if the view has a length of zero, false otherwise
         */
        bool IsEmpty() const
        {
            return mLength == 0;
        }

        /**
         * \brief Get the number of character available in this StringView
         * If the StringView type supports surrogates, the number of characters encoded might be different
         * as the number of character available.
         *
         * \return The number of character
         */
        SizeType GetLength() const
        {
            return mLength;
        }

        /**
         * \brief Returns a pointer to the underlying character data
         *
         * Provides direct access to the raw character buffer referenced by this view.
         * The returned pointer may be null if the view was default-constructed.
         *
         * \return A pointer to the first character in the view, or nullptr if empty
         */
        const CharType* GetData() const
        {
            return mString;
        }

        /**
         * \brief Returns a const reference to the character at the given index without bounds checking
         *
         * Provides direct read-only access to the character at position \p index.
         * No range checking is performed.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return A const reference to the character at the specified index
         */
        const CharType& operator[](SizeType index) const
        {
            return mString[index];
        }

    private:

        const CharType* mString;
        SizeType mLength;
    };

    template <typename TEncoding>
    const BasicStringView<TEncoding> BasicStringView<TEncoding>::Empty;

    template <typename TEncoding>
    bool operator==(BasicStringView<TEncoding> lhs, BasicStringView<TEncoding> rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    template <typename TEncoding>
    bool operator==(BasicStringView<TEncoding> lhs, const typename TEncoding::CharType* rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    template <typename TEncoding>
    bool operator==(const typename TEncoding::CharType* lhs, BasicStringView<TEncoding> rhs)
    {
        return rhs.Compare(lhs) == 0;
    }

    template <typename TEncoding>
    bool operator!=(BasicStringView<TEncoding> lhs, BasicStringView<TEncoding> rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding>
    bool operator!=(BasicStringView<TEncoding> lhs, const typename TEncoding::CharType* rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding>
    bool operator!=(const typename TEncoding::CharType* lhs, BasicStringView<TEncoding> rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * \brief A flexible, template-based string class supporting various character types, encoders, and allocators
     *
     * The BasicString class provides a generic and efficient representation of text data.
     * It is designed to work with different character types (e.g., char, wchar_t, char16_t),
     * encoders (for handling UTF conversions), and custom memory allocators.
     *
     * \tparam TEncoding The encoder template used to convert between raw string data and UTF codepoints
     * \tparam TAllocator The memory allocator used for internal storage
     */
    template <typename TEncoding, typename TAllocator = HeapAllocator>
    class BasicString
    {
        friend class Encoders;

    public:

        /**
         * \brief The encoder type used to convert raw string data from and to UTF codepoints
         */
        using EncodingType = TEncoding;

        /**
         * \brief The character type used by this string
         */
        using CharType = typename EncodingType::CharType;

        /**
         * \brief The allocator type used to store character sequences
         */
        using AllocatorType = TAllocator;

        /**
         * \brief The size type used to represent the length or capacity of the string.
         */
        using SizeType = typename Array<CharType, AllocatorType>::SizeType;

        /**
         * \brief Flag indicating whether this string type supports surrogate pairs
         *
         * If true, the encoder supports handling surrogate pairs (e.g., in UTF-16 encoding),
         * allowing representation of characters outside the Basic Multilingual Plane (BMP).
         */
        static constexpr bool SupportSurrogate = EncodingType::SupportSurrogate;

        /**
         * \brief A lightweight, non-owning view type referencing string data
         *
         * The View type allows read-only access to string content without copying or
         * transferring ownership. It is useful for efficient substring operations and
         * parameter passing.
         */
        using View = BasicStringView<TEncoding>;

        /**
         * \brief Creates a formatted string from a format pattern and arguments
         *
         * Constructs a new BasicString by applying the format pattern \p format with
         * the provided arguments. The formatting rules depend on the StringFormater implementation.
         *
         * \tparam Args The types of the format arguments
         *
         * \param format The format pattern as a string view
         * \param args The arguments to substitute into the format pattern
         *
         * \return A new BasicString containing the formatted result
         */
        template <typename... Args>
        static BasicString Format(View format, Args&&... args)
        {
            BasicString string;
            StringFormater<CharType>::FormatTo(string.mCharacters, format.GetData(), std::forward<Args>(args)...);

            return string;
        }

        /**
         * \brief Converts a string with a different character or encoder type into this string type
         *
         * This function accepts a pointer to a string encoded with a different
         * encoder and character type, decodes each codepoint to UTF-32, and re-encodes it using
         * this string’s encoder. The resulting text is stored in a new String instance.
         *
         * \tparam TOtherAllocator The allocator type used by the source string
         *
         * \param other The source string to convert from
         *
         * \return A new string instance containing the converted text
         */
        template <typename TOtherEncoding, typename TOtherAllocator>
        static BasicString Convert(const BasicString<TOtherEncoding, TOtherAllocator>& other)
        {
            if (other.IsEmpty())
                return Empty;

            return Convert(typename BasicString<TOtherEncoding, TOtherAllocator>::View(other));
        }

        /**
         * \brief Converts a string view with a different character or encoder type into this string type
         *
         * This function accepts a pointer to a string view encoded with a different
         * encoder and character type, decodes each codepoint to UTF-32, and re-encodes it using
         * this string’s encoder. The resulting text is stored in a new String instance.
         *
         * \tparam TOtherEncoding The encoder template used by the source view
         *
         * \param other The source BasicStringView to convert from
         *
         * \return A new BasicString instance containing the converted text
         */
        template <typename TOtherEncoding>
        static BasicString Convert(const BasicStringView<TOtherEncoding>& other)
        {
            if (other.IsEmpty())
                return Empty;

            return ConvertFrom<TOtherEncoding>(other.GetData(), other.GetLength());
        }

        /**
         * \brief Converts a null-terminated raw string from another encoding into this string type
         *
         * This function accepts a pointer to a null-terminated string encoded with a different
         * encoder and character type, decodes each codepoint to UTF-32, and re-encodes it using
         * this string’s encoder. The resulting text is stored in a new String instance.
         *
         * This method is particularly useful for converting between different Unicode representations,
         * such as from UTF-8 to UTF-16 or UTF-32.
         *
         * \tparam TOtherEncoder The encoder type used by the source string
         *
         * \param other A pointer to the null-terminated source string to convert
         *
         * \return A new String instance containing the converted text
         *
         * \note This function assumes that the input string is properly null-terminated
         */
        template <typename TOtherEncoder>
        static BasicString ConvertFrom(const typename TOtherEncoder::CharType* other)
        {
            using OtherEncoder = TOtherEncoder;

            using OtherCharType = typename OtherEncoder::CharType;
            using OtherSizeType = typename OtherEncoder::SizeType;

            if (other == nullptr)
                return Empty;

            return ConvertFrom<OtherEncoder>(other, StringUtils<OtherCharType, OtherSizeType>::GetLength(other));
        }

        /**
         * \brief Converts a raw string from another encoding into this string type
         *
         * This function accepts a pointer to a string encoded with a different
         * encoder and character type, decodes each codepoint to UTF-32, and re-encodes it using
         * this string’s encoder. The resulting text is stored in a new String instance.
         *
         * This method is particularly useful for converting between different Unicode representations,
         * such as from UTF-8 to UTF-16 or UTF-32.
         *
         * \tparam TOtherEncoder The encoder type used by the source string
         *
         * \param other A pointer to the source string to convert
         * \param otherLength The length of the source string convert
         *
         * \return A new String instance containing the converted text
         */
        template <typename TOtherEncoder>
        static BasicString ConvertFrom(const typename TOtherEncoder::CharType* other, typename TOtherEncoder::SizeType otherLength)
        {
            using OtherEncoder = TOtherEncoder;
            using OtherSizeType = typename OtherEncoder::SizeType;

            if (other == nullptr || otherLength == 0)
                return Empty;

            BasicString converted;
            converted.Reserve(otherLength * EncodingType::MaxCodepointEncodingLength);

            CharType buffer[EncodingType::MaxCodepointEncodingLength];
            for (OtherSizeType i = 0; i < otherLength;)
            {
                Utf32Char decoded;
                OtherSizeType decodedCound = OtherEncoder::Decode(other + i, otherLength - i, decoded);
                if (decodedCound == 0)
                    ExceptionUtils::ThrowCodepointDecodingException(i);

                i += decodedCound;

                SizeType encoded = EncodingType::Encode(decoded, buffer);
                if (encoded == 0)
                    ExceptionUtils::ThrowCodepointEncodingException(decoded);

                converted.Append(buffer, encoded);
            }

            return converted;
        }

        /**
         * \brief Creates a new BasicString from a View
         *
         * The created string will contain a copy of \p stringView plus a null-terminator
         *
         * \param stringView The view to copy
         *
         * \return The created BasicString
         */
        static BasicString FromView(View stringView)
        {
            return BasicString(stringView.GetData(), stringView.GetLength());
        }

        /**
         * \brief Utility constant value to return a reference/pointer to an empty BasicString
         */
        static const BasicString Empty;

        /**
         * \brief Default constructor
         *
         * Creates a new instance of BasicString with an empty content.
         */
        BasicString() = default;

        /**
         * \brief Constructor
         *
         * Creates a new instance of BasicString by copying a c-style string.
         *
         * \param string The c-style string to copy
         */
        BasicString(const CharType* string) :
            BasicString(string, StringUtils<CharType, SizeType>::GetLength(string))
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         *
         * Creates a new instance of BasicString by copying a c-style string.
         *
         * \param string The c-style string to copy
         * \param length The length of the string to copy
         */
        BasicString(const CharType* string, SizeType length)
        {
            Append(string, length);
        }

        /**
         * \brief Copy constructor
         *
         * Creates an instance of BasicString by copying another one.
         *
         * \param other The string to copy from
         */
        BasicString(const BasicString& other) = default;

        /**
         * \brief Move constructor
         *
         * Creates an instance of BasicString by moving another one.
         *
         * \param other The string to move
         */
        BasicString(BasicString&& other) noexcept = default;

        /**
         * \brief Copy assignment operator
         *
         * \param other The source BasicString to copy from
         *
         * \return A reference to this BasicString after assignment
         */
        BasicString& operator=(const BasicString& other) = default;

        /**
         * \brief Move assignment operator
         *
         * \param other The source BasicString to copy from
         *
         * \return A reference to this BasicString after assignment
         */
        BasicString& operator=(BasicString&& other) noexcept = default;

        /**
         * \brief Assigns a null-terminated string to this BasicString
         *
         * Replaces the current string content with the provided null-terminated raw character sequence.
         * If the provided pointer refers to the same internal buffer, the assignment is ignored to
         * prevent self-assignment issues. Otherwise, the current data is cleared, and the new string
         * is appended using the encoder's logic.
         *
         * \param string A pointer to a null-terminated character array to assign
         *
         * \return A reference to this BasicString after assignment
         */
        BasicString& operator=(const CharType* string)
        {
            if (string != mCharacters.GetData())
            {
                mCharacters.Clear();
                Append(string);
            }

            return *this;
        }

		/**
		 * \brief Appends a single character to this string
		 *
		 * Appends the character \p string to the end of this string.
		 * This overload simply forwards to the range-based version with a length of 1.
		 *
		 * \param string The character to append
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Append(CharType string)
		{
			return Append(&string, 1);
		}

		/**
		 * \brief Appends the contents of a string view to this string
		 *
		 * Appends all characters in \p string to the end of this string.
		 * If the string view is empty, the function performs no modification.
		 *
		 * \param string The view whose contents will be appended
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Append(View string)
		{
			return string.IsEmpty() ? *this : Append(string.GetData(), string.GetLength());
		}

		/**
		 * \brief Appends the contents of another BasicString to this string
		 *
		 * Appends all characters from \p string to this string.
		 * If the provided string is empty, this string remains unchanged.
		 *
		 * \param string The BasicString whose contents will be appended
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Append(const BasicString& string)
		{
			return string.IsEmpty() ? *this : Append(string.GetData(), string.GetLength());
		}

		/**
		 * \brief Appends a null-terminated character sequence to this string
		 *
		 * Appends the characters from the null-terminated sequence \p string.
		 * The length of the sequence is automatically determined using StringUtils.
		 *
		 * \param string A pointer to a null-terminated character array to append
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Append(const CharType* string)
		{
			return Append(string, StringUtils<CharType, SizeType>::GetLength(string));
		}

		/**
		 * \brief Appends a range of characters to this string
		 *
		 * Appends exactly \p length characters starting from \p string.
		 * If the input range refers to a region inside this string’s internal buffer,
		 * a temporary copy is created to avoid undefined behavior.
		 *
		 * When the string is non-empty, its trailing null-terminator is removed before appending
		 * and a new null-terminator is added after the operation, ensuring that this string
		 * remains properly null-terminated.
		 *
		 * \param string A pointer to the character data to append
		 * \param length The number of characters to append
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Append(const CharType* string, SizeType length)
		{
			if (!mCharacters.IsEmpty())
				mCharacters.PopLast();

			const CharType* begin = mCharacters.GetData();
			const CharType* end = begin + GetLength();

			if (string >= begin && string < end)
			{
				CharType* copy = COCKTAIL_STACK_ALLOC(CharType, length);
				ObjectMemoryUtils::CopyRange(length, copy, string);

				mCharacters.Append(copy, length);
			}
			else
			{
				mCharacters.Append(string, length);
			}
			mCharacters.Add('\0');

			return *this;
		}

		/**
		 * \brief Prepends a single character to this string
		 *
		 * Prepends the character \p string to the beginning of this string.
		 * This overload simply forwards to the range-based version with a length of 1.
		 *
		 * \param string The character to prepend
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Prepend(CharType string)
		{
			return Prepend(&string, 1);
		}

		/**
		 * \brief Prepends the contents of a string view to this string
		 *
		 * Prepends all characters contained in \p string to the beginning of this string.
		 * If the string view is empty, the function performs no modification.
		 *
		 * \param string The view whose contents will be prepended
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Prepend(View string)
		{
			return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
		}

		/**
		 * \brief Prepends the contents of another BasicString to this string
		 *
		 * Prepends all characters from \p string to this string.
		 * If the provided string is empty, this string remains unchanged.
		 *
		 * \param string The BasicString whose contents will be prepended
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Prepend(const BasicString& string)
		{
			return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
		}

		/**
		 * \brief Prepends a null-terminated character sequence to this string
		 *
		 * Prepends the characters from the null-terminated sequence \p string.
		 * The length of the sequence is determined automatically using StringUtils.
		 *
		 * \param string A pointer to a null-terminated character array to prepend
		 *
		 * \return A reference to this BasicString after modification
		 */
		BasicString& Prepend(const CharType* string)
		{
			return Prepend(string, StringUtils<CharType, SizeType>::GetLength(string));
		}

        /**
         * \brief Prepends a range of characters to this string
         *
         * Prepends exactly \p length characters starting from \p string.
         * If the input range refers to a region inside this string’s internal buffer,
         * a temporary copy is created to avoid undefined behavior.
         * The function ensures that the resulting string remains null-terminated.
         *
         * \param string A pointer to the character data to prepend
         * \param length The number of characters to prepend
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& Prepend(const CharType* string, SizeType length)
        {
            const bool wasEmpty = IsEmpty();

            const CharType* begin = mCharacters.GetData();
            const CharType* end = begin + GetLength();

            if (string >= begin && string < end)
            {
                CharType* copy = COCKTAIL_STACK_ALLOC(CharType, length);
                ObjectMemoryUtils::CopyRange(length, copy, string);

                mCharacters.Prepend(copy, length);
            }
            else
            {
                mCharacters.Prepend(string, length);
            }

            if (wasEmpty)
                mCharacters.Add('\0');

            return *this;
        }

		/**
		 * \brief Returns a reference to the character at the given index
		 *
		 * Provides direct access to the character at position \p index.
		 * If \p index is out of range (i.e., greater than or equal to the string length),
		 * an OutOfRange exception is thrown.
		 *
		 * \param index The zero-based index of the character to retrieve
		 *
		 * \return A reference to the character at the specified index
		 *
		 * \throws OutOfRange If \p index is outside the valid range
		 */
		CharType& At(SizeType index)
		{
			return mCharacters.At(index);
		}

		/**
		 * \brief Returns a const reference to the character at the given index
		 *
		 * Provides read-only access to the character at position \p index.
		 * If \p index is out of range, an OutOfRange exception is thrown.
		 *
		 * \param index The zero-based index of the character to retrieve
		 *
		 * \return A const reference to the character at the specified index
		 *
		 * \throws OutOfRange If \p index is outside the valid range
		 */
		const CharType& At(SizeType index) const
		{
			return mCharacters.At(index);
		}

		/**
		 * \brief Attempts to access the character at the given index
		 *
		 * Returns an Optional containing a reference to the character at \p index if the index
		 * is within bounds. If \p index is out of range, no exception is thrown and an empty
		 * Optional is returned instead.
		 *
		 * This method is the safe counterpart of At(), guaranteeing that it never throws.
		 *
		 * \param index The zero-based index of the character to retrieve
		 *
		 * \return An Optional containing a reference to the character if valid,
		 *         or an empty Optional otherwise
		 */
		Optional<CharType&> TryAt(SizeType index)
		{
			if (index >= GetLength())
				return Optional<CharType&>::Empty();

			return Optional<CharType&>::Of(mCharacters[index]);
		}

		/**
		 * \brief Attempts to access the character at the given index
		 *
		 * Returns an Optional containing a const reference to the
		 * character at \p index when valid. If \p index is out of bounds, an empty Optional
		 * is returned and no exception is thrown.
		 *
		 * \param index The zero-based index of the character to retrieve
		 *
		 * \return An Optional containing a const reference to the character if valid,
		 *         or an empty Optional otherwise
		 */
		Optional<const CharType&> TryAt(SizeType index) const
		{
			if (index >= GetLength())
				return Optional<const CharType&>::Empty();

			return Optional<const CharType&>::Of(mCharacters[index]);
		}

		/**
		 * \brief Returns a reference to the first character in the string
		 *
		 * Provides direct access to the first character.
		 * If the string is empty, an OutOfRange exception is thrown.
		 *
		 * \return A reference to the first character
		 *
		 * \throws Exception If the string is empty
		 */
		CharType& First()
		{
			if (Optional<CharType&> first = TryFirst(); !first.IsEmpty())
				return first.Get();

			ExceptionUtils::ThrowEmptyContainer();
		}

		/**
		 * \brief Returns a const reference to the first character in the string
		 *
		 * Provides read-only access to the first character.
		 * If the string is empty, an OutOfRange exception is thrown.
		 *
		 * \return A const reference to the first character
		 *
		 * \throws Exception If the string is empty
		 */
		const CharType& First() const
		{
			if (Optional<const CharType&> first = TryFirst(); !first.IsEmpty())
				return first.Get();

			ExceptionUtils::ThrowEmptyContainer();
		}

		/**
		 * \brief Attempts to access the first character
		 *
		 * Returns an Optional containing a reference to the first character if the string
		 * is not empty. Returns an empty Optional if the string is empty.
		 *
		 * \return An Optional containing a reference to the first character,
		 *         or empty if the string is empty
		 */
		Optional<CharType&> TryFirst()
		{
			if (IsEmpty())
				return Optional<CharType&>::Empty();

			return Optional<CharType&>::Of(At(0));
		}

		/**
		 * \brief Attempts to access the first character
		 *
		 * Returns an Optional containing a const reference
		 * to the first character if the string is not empty. Returns an empty Optional
		 * if the string is empty.
		 *
		 * \return An Optional containing a const reference to the first character,
		 *         or empty if the string is empty
		 */
		Optional<const CharType&> TryFirst() const
		{
			if (IsEmpty())
				return Optional<const CharType&>::Empty();

			return Optional<const CharType&>::Of(At(0));
		}

		/**
		 * \brief Returns a reference to the last character in the string
		 *
		 * Provides direct access to the last character.
		 * If the string is empty, an OutOfRange exception is thrown.
		 *
		 * \return A reference to the last character
		 *
		 * \throws Exception If the string is empty
		 */
		CharType& Last()
		{
			if (Optional<CharType&> last = TryLast(); !last.IsEmpty())
				return last.Get();

			ExceptionUtils::ThrowEmptyContainer();
		}

		/**
		 * \brief Returns a const reference to the last character in the string
		 *
		 * Provides read-only access to the last character
		 * If the string is empty, an OutOfRange exception is thrown.
		 *
		 * \return A const reference to the last character
		 *
		 * \throws Exception If the string is empty
		 */
		const CharType& Last() const
		{
			if (Optional<const CharType&> last = TryLast(); !last.IsEmpty())
				return last.Get();

			ExceptionUtils::ThrowEmptyContainer();
		}

		/**
		 * \brief Attempts to access the last character
		 *
		 * Returns an Optional containing a reference to the last character if the string
		 * is not empty. Returns an empty Optional if the string is empty.
		 *
		 * \return An Optional containing a reference to the last character,
		 *         or empty if the string is empty
		 */
		Optional<CharType&> TryLast()
		{
			if (IsEmpty())
				return Optional<CharType&>::Empty();

			return Optional<CharType&>::Of(At(GetLength() - 1));
		}

		/**
		 * \brief Attempts to access the last character
		 *
		 * Returns an Optional containing a const reference
		 * to the last character if the string is not empty. Returns an empty Optional
		 * if the string is empty.
		 *
		 * \return An Optional containing a const reference to the last character,
		 *         or empty if the string is empty
		 */
		Optional<const CharType&> TryLast() const
		{
			if (IsEmpty())
				return Optional<const CharType&>::Empty();

			return Optional<const CharType&>::Of(At(GetLength() - 1));
		}

        /**
         * \brief Finds the first occurrence of a character in this string
         *
         * Searches for the first occurrence of \p search starting from \p startIndex.
         *
         * \param search The character to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(CharType search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a string view in this string
         *
         * Searches for the first occurrence of the sequence \p search starting from \p startIndex.
         *
         * \param search The string view to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(View search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of another BasicString in this string
         *
         * Searches for the first occurrence of the sequence in \p search starting from \p startIndex.
         *
         * \param search The BasicString to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(const BasicString& search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a null-terminated character sequence in this string
         *
         * Searches for the first occurrence of the null-terminated sequence \p search
         * starting from \p startIndex. The length is determined automatically.
         *
         * \param search A pointer to the null-terminated character sequence to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(const CharType* search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search, StringUtils<CharType, SizeType>::GetLength(search), startIndex, caseSensitive);
        }

        /**
         * \brief Finds the first occurrence of a character sequence in this string
         *
         * Searches for the first occurrence of exactly \p length characters from \p search
         * starting from \p startIndex.
         *
         * \param search A pointer to the character sequence to search for
         * \param length The length of the character sequence to search for
         * \param startIndex The index to start searching from
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the first occurrence, or empty if not found
         */
        Optional<SizeType> FindFirst(const CharType* search, SizeType length, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, length, startIndex, caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a character in this string
         *
         * Searches for the last occurrence of \p search in the entire string.
         *
         * \param search The character to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(CharType search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a string view in this string
         *
         * Searches for the last occurrence of the sequence \p search.
         *
         * \param search The string view to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(View search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of another BasicString in this string
         *
         * Searches for the last occurrence of the sequence in \p search.
         *
         * \param search The BasicString to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(const BasicString& search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a null-terminated character sequence in this string
         *
         * Searches for the last occurrence of the null-terminated sequence \p search.
         * The length is determined automatically.
         *
         * \param search A pointer to the null-terminated character sequence to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(const CharType* search, bool caseSensitive = true) const
        {
            return FindLast(search, StringUtils<CharType, SizeType>::GetLength(search), caseSensitive);
        }

        /**
         * \brief Finds the last occurrence of a character sequence in this string
         *
         * Searches for the last occurrence of exactly \p length characters from \p search.
         *
         * \param search A pointer to the character sequence to search for
         * \param length The length of the character sequence to search for
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return An Optional containing the index of the last occurrence, or empty if not found
         */
        Optional<SizeType> FindLast(const CharType* search, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, length, caseSensitive);
        }

        /**
         * \brief Removes and returns the first character of the string
         *
         * Extracts the first character from this string and shifts the remaining
         * characters. If the string is empty, an exception is thrown.
         *
         * \return The removed character
         *
         * \throws Exception If the string is empty
         */
        CharType PopFirst()
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mCharacters.PopFirst();
        }

        /**
         * \brief Removes and returns the last character of the string
         *
         * Extracts the last character from this string (excluding the null terminator).
         * If the string is empty, an exception is thrown.
         *
         * \return The removed character
         *
         * \throws Exception If the string is empty
         */
        CharType PopLast()
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mCharacters.RemoveAt(GetLength() - 1);
        }
        
		/**
		 * \brief Returns the number of UTF codepoints in the string
		 *
		 * Counts the number of Unicode codepoints available in this string.
		 * The count may vary depending on the internal decoder used.
		 * For string types that do not support surrogate pairs, this is equivalent
		 * to calling GetLength().
		 *
		 * \return The number of Unicode codepoints in the string
		 */
		SizeType CodepointCount() const
		{
			return IsEmpty() ? 0 : EncodingType::GetCharCount(GetData(), GetLength());
		}

		/**
		 * \brief Returns the UTF-32 codepoint at the specified index
		 *
		 * Provides direct access to the Unicode codepoint at position \p codepointIndex.
		 * If the index is out of range or decoding fails, an exception is thrown.
		 *
		 * \param codepointIndex The zero-based index of the codepoint to retrieve
		 *
		 * \return The UTF-32 codepoint at the specified index
		 *
		 * \throws CodepointDecodingException if the codepoint cannot be decoded
		 * \throws OutOfRange If the codepoint cannot be decoded or index is invalid
		 */
		Utf32Char CodepointAt(SizeType codepointIndex) const
		{
			if (auto codepoint = TryCodepointAt(codepointIndex); !codepoint.IsEmpty())
				return codepoint.Get();

			ExceptionUtils::ThrowOutOfRange(0, CodepointCount());
		}

		/**
		 * \brief Attempts to retrieve the UTF-32 codepoint at the specified index
		 *
		 * Returns an Optional containing the codepoint at \p codepointIndex if it exists
		 * and can be decoded. If the index is invalid, an empty Optional is returned.
		 * If the codepoint cannot be decoded, an exception is thrown.
		 *
		 * \param codepointIndex The zero-based index of the codepoint to retrieve
		 *
		 * \return An Optional containing the UTF-32 codepoint if valid, or empty otherwise
		 *
		 * \throws CodepointDecodingException if the codepoint cannot be decoded
		 */
		Optional<Utf32Char> TryCodepointAt(SizeType codepointIndex) const
		{
            SizeType codepointCount = CodepointCount();
			if (codepointIndex >= codepointCount)
				return Optional<Utf32Char>::Empty();

			Optional<SizeType> offset = TryOffsetByCodepoint(0, codepointIndex);
			if (offset.IsEmpty())
				return Optional<Utf32Char>::Empty();

			Utf32Char out;
			if (EncodingType::Decode(GetData() + offset.Get(), GetLength() - offset.Get(), out) == 0)
				ExceptionUtils::ThrowCodepointDecodingException(codepointIndex);

			return Optional<Utf32Char>::Of(out);
		}

		/**
		 * \brief Returns the character offset corresponding to a number of codepoints
		 *
		 * Calculates the character offset in the string after moving \p codepointCount
		 * codepoints starting from position \p start. If the resulting offset is invalid,
		 * an exception is thrown.
		 *
		 * \param start The starting character index
		 * \param codepointCount The number of codepoints to advance
		 *
		 * \return The character offset after advancing \p codepointCount codepoints
		 *
		 * \throws OutOfRange If the resulting offset is outside the valid range
		 */
		SizeType OffsetByCodepoint(SizeType start, SizeType codepointCount) const
		{
			if (auto offset = TryOffsetByCodepoint(start, codepointCount); !offset.IsEmpty())
				return offset.Get();

			ExceptionUtils::ThrowOutOfRange(start, GetLength());
		}

		/**
		 * \brief Attempts to compute the character offset after a number of codepoints
		 *
		 * Returns an Optional containing the character offset obtained by advancing
		 * \p codepointCount codepoints starting from \p start. If an invalid codepoint
		 * or end-of-string is reached, an empty Optional is returned.
		 *
		 * \param start The starting character index
		 * \param codepointCount The number of codepoints to advance
		 *
		 * \return An Optional containing the resulting character offset, or empty if invalid
		 */
		Optional<SizeType> TryOffsetByCodepoint(SizeType start, SizeType codepointCount) const
		{
			SizeType charOffset = start;
			for (SizeType i = 0; i < codepointCount; ++i)
			{
				SizeType charCount = EncodingType::GetCharCount(GetData() + charOffset);
				if (charCount == 0)
					return Optional<SizeType>::Empty();

				charOffset += charCount;
			}

			return Optional<SizeType>::Of(charOffset);
		}

        /**
         * \brief Returns a substring starting from the given index to the end
         *
         * Creates a new BasicString containing a copy of the characters from \p start
         * to the end of this string.
         *
         * \param start The zero-based starting index of the substring
         *
         * \return A new BasicString containing the requested portion
         */
        BasicString SubString(SizeType start) const
        {
            return BasicString::FromView(SubStringView(start));
        }

        /**
         * \brief Returns a substring starting from the given index with a specified length
         *
         * Creates a new BasicString containing a copy of at most \p length characters
         * starting from \p start.
         *
         * \param start The zero-based starting index of the substring
         * \param length The maximum number of characters to include
         *
         * \return A new BasicString containing the requested portion
         */
        BasicString SubString(SizeType start, SizeType length) const
        {
            return BasicString::FromView(SubStringView(start, length));
        }

        /**
         * \brief Returns a non-owning view over a portion of this string starting from the given index
         *
         * Creates a View referencing the characters from \p start to the end of this string.
         * No copy is performed.
         *
         * \param start The zero-based starting index of the sub-view
         *
         * \return A View over the requested range
         */
        View SubStringView(SizeType start) const
        {
            return SubStringView(start, GetLength() - start);
        }

        /**
         * \brief Returns a non-owning view over a portion of this string
         *
         * Creates a View referencing at most \p length characters starting from \p start.
         * If \p length exceeds the remaining characters, the view is clamped.
         * No copy is performed.
         *
         * \param start The zero-based starting index of the sub-view
         * \param length The maximum number of characters to include
         *
         * \return A View over the requested range
         */
        View SubStringView(SizeType start, SizeType length) const
        {
            return View(GetData() + start, std::min(length, GetLength() - start));
        }

        /**
         * \brief Compares this string with a string view
         *
         * Performs a lexicographic comparison between this string and \p stringView.
         *
         * \param stringView The string view to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this string is less than \p stringView,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(View stringView, bool caseSensitive = true) const
        {
            return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
        }

        /**
         * \brief Compares this string with another BasicString
         *
         * Performs a lexicographic comparison between this string and \p other.
         *
         * \param other The BasicString to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this string is less than \p other,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const BasicString& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Compares this string with a null-terminated character sequence
         *
         * Performs a lexicographic comparison between this string and \p string.
         * The length of \p string is determined automatically.
         *
         * \param string A pointer to the null-terminated character sequence to compare against
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this string is less than \p string,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const CharType* string, bool caseSensitive = true) const
        {
            return Compare(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        /**
         * \brief Compares this string with a character sequence of given length
         *
         * Performs a lexicographic comparison between this string and \p length characters
         * starting from \p string.
         *
         * \param string A pointer to the character sequence to compare against
         * \param length The number of characters to compare
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return A negative value if this string is less than the sequence,
         *         zero if they are equal, or a positive value if greater
         */
        int Compare(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::Compare(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        /**
         * \brief Checks whether this string starts with a given character
         *
         * \param character The character to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string starts with \p character, false otherwise
         */
        bool StartsWith(CharType character, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mCharacters.First(), character, caseSensitive);
        }

        /**
         * \brief Checks whether this string starts with a given string view
         *
         * \param stringView The string view to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string starts with \p stringView, false otherwise
         */
        bool StartsWith(View stringView, bool caseSensitive = true) const
        {
            return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this string starts with another BasicString
         *
         * \param other The BasicString to check for at the beginning
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string starts with \p other, false otherwise
         */
        bool StartsWith(const BasicString& other, bool caseSensitive = true) const
        {
            return StartsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this string starts with a null-terminated character sequence
         *
         * \param string A pointer to the null-terminated character sequence to check
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string starts with \p string, false otherwise
         */
        bool StartsWith(const CharType* string, bool caseSensitive = true) const
        {
            return StartsWith(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        /**
         * \brief Checks whether this string starts with a character sequence of given length
         *
         * \param string A pointer to the character sequence to check for at the beginning
         * \param length The length of the character sequence
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string starts with the given sequence, false otherwise
         */
        bool StartsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        /**
         * \brief Checks whether this string ends with a given character
         *
         * \param character The character to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string ends with \p character, false otherwise
         */
        bool EndsWith(CharType character, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(character, mCharacters[GetLength() - 1], caseSensitive);
        }

        /**
         * \brief Checks whether this string ends with a given string view
         *
         * \param other The string view to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string ends with \p other, false otherwise
         */
        bool EndsWith(View other, bool caseSensitive = true) const
        {
            return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this string ends with another BasicString
         *
         * \param other The BasicString to check for at the end
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string ends with \p other, false otherwise
         */
        bool EndsWith(const BasicString& other, bool caseSensitive = true) const
        {
            return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        /**
         * \brief Checks whether this string ends with a null-terminated character sequence
         *
         * \param string A pointer to the null-terminated character sequence to check
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string ends with \p string, false otherwise
         */
        bool EndsWith(const CharType* string, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        /**
         * \brief Checks whether this string ends with a character sequence of given length
         *
         * \param string A pointer to the character sequence to check for at the end
         * \param length The length of the character sequence
         * \param caseSensitive Whether the comparison should be case-sensitive
         *
         * \return True if the string ends with the given sequence, false otherwise
         */
        bool EndsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        /**
         * \brief Splits this string by a separator character and returns the resulting parts
         *
         * Creates an Array of substrings obtained by splitting this string at each
         * occurrence of \p separator. The separator itself is not included in the results.
         *
         * \param separator The character to split on
         *
         * \return An Array of substrings
         */
        Array<BasicString, AllocatorType> Split(CharType separator) const
        {
            Array<BasicString, AllocatorType> splits;
            Split(splits, separator);

            return splits;
        }

        /**
         * \brief Splits this string by a separator character into an existing array
         *
         * Populates \p splits with substrings obtained by splitting this string at each
         * occurrence of \p separator. The array is cleared before insertion.
         * The separator itself is not included in the results.
         *
         * \param splits The array to populate with the resulting substrings
         * \param separator The character to split on
         */
        void Split(Array<BasicString, AllocatorType>& splits, CharType separator) const
        {
            splits.Clear();

            for (unsigned int start = 0; start < GetLength();)
            {
                Array<CharType> slice;
                Optional<unsigned int> index = mCharacters.FindIndex(separator, start);
                if (index.IsEmpty())
                {
                    slice = mCharacters.Slice(start);
                }
                else
                {
                    slice = mCharacters.Slice(start, index.Get() - start);
                }

                BasicString split;
                split.mCharacters = std::move(slice);
                if (split.mCharacters.Last() != '\0')
                    split.mCharacters.Add('\0');

                start += split.GetLength() + 1;

                splits.Add(std::move(split));
            }
        }

        /**
         * \brief Converts all characters in this string to lowercase in-place
         *
         * Transforms each character to its lowercase equivalent using CharUtils.
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& ToLowercase()
        {
            mCharacters.TransformInPlace([](CharType character) {
                return CharUtils<CharType>::ToLower(character);
            });

            return *this;
        }

        /**
         * \brief Converts all characters in this string to uppercase in-place
         *
         * Transforms each character to its uppercase equivalent using CharUtils.
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& ToUppercase()
        {
            mCharacters.TransformInPlace([](CharType character) {
                return CharUtils<CharType>::ToUpper(character);
            });

            return *this;
        }

        /**
         * \brief Removes all characters from this string
         *
         * After this call, the string is empty and GetLength() returns 0.
         */
        void Clear()
        {
            mCharacters.Clear();
        }

        /**
         * \brief Reserves storage capacity for at least a given number of characters
         *
         * Pre-allocates memory for \p size characters plus the null terminator,
         * avoiding reallocations during subsequent Append or Prepend calls.
         *
         * \param size The number of characters to reserve capacity for
         */
        void Reserve(SizeType size)
        {
            mCharacters.Reserve(size + 1);
        }

        /**
         * \brief Resizes the string to a given number of characters
         *
         * If \p size is larger than the current length, new characters are filled with
         * the null character. If \p size is smaller, the string is truncated.
         *
         * \param size The desired number of characters
         */
        void Resize(SizeType size)
        {
            if (!IsEmpty())
                PopLast();

            mCharacters.Resize(size + 1, '\0');
        }

        /**
         * \brief Ensures the string has at least the given size
         *
         * If the current length is already greater than or equal to \p size, no action is taken.
         * Otherwise, the string is resized and new characters are filled with the null character.
         *
         * \param size The minimum desired number of characters
         *
         * \return True if the string was resized, false if it already had sufficient size
         */
        bool EnsureSize(SizeType size)
        {
            if (!IsEmpty())
                PopLast();

            bool resized = mCharacters.EnsureSize(size + 1);
            mCharacters[size] = '\0';

            return resized;
        }

        /**
         * \brief Resizes the string to a given number of characters with a fill value
         *
         * If \p size is larger than the current length, new characters are filled with \p value.
         * If \p size is smaller, the string is truncated.
         *
         * \param size The desired number of characters
         * \param value The character used to fill newly added positions
         */
        void Resize(SizeType size, CharType value)
        {
            if (!IsEmpty())
                PopLast();

            mCharacters.Resize(size, value);
            mCharacters.Append('\0');
        }

        /**
         * \brief Ensures the string has at least the given size with a fill value
         *
         * If the current length is already greater than or equal to \p size, no action is taken.
         * Otherwise, the string is resized and new characters are filled with \p value.
         *
         * \param size The minimum desired number of characters
         * \param value The character used to fill newly added positions
         *
         * \return True if the string was resized, false if it already had sufficient size
         */
        bool EnsureSize(SizeType size, CharType value)
        {
            if (!IsEmpty())
                PopLast();

            bool resized = mCharacters.EnsureSize(size + 1, value);
            mCharacters[size] = '\0';

            return resized;
        }

        /**
         * \brief Checks whether this string is empty
         *
         * \return True if the string has a length of zero, false otherwise
         */
        bool IsEmpty() const
        {
            return GetLength() == 0;
        }

        /**
         * \brief Get the number of character available in this String
         * If the String type supports surrogates, the number of characters encoded might be different
         * as the number of character available.
         *
         * \return The number of character
         */
        SizeType GetLength() const
        {
            return mCharacters.IsEmpty() ? 0 : mCharacters.GetSize() - 1;
        }

        /**
         * \brief Returns a pointer to the mutable underlying character data
         *
         * Provides direct access to the raw character buffer.
         * Returns nullptr if the string is empty.
         *
         * \return A pointer to the first character, or nullptr if empty
         */
        CharType* GetData()
        {
            return IsEmpty() ? nullptr : mCharacters.GetData();
        }

        /**
         * \brief Returns a pointer to the underlying character data
         *
         * Provides read-only access to the raw character buffer.
         * Returns nullptr if the string is empty.
         *
         * \return A pointer to the first character, or nullptr if empty
         */
        const CharType* GetData() const
        {
            return IsEmpty() ? nullptr : mCharacters.GetData();
        }

        /**
         * \brief Returns a reference to the character at the given index without bounds checking
         *
         * Provides direct access to the character at position \p index.
         * No range checking is performed.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return A reference to the character at the specified index
         */
        CharType& operator[](SizeType index)
        {
            return mCharacters[index];
        }

        /**
         * \brief Returns a const reference to the character at the given index without bounds checking
         *
         * Provides read-only access to the character at position \p index.
         * No range checking is performed.
         *
         * \param index The zero-based index of the character to retrieve
         *
         * \return A const reference to the character at the specified index
         */
        const CharType& operator[](SizeType index) const
        {
            return mCharacters[index];
        }

        /**
         * \brief Appends a string view to this string using the += operator
         *
         * \param rhs The string view to append
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& operator+=(View rhs)
        {
            return Append(rhs);
        }

        /**
         * \brief Appends another BasicString to this string using the += operator
         *
         * \param rhs The BasicString to append
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& operator+=(const BasicString& rhs)
        {
            return Append(rhs);
        }

        /**
         * \brief Appends a null-terminated character sequence to this string using the += operator
         *
         * \param rhs A pointer to the null-terminated character sequence to append
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& operator+=(const CharType* rhs)
        {
            return Append(rhs);
        }

    private:

        Array<CharType, AllocatorType> mCharacters;
    };

    template <typename TEncoding, typename TAllocator>
    const BasicString<TEncoding, TAllocator> BasicString<TEncoding, TAllocator>::Empty;

    template <typename TEncoding, typename TAllocator>
    bool operator==(const BasicString<TEncoding, TAllocator>& lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    template <typename TEncoding, typename TAllocator>
    bool operator==(const BasicString<TEncoding, TAllocator>& lhs, const typename TEncoding::CharType* rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    template <typename TEncoding, typename TAllocator>
    bool operator==(const typename TEncoding::CharType* lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return rhs.Compare(lhs) == 0;
    }

    template <typename TEncoding, typename TAllocator>
    bool operator==(const BasicString<TEncoding, TAllocator>& lhs, BasicStringView<TEncoding> rhs)
    {
        return lhs.Compare(rhs) == 0;
    }

    template <typename TEncoding, typename TAllocator>
    bool operator==(BasicStringView<TEncoding> lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return rhs.Compare(lhs) == 0;
    }

    template <typename TEncoding, typename TAllocator>
    bool operator!=(const BasicString<TEncoding, TAllocator>& lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding, typename TAllocator>
    bool operator!=(const BasicString<TEncoding, TAllocator>& lhs, const typename TEncoding::CharType* rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding, typename TAllocator>
    bool operator!=(const typename TEncoding::CharType* lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding, typename TAllocator>
    bool operator!=(const BasicString<TEncoding, TAllocator>& lhs, BasicStringView<TEncoding> rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding, typename TAllocator>
    bool operator!=(BasicStringView<TEncoding> lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TEncoding, typename TAllocator>
    BasicString<TEncoding, TAllocator> operator+(const BasicString<TEncoding, TAllocator>& lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return BasicString<TEncoding, TAllocator>(lhs) += rhs;
    }

    template <typename TEncoding, typename TAllocator>
    BasicString<TEncoding, TAllocator> operator+(const typename TEncoding::CharType* lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return BasicString<TEncoding, TAllocator>(lhs) += rhs;
    }

    template <typename TEncoding, typename TAllocator>
    BasicString<TEncoding, TAllocator> operator+(const BasicString<TEncoding, TAllocator>& lhs, const typename TEncoding::CharType* rhs)
    {
        return BasicString<TEncoding, TAllocator>(lhs) += rhs;
    }

    template <typename TEncoding, typename TAllocator>
    BasicString<TEncoding, TAllocator> operator+(BasicStringView<TEncoding> lhs, const BasicString<TEncoding, TAllocator>& rhs)
    {
        return BasicString<TEncoding, TAllocator>::FromView(lhs) += rhs;
    }

    template <typename TEncoding, typename TAllocator>
    BasicString<TEncoding, TAllocator> operator+(const BasicString<TEncoding, TAllocator>& lhs, BasicStringView<TEncoding> rhs)
    {
        return BasicString<TEncoding, TAllocator>(lhs) += rhs;
    }

    using AsciiString = BasicString<Encoders::Ascii>;
    using AsciiStringView = AsciiString::View;

    using Utf8String = BasicString<Encoders::Utf8>;
    using Utf8StringView = Utf8String::View;

    using Utf16String = BasicString<Encoders::Utf16>;
    using Utf16StringView = Utf16String::View;

    using Utf32String = BasicString<Encoders::Utf32>;
    using Utf32StringView = Utf32String::View;

    using String = BasicString<Encoders::Text>;
    using StringView = String::View;

    template <typename TEncoding>
    struct Formatter<BasicStringView<TEncoding>, void>
    {
        using StringViewType = BasicStringView<TEncoding>;
        using CharType = typename StringViewType::CharType;

        const CharType* Apply(const StringViewType& value) const noexcept
        {
            return value.GetData();
        }
    };

    template <typename TEncoding, typename TAllocator>
    struct Formatter<BasicString<TEncoding, TAllocator>, void>
    {
        using StringType = BasicString<TEncoding, TAllocator>;
        using CharType = typename StringType::CharType;

        const CharType* Apply(const StringType& value) const noexcept
        {
            return value.GetData();
        }
    };
}

namespace std
{
    template <typename TEncoding, typename TAllocator>
    struct hash<Ck::BasicString<TEncoding, TAllocator>>
    {
        using StringType = Ck::BasicString<TEncoding, TAllocator>;

        size_t operator()(const StringType& string) const noexcept
        {
            size_t hash = 0;
            for (typename StringType::SizeType i = 0; i < string.GetLength(); ++i)
                Ck::HashCombine(hash, string[i]);

            return hash;
        }
    };

    template <typename TEncoding>
    struct hash<Ck::BasicStringView<TEncoding>>
    {
        using StringViewType = Ck::BasicStringView<TEncoding>;

        size_t operator()(const StringViewType& string) const noexcept
        {
            size_t hash = 0;
            for (typename StringViewType::SizeType i = 0; i < string.GetLength(); ++i)
                Ck::HashCombine(hash, string[i]);

            return hash;
        }
    };
}

#endif // COCKTAIL_CORE_STRING_HPP
