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
     * \brief
     */
    template <typename TEncoding>
    class BasicStringView
    {
    public:

        /**
         * \brief The decoder type used to convert raw string data from and to UTF Codepoints
         */
        using EncodingType = TEncoding;

        /**
         * \brief
         */
        using CharType = typename EncodingType::CharType;

        /**
         * \brief
         */
        using SizeType = typename EncodingType::SizeType;

        /**
         * \brief Utility constant value to return a reference/pointer to an empty StringView
         */
        static const BasicStringView Empty;

        BasicStringView() :
            mString(nullptr),
            mLength(0)
        {
            /// Nothing
        }

        template <typename TAllocator>
        BasicStringView(const BasicString<TEncoding, TAllocator>& string) :
            BasicStringView(string.GetData(), string.GetLength())
        {
            /// Nothing
        }

        BasicStringView(const CharType* string) :
            BasicStringView(string, StringUtils<CharType, SizeType>::GetLength(string))
        {
            /// Nothing
        }

        BasicStringView(const CharType* string, SizeType length) :
            mString(string),
            mLength(length)
        {
            /// Nothing
        }

        template <typename TAllocator>
        BasicStringView operator=(const BasicString<TEncoding, TAllocator>& string)
        {
            mString = string.GetData();
            mLength = string.GetLength();

            return *this;
        }

        BasicStringView operator=(const CharType* string)
        {
            mString = string;
            mLength = StringUtils<CharType, SizeType>::GetLength(string);

            return *this;
        }

        const CharType& At(SizeType index) const
        {
            if (index >= mLength)
                ExceptionUtils::ThrowOutOfRange(index, mLength);

            return mString[index];
        }

        Optional<CharType&> TryAt(SizeType index)
        {
            if (index >= mLength)
                Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(mString[index]);
        }

        Optional<const CharType&> TryAt(SizeType index) const
        {
            if (index >= mLength)
                Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(mString[index]);
        }

        CharType& First()
        {
            if (Optional<CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const CharType& First() const
        {
            if (Optional<const CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<CharType&> TryFirst()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(0));
        }

        Optional<const CharType&> TryFirst() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(0));
        }

        CharType& Last()
        {
            if (Optional<CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const CharType& Last() const
        {
            if (Optional<const CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<CharType&> TryLast()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(GetLength() - 1));
        }

        Optional<const CharType&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(GetLength() - 1));
        }

        Optional<SizeType> FindFirst(CharType search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mString, mLength, search, startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(BasicStringView search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        template <typename TAllocator>
        Optional<SizeType> FindFirst(const BasicString<TEncoding, TAllocator>& search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(const CharType* search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search, StringUtils<CharType, SizeType>::GetLength(search), startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(const CharType* search, SizeType length, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mString, mLength, search, length, startIndex, caseSensitive);
        }

        Optional<SizeType> FindLast(CharType search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mString, mLength, search, caseSensitive);
        }

        Optional<SizeType> FindLast(BasicStringView search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }
        template <typename TAllocator>
        Optional<SizeType> FindLast(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        Optional<SizeType> FindLast(const CharType* search, bool caseSensitive = true) const
        {
            return FindLast(search, StringUtils<CharType, SizeType>::GetLength(search), caseSensitive);
        }

        Optional<SizeType> FindLast(const CharType* search, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mString, mLength, search, length, caseSensitive);
        }

        BasicStringView SubStringView(SizeType codepointStart) const
        {
            return SubStringView(codepointStart, GetCodepointLength() - codepointStart);
        }

        BasicStringView SubStringView(SizeType codepointStart, SizeType codepointLength) const
        {
            return CodepointToCharIndex(codepointStart)
                .Map([&](SizeType start) {
                    SizeType charCount = 0;
                    for (SizeType i = 0; i < codepointLength; ++i)
                        charCount += EncodingType::GetCharCount(GetData() + start + charCount);

                    return View(GetData() + start, charCount);
                })
                .GetOr(Empty);
        }

        int Compare(const BasicStringView& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        template <typename TAllocator>
        int Compare(const BasicString<TEncoding, TAllocator>& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        int Compare(const CharType* string, bool caseSensitive = true) const
        {
            return Compare(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        int Compare(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::Compare(mString, length, string, length, caseSensitive);
        }

        bool StartsWith(CharType search, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mString[0], search, caseSensitive);
        }

        bool StartsWith(BasicStringView search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        template <typename TAllocator>
        bool StartsWith(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        bool StartsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, string, length, caseSensitive);
        }

        bool EndsWith(CharType search, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mString[mLength - 1], search, caseSensitive);
        }

        bool EndsWith(BasicStringView search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        template <typename TAllocator>
        bool EndsWith(const BasicString<TEncoding, TAllocator>& search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
        }

        bool EndsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mString, mLength, string, length, caseSensitive);
        }

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
         * \brief Get the number of UTF codepoint available in this StringView
         * The number of codepoint may vary depending on the decoder used internally.
         * If the StringView type does not support surrogates, this function is the same as calling GetLength.
         *
         * \return The number of code point
         */
        SizeType GetCodepointLength() const
        {
            return EncodingType::GetCharCount(mString, mLength);
        }

        const CharType* GetData() const
        {
            return mString;
        }

        const CharType& operator[](SizeType index) const
        {
            return mString[index];
        }

    private:

        CharType* FetchCodepoint(SizeType codepointIndex)
        {
            if (auto codepoint = TryFetchCodepoint(codepointIndex); !codepoint.IsEmpty())
                return codepoint.Get();

            ExceptionUtils::ThrowOutOfRange(codepointIndex, GetCodepointLength());
        }

        const CharType* FetchCodepoint(SizeType codepointIndex) const
        {
            if (auto codepoint = TryFetchCodepoint(codepointIndex); !codepoint.IsEmpty())
                return codepoint.Get();

            ExceptionUtils::ThrowOutOfRange(codepointIndex, GetCodepointLength());
        }

        Optional<CharType*> TryFetchCodepoint(SizeType codepointIndex)
        {
            return CodepointToCharIndex(codepointIndex).Map([&](SizeType index) {
                return GetData() + index;
            });
        }

        Optional<const CharType*> TryFetchCodepoint(SizeType codepointIndex) const
        {
            return CodepointToCharIndex(codepointIndex).Map([&](SizeType index) {
                return GetData() + index;
            });
        }

        Optional<SizeType> CodepointToCharIndex(SizeType codepointIndex) const
        {
            for (SizeType i = 0; i < GetLength(); i += EncodingType::GetCharCount(GetData() + i))
            {
                if (i == codepointIndex)
                    return Optional<SizeType>::Of(i);
            }

            return Optional<SizeType>::Empty();
        }

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
    template <typename TEncoding, typename TAllocator = SizedHeapAllocator<32>>
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
         * \brief
         *
         * \tparam Args
         *
         * \param format
         * \param args
         *
         * \return
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
            ;
            using OtherCharType = typename OtherEncoder::CharType;
            using OtherSizeType = typename OtherEncoder::SizeType;

            return ConvertFrom<OtherEncoder>(other, StringUtils<OtherCharType, OtherSizeType>::GetLength(other));
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
        static BasicString ConvertFrom(const typename TOtherEncoder::CharType* other, typename TOtherEncoder::SizeType otherLength)
        {
            using OtherEncoder = TOtherEncoder;
            using OtherSizeType = typename OtherEncoder::SizeType;

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
         * \brief Appends a single character to the end of the string
         *
         * Adds the given character to the end of the string, extending its length by one.
         * A null terminator is maintained automatically.
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
         * If the provided view is not empty, its contents are appended to this string’s buffer.
         * The operation does not modify the source view.
         *
         * \param string The string view to append
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
         * If the source string is not empty, its character data is appended to the end of this
         * string. A null terminator is maintained automatically.
         *
         * \param string The BasicString to append
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& Append(const BasicString& string)
        {
            return string.IsEmpty() ? *this : Append(string.GetData(), string.GetLength());
        }

        /**
         * \brief Appends a null-terminated character array to this string

         * Determines the length of the provided null-terminated string and appends its contents
         * to this string. A null terminator is maintained automatically.
         *
         * \param string A pointer to the null-terminated character array to append
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
         * The function ensures that the resulting string remains null-terminated.
         *
         * \param string A pointer to the character data to append
         * \param length The number of characters to append
         *
         * \return A reference to this BasicString after modification
         */
        BasicString& Append(const CharType* string, SizeType length)
        {
            if (!IsEmpty())
                mCharacters.PopLast();

            const CharType* begin = mCharacters.GetData();
            const CharType* end = begin + GetLength();

            if (string >= begin && string < end)
            {
                CharType* copy = COCKTAIL_STACK_ALLOC(CharType, length);
                AllocatorUtils::CopyRange(length, copy, string);

                mCharacters.Append(copy, length);
            }
            else
            {
                mCharacters.Append(string, length);
            }
            mCharacters.Add('\0');

            return *this;
        }

        BasicString& Prepend(CharType string)
        {
            return Prepend(&string, 1);
        }

        BasicString& Prepend(View string)
        {
            return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
        }

        BasicString& Prepend(const BasicString& string)
        {
            return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
        }

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
                AllocatorUtils::CopyRange(length, copy, string);

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

        CharType& At(SizeType index)
        {
            return mCharacters.At(index);
        }

        const CharType& At(SizeType index) const
        {
            return mCharacters.At(index);
        }

        Optional<CharType&> TryAt(SizeType index)
        {
            if (index >= GetLength())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(mCharacters[index]);
        }

        Optional<const CharType&> TryAt(SizeType index) const
        {
            if (index >= GetLength())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(mCharacters[index]);
        }

        CharType& First()
        {
            if (Optional<CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const CharType& First() const
        {
            if (Optional<const CharType&> first = TryFirst(); !first.IsEmpty())
                return first.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<CharType&> TryFirst()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(0));
        }

        Optional<const CharType&> TryFirst() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(0));
        }

        CharType& Last()
        {
            if (Optional<CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        const CharType& Last() const
        {
            if (Optional<const CharType&> last = TryLast(); !last.IsEmpty())
                return last.Get();

            ExceptionUtils::ThrowEmptyContainer();
        }

        Optional<CharType&> TryLast()
        {
            if (IsEmpty())
                return Optional<CharType&>::Empty();

            return Optional<CharType&>::Of(At(GetLength() - 1));
        }

        Optional<const CharType&> TryLast() const
        {
            if (IsEmpty())
                return Optional<const CharType&>::Empty();

            return Optional<const CharType&>::Of(At(GetLength() - 1));
        }

        Optional<SizeType> FindFirst(CharType search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(View search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(const BasicString& search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(const CharType* search, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return FindFirst(search, StringUtils<CharType, SizeType>::GetLength(search), startIndex, caseSensitive);
        }

        Optional<SizeType> FindFirst(const CharType* search, SizeType length, SizeType startIndex = 0, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, length, startIndex, caseSensitive);
        }

        Optional<SizeType> FindLast(CharType search, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, caseSensitive);
        }

        Optional<SizeType> FindLast(View search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        Optional<SizeType> FindLast(const BasicString& search, bool caseSensitive = true) const
        {
            return FindLast(search.GetData(), search.GetLength(), caseSensitive);
        }

        Optional<SizeType> FindLast(const CharType* search, bool caseSensitive = true) const
        {
            return FindLast(search, StringUtils<CharType, SizeType>::GetLength(search), caseSensitive);
        }

        Optional<SizeType> FindLast(const CharType* search, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, length, caseSensitive);
        }

        CharType PopFirst()
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mCharacters.PopFirst();
        }

        CharType PopLast()
        {
            if (IsEmpty())
                ExceptionUtils::ThrowEmptyContainer();

            return mCharacters.RemoveAt(GetLength() - 1);
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

        Utf32Char CodepointAt(SizeType codepointIndex) const
        {
            if (auto codepoint = TryCodepointAt(codepointIndex); !codepoint.IsEmpty())
                return codepoint.Get();

            ExceptionUtils::ThrowCodepointDecodingException(codepointIndex);
        }

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

        SizeType OffsetByCodepoint(SizeType start, SizeType codepointCount) const
        {
            if (auto offset = TryOffsetByCodepoint(start, codepointCount); !offset.IsEmpty())
                return offset.Get();

            ExceptionUtils::ThrowOutOfRange(start, GetLength());
        }

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

        BasicString SubString(SizeType start) const
        {
            return BasicString::FromView(SubStringView(start));
        }

        BasicString SubString(SizeType start, SizeType length) const
        {
            return BasicString::FromView(SubStringView(start, length));
        }

        View SubStringView(SizeType start) const
        {
            return SubStringView(start, GetLength() - start);
        }

        View SubStringView(SizeType start, SizeType length) const
        {
            return View(GetData() + start, std::min(length, GetLength() - start));
        }

        int Compare(View stringView, bool caseSensitive = true) const
        {
            return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
        }

        int Compare(const BasicString& other, bool caseSensitive = true) const
        {
            return Compare(other.GetData(), other.GetLength(), caseSensitive);
        }

        int Compare(const CharType* string, bool caseSensitive = true) const
        {
            return Compare(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        int Compare(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::Compare(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        bool StartsWith(CharType character, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(mCharacters.First(), character, caseSensitive);
        }

        bool StartsWith(View stringView, bool caseSensitive = true) const
        {
            return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
        }

        bool StartsWith(const BasicString& other, bool caseSensitive = true) const
        {
            return StartsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        bool StartsWith(const CharType* string, bool caseSensitive = true) const
        {
            return StartsWith(string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        bool StartsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::StartsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        bool EndsWith(CharType character, bool caseSensitive = true) const
        {
            return IsEmpty() ? false : StringUtils<CharType, SizeType>::Equal(character, mCharacters[GetLength() - 1], caseSensitive);
        }

        bool EndsWith(View other, bool caseSensitive = true) const
        {
            return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        bool EndsWith(const BasicString& other, bool caseSensitive = true) const
        {
            return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
        }

        bool EndsWith(const CharType* string, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, StringUtils<CharType, SizeType>::GetLength(string), caseSensitive);
        }

        bool EndsWith(const CharType* string, SizeType length, bool caseSensitive = true) const
        {
            return StringUtils<CharType, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
        }

        Array<BasicString, TAllocator> Split(CharType separator) const
        {
            Array<BasicString, TAllocator> splits;
            Split(splits, separator);

            return splits;
        }

        void Split(Array<BasicString, TAllocator>& splits, CharType separator) const
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

        BasicString& ToLowercase()
        {
            mCharacters.TransformInPlace([](CharType character) {
                return CharUtils<CharType>::ToLower(character);
            });

            return *this;
        }

        BasicString& ToUppercase()
        {
            mCharacters.TransformInPlace([](CharType character) {
                return CharUtils<CharType>::ToUpper(character);
            });

            return *this;
        }

        void Clear()
        {
            mCharacters.Clear();
        }

        void Reserve(SizeType size)
        {
            mCharacters.Reserve(size);
        }

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

        CharType* GetData()
        {
            return IsEmpty() ? nullptr : mCharacters.GetData();
        }

        const CharType* GetData() const
        {
            return IsEmpty() ? nullptr : mCharacters.GetData();
        }

        CharType& operator[](SizeType index)
        {
            return mCharacters[index];
        }

        const CharType& operator[](SizeType index) const
        {
            return mCharacters[index];
        }

        BasicString& operator+=(View rhs)
        {
            return Append(rhs);
        }

        BasicString& operator+=(const BasicString& rhs)
        {
            return Append(rhs);
        }

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
    using AnsiStringView = AsciiString::View;

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
