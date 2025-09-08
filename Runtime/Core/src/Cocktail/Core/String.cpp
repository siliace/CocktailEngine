#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Exception.hpp>

namespace Ck
{
	const String String::Empty;

	String::String(StringView string) :
		String(string.GetData(), string.GetLength())
	{
		/// Nothing
	}

	String::String(const TextChar* string) :
		String(string, StringUtils<TextChar, SizeType>::GetLength(string))
	{
		/// Nothing
	}

	String::String(const TextChar* string, SizeType length)
	{
		mCharacters.Append(string, length);
		mCharacters.Add(CK_TEXT('\0'));
	}

	String& String::operator=(StringView string)
	{
		if (string.GetData() != mCharacters.GetData())
		{
			mCharacters.Clear();
			mCharacters.Append(string.GetData(), string.GetLength());
			mCharacters.Add(CK_TEXT('\0'));
		}

		return *this;
	}

	String& String::operator=(const TextChar* string)
	{
		if (string != mCharacters.GetData())
		{
			mCharacters.Clear();
			mCharacters.Append(string, StringUtils<TextChar, SizeType>::GetLength(string));
			mCharacters.Add(CK_TEXT('\0'));
		}

		return *this;
	}

	String& String::Append(TextChar string)
	{
		return Append(&string, 1);
	}

	String& String::Append(StringView string)
	{
		return string.IsEmpty() ? *this : Append(string.GetData(), string.GetLength());
	}

	String& String::Append(const String& string)
	{
		return string.IsEmpty() ? *this : Append(string.GetData(), string.GetLength());
	}

	String& String::Append(const TextChar* string)
	{
		return Append(string, StringUtils<TextChar, SizeType>::GetLength(string));
	}

	String& String::Append(const TextChar* string, SizeType length)
	{
		if (!IsEmpty())
			mCharacters.PopLast();

		const TextChar* begin = mCharacters.GetData();
		const TextChar* end = begin + GetLength();

		if (string >= begin && string < end)
		{
			TextChar* copy = COCKTAIL_STACK_ALLOC(TextChar, length);
			AllocatorUtils::CopyRange(length, copy, string);

			mCharacters.Append(copy, length);
		}
		else
		{
			mCharacters.Append(string, length);
		}
		mCharacters.Add(CK_TEXT('\0'));

		return *this;
	}

	String& String::Prepend(TextChar string)
	{
		return Prepend(&string, 1);
	}

	String& String::Prepend(StringView string)
	{
		return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
	}

	String& String::Prepend(const String& string)
	{
		return string.IsEmpty() ? *this : Prepend(string.GetData(), string.GetLength());
	}

	String& String::Prepend(const TextChar* string)
	{
		return Prepend(string, StringUtils<TextChar, SizeType>::GetLength(string));
	}

	String& String::Prepend(const TextChar* string, SizeType length)
	{
		const bool wasEmpty = IsEmpty();

		const TextChar* begin = mCharacters.GetData();
		const TextChar* end = begin + GetLength();

		if (string >= begin && string < end)
		{
			TextChar* copy = COCKTAIL_STACK_ALLOC(TextChar, length);
			AllocatorUtils::CopyRange(length, copy, string);

			mCharacters.Prepend(copy, length);
		}
		else
		{
			mCharacters.Prepend(string, length);
		}

		if (wasEmpty)
			mCharacters.Add(CK_TEXT('\0'));

		return *this;
	}

	TextChar& String::At(SizeType index)
	{
		assert(index < GetLength());
		return mCharacters[index];
	}

	const TextChar& String::At(SizeType index) const
	{
		assert(index < GetLength());
		return mCharacters[index];
	}

	Optional<TextChar&> String::TryAt(SizeType index)
	{
		if (index >= GetLength())
			return Optional<TextChar&>::Empty();

		return Optional<TextChar&>::Of(mCharacters[index]);
	}

	Optional<const TextChar&> String::TryAt(SizeType index) const
	{
		if (index >= GetLength())
			return Optional<const TextChar&>::Empty();

		return Optional<const TextChar&>::Of(mCharacters[index]);
	}

	Optional<String::SizeType> String::FindFirst(TextChar search, SizeType startIndex, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, startIndex, caseSensitive);
	}

	Optional<String::SizeType> String::FindFirst(StringView search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
	}

	Optional<String::SizeType> String::FindFirst(const String& search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
	}

	Optional<String::SizeType> String::FindFirst(const TextChar* search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search, StringUtils<TextChar, SizeType>::GetLength(search), startIndex, caseSensitive);
	}

	Optional<String::SizeType> String::FindFirst(const TextChar* search, SizeType length, SizeType startIndex, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::FindFirst(mCharacters.GetData(), GetLength(), search, length, startIndex, caseSensitive);
	}

	Optional<String::SizeType> String::FindLast(TextChar search, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, caseSensitive);
	}

	Optional<String::SizeType> String::FindLast(StringView search, bool caseSensitive) const
	{
		return FindLast(search.GetData(), search.GetLength(), caseSensitive);
	}

	Optional<String::SizeType> String::FindLast(const String& search, bool caseSensitive) const
	{
		return FindLast(search.GetData(), search.GetLength(), caseSensitive);
	}

	Optional<String::SizeType> String::FindLast(const TextChar* search, bool caseSensitive) const
	{
		return FindLast(search, StringUtils<TextChar, SizeType>::GetLength(search), caseSensitive);
	}

	Optional<String::SizeType> String::FindLast(const TextChar* search, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::FindLast(mCharacters.GetData(), GetLength(), search, length, caseSensitive);
	}

	String String::SubString(SizeType start) const
	{
		return SubString(start, GetLength() - start);
	}

	String String::SubString(SizeType start, SizeType length) const
	{
		String substring;
		substring.mCharacters = mCharacters.Slice(start, length);
		substring.mCharacters.Add(CK_TEXT('\0'));

		return substring;
	}

	StringView String::SubStringView(SizeType start) const
	{
		return SubStringView(start, GetLength() - start);
	}

	StringView String::SubStringView(SizeType start, SizeType length) const
	{
		return StringView(mCharacters.GetData() + start, length);
	}

	int String::Compare(StringView stringView, bool caseSensitive) const
	{
		return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
	}

	int String::Compare(const String& other, bool caseSensitive) const
	{
		return Compare(other.GetData(), other.GetLength(), caseSensitive);
	}

	int String::Compare(const TextChar* string, bool caseSensitive) const
	{
		return Compare(string, StringUtils<TextChar, SizeType>::GetLength(string), caseSensitive);
	}

	int String::Compare(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::Compare(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
	}

	bool String::StartsWith(TextChar character, bool caseSensitive) const
	{
		return IsEmpty() ? false : StringUtils<TextChar, SizeType>::Equal(mCharacters.First(), character, caseSensitive);
	}

	bool String::StartsWith(StringView stringView, bool caseSensitive) const
	{
		return Compare(stringView.GetData(), stringView.GetLength(), caseSensitive);
	}

	bool String::StartsWith(const String& other, bool caseSensitive) const
	{
		return StartsWith(other.GetData(), other.GetLength(), caseSensitive);
	}

	bool String::StartsWith(const TextChar* string, bool caseSensitive) const
	{
		return StartsWith(string, StringUtils<TextChar, SizeType>::GetLength(string), caseSensitive);
	}

	bool String::StartsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::StartsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
	}

	bool String::EndsWith(TextChar character, bool caseSensitive) const
	{
		return IsEmpty() ? false : StringUtils<TextChar, SizeType>::Equal(character, mCharacters[GetLength() - 1], caseSensitive);
	}

	bool String::EndsWith(StringView other, bool caseSensitive) const
	{
		return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
	}

	bool String::EndsWith(const String& other, bool caseSensitive) const
	{
		return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
	}

	bool String::EndsWith(const TextChar* string, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, StringUtils<TextChar, SizeType>::GetLength(string), caseSensitive);
	}

	bool String::EndsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar, SizeType>::EndsWith(mCharacters.GetData(), GetLength(), string, length, caseSensitive);
	}

	String& String::ToLowercase()
	{
		mCharacters.TransformInPlace([](TextChar character) {
			return CharUtils<TextChar>::ToLower(character);
		});

		return *this;
	}

	String& String::ToUppercase()
	{
		mCharacters.TransformInPlace([](TextChar character) {
			return CharUtils<TextChar>::ToUpper(character);
		});

		return *this;
	}

	bool String::IsNumeric() const
	{
		return StringUtils<TextChar, SizeType>::IsNumeric(GetData(), GetLength());
	}

	void String::Clear()
	{
		mCharacters.Clear();
	}

	void String::Reserve(SizeType size)
	{
		mCharacters.Reserve(size);
	}

	bool String::IsEmpty() const
	{
		return GetLength() == 0;
	}

	String::SizeType String::GetLength() const
	{
		return mCharacters.IsEmpty() ? 0 : mCharacters.GetSize() - 1;
	}

	TextChar* String::GetData()
	{
		return IsEmpty() ? nullptr : mCharacters.GetData();
	}

	const TextChar* String::GetData() const
	{
		return IsEmpty() ? nullptr : mCharacters.GetData();
	}

	TextChar& String::operator[](SizeType index)
	{
		return mCharacters[index];
	}

	const TextChar& String::operator[](SizeType index) const
	{
		return mCharacters[index];
	}

	bool operator==(const String& lhs, StringView rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const String& lhs, const String& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const String& lhs, const TextChar* rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const TextChar* lhs, const String& rhs)
	{
		return rhs.Compare(lhs) == 0;
	}

	bool operator!=(const String& lhs, StringView rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const String& lhs, const String& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const String& lhs, const TextChar* rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const TextChar* lhs, const String& rhs)
	{
		return !(lhs == rhs);
	}

	String operator+(const String& lhs, StringView rhs)
	{
		String lhs1(lhs);
		return lhs1 += rhs;
	}

	String operator+(const String& lhs, const String& rhs)
	{
		String result = lhs;
		return result += rhs;
	}

	String operator+(const String& lhs, const TextChar* rhs)
	{
		String result = lhs;
		return result += rhs;
	}

	String operator+(const TextChar* lhs, const String& rhs)
	{
		String result = lhs;
		return result += rhs;
	}

	String& operator+=(String& lhs, StringView rhs)
	{
		return lhs.Append(rhs);
	}

	String& operator+=(String& lhs, const String& rhs)
	{
		return lhs.Append(rhs);
	}

	String& operator+=(String& lhs, const TextChar* rhs)
	{
		return lhs.Append(rhs);
	}
}

namespace std
{
	size_t hash<Ck::String>::operator()(const Ck::String& string) const noexcept
	{
		size_t hash = 0;
		for (Ck::String::SizeType i = 0; i < string.GetLength(); i++)
			Ck::HashCombine(hash, string[i]);

		return hash;
	}
}
