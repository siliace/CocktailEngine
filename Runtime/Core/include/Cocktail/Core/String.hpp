#ifndef COCKTAIL_CORE_STRING_HPP
#define COCKTAIL_CORE_STRING_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/StringFormater.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>
#include <Cocktail/Core/Utility/Encoding/AsciiEncoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf16Encoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf32Encoder.hpp>
#include <Cocktail/Core/Utility/Encoding/Utf8Encoder.hpp>

namespace Ck
{
	namespace Detail
	{
		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		class BasicString;

		/**
		 * \brief
		 */
		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		class BasicStringView
		{
		public:

			/**
			 * \brief
			 */
			using CharType = TChar;

			/**
			 * \brief
			 */
			using SizeType = TSize;

			/**
			 * \brief The decoder type used to convert raw string data from and to UTF Codepoints
			 */
			using EncoderType = TEncoder<CharType, SizeType>;

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
			BasicStringView(const BasicString<TChar, TEncoder, TAllocator>& string) :
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
			BasicStringView operator=(const BasicString<TChar, TEncoder, TAllocator>& string)
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

			Optional<const CharType&> TryAt(SizeType index) const
			{
				if (index >= mLength)
					Optional<const CharType&>::Empty();

				return Optional<const CharType&>::Of(mString[index]);
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
			Optional<SizeType> FindFirst(const BasicString<TChar, TEncoder, TAllocator>& search, SizeType startIndex = 0, bool caseSensitive = true) const
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
			Optional<SizeType> FindLast(const BasicString<TChar, TEncoder, TAllocator>& search, bool caseSensitive = true) const
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
				return CodepointToCharIndex(codepointStart).Map([&](SizeType start) {
					SizeType charCount = 0;
					for (SizeType i = 0; i < codepointLength; ++i)
						charCount += EncoderType::GetCharCount(GetData() + start + charCount);

					return View(GetData() + start, charCount);
				}).GetOr(Empty);
			}

			int Compare(const BasicStringView& other, bool caseSensitive = true) const
			{
				return Compare(other.GetData(), other.GetLength(), caseSensitive);
			}

			template <typename TAllocator>
			int Compare(const BasicString<TChar, TEncoder, TAllocator>& other, bool caseSensitive = true) const
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
			bool StartsWith(const BasicString<TChar, TEncoder, TAllocator>& search, bool caseSensitive = true) const
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
			bool EndsWith(const BasicString<TChar, TEncoder, TAllocator>& search, bool caseSensitive = true) const
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
				return EncoderType::GetCharCount(mString, mLength);
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
				for (SizeType i = 0; i < GetLength(); i += EncoderType::GetCharCount(GetData() + i))
				{
					if (i == codepointIndex)
						return Optional<SizeType>::Of(i);
				}

				return Optional<SizeType>::Empty();
			}

			const CharType* mString;
			SizeType mLength;
		};

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		const BasicStringView<TChar, TEncoder, TSize> BasicStringView<TChar, TEncoder, TSize>::Empty;

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator==(BasicStringView<TChar, TEncoder, TSize> lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return lhs.Compare(rhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator==(BasicStringView<TChar, TEncoder, TSize> lhs, const TChar* rhs)
		{
			return lhs.Compare(rhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator==(const TChar* lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return rhs.Compare(lhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator!=(BasicStringView<TChar, TEncoder, TSize> lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator!=(BasicStringView<TChar, TEncoder, TSize> lhs, const TChar* rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TSize>
		bool operator!=(const TChar* lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
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
		 * \tparam TChar The character type used to store individual string elements
		 * \tparam TEncoder The encoder template used to convert between raw string data and UTF codepoints
		 * \tparam TAllocator The memory allocator used for internal storage
		 */
		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator = SizedHeapAllocator<32>>
		class BasicString
		{
		public:

			/**
			 * \brief The character type used by this string
			 */
			using CharType = TChar;

			/**
			 * \brief The size type used to represent the length or capacity of the string.
			 */
			using SizeType = typename Array<CharType, TAllocator>::SizeType;

			/**
			 * \brief The encoder type used to convert raw string data from and to UTF codepoints
			 */
			using EncoderType = TEncoder<CharType, SizeType>;

			/**
			 * \brief Flag indicating whether this string type supports surrogate pairs
		     *
	         * If true, the encoder supports handling surrogate pairs (e.g., in UTF-16 encoding),
	         * allowing representation of characters outside the Basic Multilingual Plane (BMP).
			 */
			static constexpr bool SupportSurrogate = EncoderType::SupportSurrogate;

			/**
			 * \brief A lightweight, non-owning view type referencing string data
			 *
			 * The View type allows read-only access to string content without copying or
			 * transferring ownership. It is useful for efficient substring operations and
			 * parameter passing.
			 */
			using View = BasicStringView<CharType, TEncoder, SizeType>;

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
			 * \tparam TOtherChar The character type used by the source string
			 * \tparam TOtherEncoder The encoder template used by the source string
			 * \tparam TOtherAllocator The allocator type used by the source string
			 *
			 * \param other The source string to convert from
			 *
			 * \return A new string instance containing the converted text
			 */
			template <typename TOtherChar, template <class, class> typename TOtherEncoder, typename TOtherAllocator>
			static BasicString Convert(const BasicString<TOtherChar, TOtherEncoder, TOtherAllocator>& other)
			{
				return Convert(typename BasicString<TOtherChar, TOtherEncoder, TOtherAllocator>::View(other));
			}

			/**
			 * \brief Converts a string view with a different character or encoder type into this string type
			 *
			 * This function accepts a pointer to a string view encoded with a different
			 * encoder and character type, decodes each codepoint to UTF-32, and re-encodes it using
			 * this string’s encoder. The resulting text is stored in a new String instance.
			 *
			 * \tparam TOtherChar The character type used by the source view
			 * \tparam TOtherEncoder The encoder template used by the source view
			 * \tparam TOtherSize The size type used by the source view
			 *
			 * \param other The source BasicStringView to convert from
			 *
			 * \return A new BasicString instance containing the converted text
			 */
			template <typename TOtherChar, template <class, class> typename TOtherEncoder, typename TOtherSize>
			static BasicString Convert(const BasicStringView<TOtherChar, TOtherEncoder, TOtherSize>& other)
			{
				return ConvertFrom<TOtherEncoder, TOtherChar, TOtherSize>(other.GetData(), other.GetLength());
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
			 * \tparam TOtherChar The character type of the source string
			 *
			 * \param other A pointer to the null-terminated source string to convert
			 *
			 * \return A new String instance containing the converted text
			 *
			 * \note This function assumes that the input string is properly null-terminated
			 */
			template <template <class, class> typename TOtherEncoder, typename TOtherChar, typename TOtherSize = SizeType>
			static BasicString ConvertFrom(const TOtherChar* other)
			{
				return ConvertFrom<TOtherEncoder, TOtherChar, TOtherSize>(other, StringUtils<TOtherChar, TOtherSize>::GetLength(other));
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
			 * \tparam TOtherChar The character type of the source string
			 *
			 * \param other A pointer to the null-terminated source string to convert
			 *
			 * \return A new String instance containing the converted text
			 *
			 * \note This function assumes that the input string is properly null-terminated
			 */
			template <template <class, class> typename TOtherEncoder, typename TOtherChar, typename TOtherSize = SizeType>
			static BasicString ConvertFrom(const TOtherChar* other, TOtherSize otherLength)
			{
				using OtherEncoder = TOtherEncoder<TOtherChar, TOtherSize>;

				BasicString converted;
				converted.Reserve(otherLength * EncoderType::MaxCodepointEncodingLength);

				CharType buffer[EncoderType::MaxCodepointEncodingLength];
				for (TOtherSize i = 0; i < otherLength;)
				{
					Utf32Char decoded;
					TOtherSize decodedCound = OtherEncoder::Decode(&other[i], decoded);
					if (decodedCound == 0)
						ExceptionUtils::ThrowCodepointDecodingException(i);

					i += decodedCound;

					SizeType encoded = EncoderType::Encode(decoded, buffer);
					if (encoded == 0)
						ExceptionUtils::ThrowCodepointEncodingException(decoded);

					converted.Append(buffer, encoded);
				}

				return converted;
			}

			static BasicString FromView(View StringView)
			{
				return BasicString(StringView.GetData(), StringView.GetLength());
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

			Utf32Char CodepointAt(SizeType codepointIndex) const
			{
				Utf32Char out;
				if (EncoderType::Decode(FetchCodepoint(codepointIndex), out) == 0)
					ExceptionUtils::ThrowCodepointDecodingException(codepointIndex);

				return out;
			}

			Optional<Utf32Char> TryCodepointAt(SizeType codepointIndex) const
			{
				auto codepoint = TryFetchCodepoint(codepointIndex);
				if (codepoint.IsEmpty())
					return Optional<Utf32Char>::Empty();

				Utf32Char out;
				if (EncoderType::Decode(codepoint.Get(), out) == 0)
					return Optional<Utf32Char>::Empty();

				return Optional<Utf32Char>::Of(out);
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

			BasicString SubString(SizeType codepointStart) const
			{
				return BasicString::FromView(
					SubStringView(codepointStart)
				);
			}

			BasicString SubString(SizeType codepointStart, SizeType codepointLength) const
			{
				return BasicString::FromView(
					SubStringView(codepointStart, codepointLength)
				);
			}

			View SubStringView(SizeType codepointStart) const
			{
				return SubStringView(codepointStart, GetCodepointLength() - codepointStart);
			}

			View SubStringView(SizeType codepointStart, SizeType codepointLength) const
			{
				return CodepointToCharIndex(codepointStart).Map([&](SizeType start) {
					SizeType charCount = 0;
					for (SizeType i = 0; i < codepointLength; ++i)
						charCount += EncoderType::GetCharCount(GetData() + start + charCount);

					return View(GetData() + start, charCount);
				}).GetOr(View::Empty);
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

			/**
			 * \brief Get the number of UTF codepoint available in this String
			 * The number of codepoint may vary depending on the decoder used internally.
			 * If the String type does not support surrogates, this function is the same as calling GetLength.
			 *
			 * \return The number of code point
			 */
			SizeType GetCodepointLength() const
			{
				return IsEmpty() ? 0 : EncoderType::GetCharCount(GetData(), GetLength());
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
				for (SizeType i = 0; i < GetLength(); i += EncoderType::GetCharCount(GetData() + i))
				{
					if (i == codepointIndex)
						return Optional<SizeType>::Of(i);
				}

				return Optional<SizeType>::Empty();
			}

			Array<CharType> mCharacters;
		};

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		const BasicString<TChar, TEncoder, TAllocator> BasicString<TChar, TEncoder, TAllocator>::Empty;

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator==(const BasicString<TChar, TEncoder, TAllocator>& lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return lhs.Compare(rhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator==(const BasicString<TChar, TEncoder, TAllocator>& lhs, const TChar* rhs)
		{
			return lhs.Compare(rhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator==(const TChar* lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return rhs.Compare(lhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		bool operator==(const BasicString<TChar, TEncoder, TAllocator>& lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return lhs.Compare(rhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		bool operator==(BasicStringView<TChar, TEncoder, TSize> lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return rhs.Compare(lhs) == 0;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator!=(const BasicString<TChar, TEncoder, TAllocator>& lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator!=(const BasicString<TChar, TEncoder, TAllocator>& lhs, const TChar* rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		bool operator!=(const TChar* lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		bool operator!=(const BasicString<TChar, TEncoder, TAllocator>& lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		bool operator!=(BasicStringView<TChar, TEncoder, TSize> lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return !(lhs == rhs);
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		BasicString<TChar, TEncoder, TAllocator> operator+(const BasicString<TChar, TEncoder, TAllocator>& lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return BasicString<TChar, TEncoder, TAllocator>(lhs) += rhs;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		BasicString<TChar, TEncoder, TAllocator> operator+(const TChar* lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return BasicString<TChar, TEncoder, TAllocator>(lhs) += rhs;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
		BasicString<TChar, TEncoder, TAllocator> operator+(const BasicString<TChar, TEncoder, TAllocator>& lhs, const TChar* rhs)
		{
			return BasicString<TChar, TEncoder, TAllocator>(lhs) += rhs;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		BasicString<TChar, TEncoder, TAllocator> operator+(BasicStringView<TChar, TEncoder, TSize> lhs, const BasicString<TChar, TEncoder, TAllocator>& rhs)
		{
			return BasicString<TChar, TEncoder, TAllocator>::FromView(lhs) += rhs;
		}

		template <typename TChar, template<class, class> typename TEncoder, typename TAllocator, typename TSize>
		BasicString<TChar, TEncoder, TAllocator> operator+(const BasicString<TChar, TEncoder, TAllocator>& lhs, BasicStringView<TChar, TEncoder, TSize> rhs)
		{
			return BasicString<TChar, TEncoder, TAllocator>(lhs) += rhs;
		}
	}

	using AnsiString = Detail::BasicString<AnsiChar, AsciiEncoder>;
	using AnsiStringView = AnsiString::View;

	using Utf8String = Detail::BasicString<Utf8Char, Utf8Encoder>;
	using Utf8StringView = Utf8String::View;

	using Utf16String = Detail::BasicString<Utf16Char, Utf16Encoder>;
	using Utf16StringView = Utf16String::View;

	using Utf32String = Detail::BasicString<Utf32Char, Utf32Encoder>;
	using Utf32StringView = Utf32String::View;

#ifdef COCKTAIL_OS_WINDOWS
	using String = Detail::BasicString<TextChar, Utf16Encoder>;
	using StringView = String::View;
#else
	using String = Detail::BasicString<TextChar, Utf8Encoder>;
	using StringView = String::View;
#endif

	template <typename TChar, template<class, class> typename TEncoder, typename TSize>
	struct Formatter<Detail::BasicStringView<TChar, TEncoder, TSize>, void>
	{
		using StringViewType = Detail::BasicStringView<TChar, TEncoder, TSize>;
		using CharType = typename StringViewType::CharType;

		const CharType* Apply(const StringViewType& value) const noexcept
		{
			return value.GetData();
		}
	};

	template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
	struct Formatter<Detail::BasicString<TChar, TEncoder, TAllocator>, void>
	{
		using StringType = Detail::BasicString<TChar, TEncoder, TAllocator>;
		using CharType = typename StringType::CharType;

		const CharType* Apply(const StringType& value) const noexcept
		{
			return value.GetData();
		}
	};
}

namespace std
{
	template <typename TChar, template<class, class> typename TEncoder, typename TAllocator>
	struct hash<Ck::Detail::BasicString<TChar, TEncoder, TAllocator>>
	{
		using StringType = Ck::Detail::BasicString<TChar, TEncoder, TAllocator>;

		size_t operator()(const StringType& string) const noexcept
		{
			size_t hash = 0;
			for (typename StringType::SizeType i = 0; i < string.GetLength(); ++i)
				Ck::HashCombine(hash, string[i]);

			return hash;
		}
	};

	template <typename TChar, template<class, class> typename TEncoder, typename TSize>
	struct hash<Ck::Detail::BasicStringView<TChar, TEncoder, TSize>>
	{
		using StringViewType = Ck::Detail::BasicStringView<TChar, TEncoder, TSize>;

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
