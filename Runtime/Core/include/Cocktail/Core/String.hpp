#ifndef COCKTAIL_CORE_STRING_HPP
#define COCKTAIL_CORE_STRING_HPP

#include <locale>
#include <Cocktail/Core/Utility/StringFormater.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
	class StringView;

	class COCKTAIL_CORE_API String
	{
	public:

		using SizeType = Array<TextChar>::SizeType;

		/**
		 * \brief Utility constant value to return a reference/pointer to an empty String
		 */
		static const String Empty;

		template <typename... Args>
		static String Format(StringView format, Args&&... args)
		{
			String formatted;
			StringFormater<TextChar>::FormatTo(formatted.mCharacters, format.GetData(), std::forward<Args>(args)...);
			formatted.mCharacters.Add(CK_TEXT('\0'));

			return formatted;
		}

		String() = default;
		explicit String(StringView string);
		String(const TextChar* string);
		String(const TextChar* string, SizeType length);

		String& operator=(StringView string);
		String& operator=(const TextChar* string);

		String& Append(TextChar string);
		String& Append(StringView string);
		String& Append(const String& string);
		String& Append(const TextChar* string);
		String& Append(const TextChar* string, SizeType length);

		String& Prepend(TextChar string);
		String& Prepend(StringView string);
		String& Prepend(const String& string);
		String& Prepend(const TextChar* string);
		String& Prepend(const TextChar* string, SizeType length);

		TextChar& At(SizeType index);
		const TextChar& At(SizeType index) const;

		Optional<TextChar&> TryAt(SizeType index);
		Optional<const TextChar&> TryAt(SizeType index) const;

		Optional<SizeType> FindFirst(TextChar search, SizeType startIndex = 0, bool caseSensitive = true) const;
		Optional<SizeType> FindFirst(StringView search, SizeType startIndex = 0, bool caseSensitive = true) const;
		Optional<SizeType> FindFirst(const String& search, SizeType startIndex = 0, bool caseSensitive = true) const;
		Optional<SizeType> FindFirst(const TextChar* search, SizeType startIndex = 0, bool caseSensitive = true) const;
		Optional<SizeType> FindFirst(const TextChar* search, SizeType length, SizeType startIndex = 0, bool caseSensitive = true) const;

		Optional<SizeType> FindLast(TextChar search, bool caseSensitive = true) const;
		Optional<SizeType> FindLast(StringView search, bool caseSensitive = true) const;
		Optional<SizeType> FindLast(const String& search, bool caseSensitive = true) const;
		Optional<SizeType> FindLast(const TextChar* search, bool caseSensitive = true) const;
		Optional<SizeType> FindLast(const TextChar* search, SizeType length, bool caseSensitive = true) const;

		String SubString(SizeType start) const;
		String SubString(SizeType start, SizeType length) const;
		StringView SubStringView(SizeType start) const;
		StringView SubStringView(SizeType start, SizeType length) const;

		int Compare(StringView stringView, bool caseSensitive = true) const;
		int Compare(const String& other, bool caseSensitive = true) const;
		int Compare(const TextChar* string, bool caseSensitive = true) const;
		int Compare(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool StartsWith(TextChar character, bool caseSensitive = true) const;
		bool StartsWith(StringView stringView, bool caseSensitive = true) const;
		bool StartsWith(const String& other, bool caseSensitive = true) const;
		bool StartsWith(const TextChar* string, bool caseSensitive = true) const;
		bool StartsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool EndsWith(TextChar character, bool caseSensitive = true) const;
		bool EndsWith(StringView other, bool caseSensitive = true) const;
		bool EndsWith(const String& other, bool caseSensitive = true) const;
		bool EndsWith(const TextChar* string, bool caseSensitive = true) const;
		bool EndsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		template <typename TAllocator = SizedHeapAllocator<32>>
		Array<String, TAllocator> Split(TextChar separator) const
		{
			Array<String, TAllocator> splits;
			Split(splits, separator);

			return splits;
		}

		template <typename TAllocator = SizedHeapAllocator<32>>
		void Split(Array<String, TAllocator>& splits, TextChar separator) const
		{
			splits.Clear();

			for (unsigned int start = 0; start < GetLength();)
			{
				Array<TextChar> slice;
				Optional<unsigned int> index = mCharacters.FindIndex(separator, start);
				if (index.IsEmpty())
				{
					slice = mCharacters.Slice(start);
				}
				else
				{
					slice = mCharacters.Slice(start, index.Get() - start);
				}

				String split;
				split.mCharacters = std::move(slice);
				if (split.mCharacters.Last() != CK_TEXT('\0'))
					split.mCharacters.Add(CK_TEXT('\0'));

				start += split.GetLength() + 1;

				splits.Add(std::move(split));
			}
		}

		String& ToLowercase();
		String& ToUppercase();

		bool IsNumeric() const;

		void Clear();
		void Reserve(SizeType size);

		bool IsEmpty() const;
		SizeType GetLength() const;

		TextChar* GetData();
		const TextChar* GetData() const;

		TextChar& operator[](SizeType index);
		const TextChar& operator[](SizeType index) const;

	private:

		Array<TextChar> mCharacters;
	};

	COCKTAIL_CORE_API bool operator==(const String& lhs, StringView rhs);
	COCKTAIL_CORE_API bool operator==(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(const String& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator==(const TextChar* lhs, const String& rhs);

	COCKTAIL_CORE_API bool operator!=(const String& lhs, StringView rhs);
	COCKTAIL_CORE_API bool operator!=(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(const String& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator!=(const TextChar* lhs, const String& rhs);

	COCKTAIL_CORE_API String operator+(const String& lhs, StringView rhs);
	COCKTAIL_CORE_API String operator+(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API String operator+(const String& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API String operator+(const TextChar* lhs, const String& rhs);

	COCKTAIL_CORE_API String& operator+=(String& lhs, StringView rhs);
	COCKTAIL_CORE_API String& operator+=(String& lhs, const String& rhs);
	COCKTAIL_CORE_API String& operator+=(String& lhs, const TextChar* rhs);

	template <>
	struct Formater<String, void>
	{
		const TextChar* Apply(const String& value) const noexcept
		{
			return value.GetData();
		}
	};
}

namespace std
{
	template <>
	struct COCKTAIL_CORE_API hash<Ck::String>
	{
		size_t operator()(const Ck::String& string) const noexcept;
	};
}

#endif // COCKTAIL_CORE_STRING_HPP
