#include <Cocktail/Core/StringView.hpp>

namespace Ck
{
	StringView::StringView() :
		mString(nullptr),
		mLength(0)
	{
		/// Nothing
	}

	StringView::StringView(const TextChar* string) :
		StringView(string, String::Length(string))
	{
	}

	StringView::StringView(const TextChar* string, SizeType length) :
		mString(string),
		mLength(length)
	{
		/// Nothing
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
			ExceptionUtils::ThrowOutOfRange(index, mLength);

		return Optional<const TextChar&>::Of(mString[index]);
	}

	int StringView::Compare(StringView stringView, bool caseSensitive) const
	{
		return 0;
	}

	int StringView::Compare(const String& other, bool caseSensitive) const
	{
		return 0;
	}

	int StringView::Compare(const TextChar* string, bool caseSensitive) const
	{
		return 0;
	}

	int StringView::Compare(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return 0;
	}

	bool StringView::StartsWith(TextChar character, bool caseSensitive)
	{
		return false;
	}

	bool StringView::StartsWith(StringView stringView, bool caseSensitive)
	{
		return false;
	}

	bool StringView::StartsWith(const String& other, bool caseSensitive) const
	{
		return false;
	}

	bool StringView::StartsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return false;
	}

	bool StringView::EndsWith(TextChar character, bool caseSensitive)
	{
		return false;
	}

	bool StringView::EndsWith(StringView stringView, bool caseSensitive)
	{
		return false;
	}

	bool StringView::EndsWith(const String& other, bool caseSensitive) const
	{
		return false;
	}

	bool StringView::EndsWith(const TextChar* string, SizeType length, bool caseSensitive) const
	{
		return false;
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

	bool operator==(const StringView& lhs, const StringView& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const StringView& lhs, const TextChar* rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const TextChar* lhs, const StringView& rhs)
	{
		return rhs.Compare(lhs) == 0;
	}

	bool operator==(const StringView& lhs, const String& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const String& lhs, const StringView& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator!=(const StringView& lhs, const StringView& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const StringView& lhs, const TextChar* rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const TextChar* lhs, const StringView& rhs)
	{
		return !(lhs == rhs);
	}
}
