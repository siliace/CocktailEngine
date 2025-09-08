#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/StringView.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
	const StringView StringView::Empty;

	StringView::StringView() :
		mString(nullptr),
		mLength(0)
	{
		/// Nothing
	}

	StringView::StringView(const String& string) :
		mString(string.GetData()),
		mLength(string.GetLength())
	{
		/// Nothing
	}

	StringView::StringView(const TextChar* string) :
		StringView(string, StringUtils<TextChar>::GetLength(string))
	{
		/// Nothing
	}

	StringView::StringView(const TextChar* string, SizeType length) :
		mString(string),
		mLength(length)
	{
		/// Nothing
	}

	StringView& StringView::operator=(StringView other)
	{
		mString = other.mString;
		mLength = other.mLength;

		return *this;
	}

	StringView& StringView::operator=(const String& string)
	{
		mString = string.GetData();
		mLength = string.GetLength();

		return *this;
	}

	StringView& StringView::operator=(const TextChar* string)
	{
		mString = string;
		mLength = StringUtils<TextChar>::GetLength(string);

		return *this;
	}

	const TextChar& StringView::At(SizeType index) const
	{
		if (index >= mLength)
			ExceptionUtils::ThrowOutOfRange(index, mLength);

		return mString[index];
	}

	Optional<const TextChar&> StringView::TryAt(SizeType index) const
	{
		if (index >= mLength)
			Optional<const TextChar&>::Empty();

		return Optional<const TextChar&>::Of(mString[index]);
	}

	Optional<StringView::SizeType> StringView::FindFirst(TextChar search, SizeType startIndex, bool caseSensitive) const
	{
		return StringUtils<TextChar>::FindFirst(mString, mLength, search, startIndex, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindFirst(StringView search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindFirst(const String& search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search.GetData(), search.GetLength(), startIndex, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindFirst(const TextChar* search, SizeType startIndex, bool caseSensitive) const
	{
		return FindFirst(search, StringUtils<TextChar>::GetLength(search), startIndex, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindFirst(const TextChar* search, SizeType length, SizeType startIndex, bool caseSensitive) const
	{
		return StringUtils<TextChar>::FindFirst(mString, mLength, search, length, startIndex, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindLast(TextChar search, bool caseSensitive) const
	{
		return StringUtils<TextChar>::FindLast(mString, mLength, search, caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindLast(StringView search, bool caseSensitive) const
	{
		return FindLast(search.GetData(), search.GetLength(), caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindLast(const String& search, bool caseSensitive) const
	{
		return FindLast(search.GetData(), search.GetLength(), caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindLast(const TextChar* search, bool caseSensitive) const
	{
		return FindLast(search, StringUtils<TextChar>::GetLength(search), caseSensitive);
	}

	Optional<StringView::SizeType> StringView::FindLast(const TextChar* search, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar>::FindLast(mString, mLength, search, length, caseSensitive);
	}

	StringView StringView::SubStringView(SizeType start) const
	{
		return SubStringView(start, mLength - start);
	}

	StringView StringView::SubStringView(SizeType start, SizeType length) const
	{
		return StringView(mString + start, length);
	}

	int StringView::Compare(StringView other, bool caseSensitive) const
	{
		return Compare(other.GetData(), other.GetLength(), caseSensitive);
	}

	int StringView::Compare(const String& other, bool caseSensitive) const
	{
		return Compare(other.GetData(), other.GetLength(), caseSensitive);
	}

	int StringView::Compare(const TextChar* string, bool caseSensitive) const
	{
		return Compare(string, StringUtils<TextChar>::GetLength(string), caseSensitive);
	}

	int StringView::Compare(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar>::Compare(mString, length, string, length, caseSensitive);
	}

	bool StringView::StartsWith(TextChar search, bool caseSensitive) const
	{
		return IsEmpty() ? false : StringUtils<TextChar>::Equal(mString[0], search, caseSensitive);
	}

	bool StringView::StartsWith(StringView search, bool caseSensitive) const
	{
		return StringUtils<TextChar>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
	}

	bool StringView::StartsWith(const String& search, bool caseSensitive) const
	{
		return StringUtils<TextChar>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
	}

	bool StringView::StartsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar>::StartsWith(mString, mLength, string, length, caseSensitive);
	}

	bool StringView::EndsWith(TextChar search, bool caseSensitive) const
	{
		return IsEmpty() ? false : StringUtils<TextChar>::Equal(mString[mLength - 1], search, caseSensitive);
	}

	bool StringView::EndsWith(StringView search, bool caseSensitive) const
	{
		return StringUtils<TextChar>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
	}

	bool StringView::EndsWith(const String& search, bool caseSensitive) const
	{
		return StringUtils<TextChar>::StartsWith(mString, mLength, search.GetData(), search.GetLength(), caseSensitive);
	}

	bool StringView::EndsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return StringUtils<TextChar>::EndsWith(mString, mLength, string, length, caseSensitive);
	}

	bool StringView::IsNumeric() const
	{
		return StringUtils<TextChar>::IsNumeric(mString, mLength);
	}

	bool StringView::IsEmpty() const
	{
		return mLength == 0;
	}

	StringView::SizeType StringView::GetLength() const
	{
		return mLength;
	}

	const TextChar* StringView::GetData() const
	{
		return mString;
	}

	const TextChar& StringView::operator[](SizeType index) const
	{
		return mString[index];
	}

	bool operator==(StringView lhs, StringView rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(StringView lhs, const String& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(StringView lhs, const TextChar* rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const TextChar* lhs, StringView rhs)
	{
		return rhs.Compare(lhs) == 0;
	}

	bool operator!=(StringView lhs, StringView rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(StringView lhs, const String& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(StringView lhs, const TextChar* rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const TextChar* lhs, StringView rhs)
	{
		return !(lhs == rhs);
	}
}

namespace std
{
	size_t hash<Ck::StringView>::operator()(const Ck::StringView& stringView) const noexcept
	{
		size_t hash = 0;
		for (Ck::StringView::SizeType i = 0; i < stringView.GetLength(); i++)
			Ck::HashCombine(hash, stringView[i]);

		return hash;
	}
}
