#include <cctype>

#include <Cocktail/Core/String.hpp>

namespace Ck
{
	namespace
	{
		bool CompareTextChar(TextChar lhs, TextChar rhs, bool caseSensitive)
		{
			return caseSensitive ? lhs == rhs : std::tolower(lhs) == std::tolower(rhs);
		}
	}

	const String String::Empty;

	String::SizeType String::Length(const TextChar* string)
	{
		assert(string);

		SizeType length = 0;
		while (string[length] != '\0') 
			length++;
		
		return length;
	}

	String::String(const TextChar* string) :
		String(string, Length(string))
	{
		/// Nothing
	}

	String::String(const TextChar* string, SizeType length)
	{
		mCharacters.Append(string, length);
		mCharacters.Add('\0');
	}

	String& String::Append(const String& other)
	{
		return other.IsEmpty() ? *this : Append(other.GetData(), other.GetLength());
	}

	String& String::Append(const TextChar* string)
	{
		return Append(string, Length(string));
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
		mCharacters.Add('\0');

		return *this;
	}

	String& String::Prepend(const String& other)
	{
		return other.IsEmpty() ? *this : Prepend(other.GetData(), other.GetLength());
	}

	String& String::Prepend(const TextChar* string)
	{
		return Prepend(string, Length(string));
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
			mCharacters.Add('\0');

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

	int String::Compare(StringView stringView, bool caseSensitive) const
	{
		return 0;
	}

	int String::Compare(const String& other, bool caseSensitive) const
	{
		return Compare(other.GetData(), other.GetLength(), caseSensitive);
	}

	int String::Compare(const TextChar* string, bool caseSensitive) const
	{
		return Compare(string, Length(string), caseSensitive);
	}

	int String::Compare(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		SizeType minLen = std::min(GetLength(), length);

		for (SizeType i = 0; i < minLen; i++)
		{
			if (!CompareTextChar(mCharacters[i], string[i], caseSensitive))
				return (mCharacters[i] < string[i]) ? -1 : 1;
		}

		if (GetLength() < length) 
			return -1;

		if (GetLength() > length) 
			return 1;

		return 0;

	}

	bool String::StartsWith(TextChar character, bool caseSensitive) const
	{
		return IsEmpty() ? false : CompareTextChar(character, mCharacters.First(), caseSensitive);
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
		return StartsWith(string, Length(string), caseSensitive);
	}

	bool String::StartsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		if (GetLength() < length)
			return false;

		for (SizeType i = 0; i < length; i++)
		{
			if (!CompareTextChar(mCharacters[i], string[i], caseSensitive))
				return false;
		}

		return true;
	}

	bool String::EndsWith(TextChar character, bool caseSensitive) const
	{
		return IsEmpty() ? false : CompareTextChar(character, mCharacters.Last(), caseSensitive);
	}

	bool String::EndsWith(const String& other, bool caseSensitive) const
	{
		if (other.IsEmpty())
			return false;

		return EndsWith(other.GetData(), other.GetLength(), caseSensitive);
	}

	bool String::EndsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		if (GetLength() < length)
			return false;

		for (SizeType i = 0; i < length; i++)
		{
			if (!CompareTextChar(mCharacters[GetLength() - length + i], string[i], caseSensitive))
				return false;
		}

		return true;
	}

	String& String::ToLowercase()
	{
		mCharacters.TransformInPlace([](TextChar character) {
			return std::tolower(character);
		});

		return *this;
	}

	String& String::ToUppercase()
	{
		mCharacters.TransformInPlace([](TextChar character) {
			return std::toupper(character);
		});

		return *this;
	}

	bool String::IsEmpty() const
	{
		return GetLength() == 0;
	}

	String::SizeType String::GetLength() const
	{
		return mCharacters.IsEmpty() ? 0 : mCharacters.GetSize() - 1;
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
