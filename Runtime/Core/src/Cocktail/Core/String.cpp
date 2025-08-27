#include <Cocktail/Core/String.hpp>

namespace Ck
{
	const String String::Empty;

	namespace
	{
		String::SizeType Strlen(const String::ElementType* begin)
		{
			const String::ElementType* end = begin;
			while (*end != '\0')
				++end;

			return end - begin;
		}

		bool CompareTextChar(String::ElementType lhs, String::ElementType rhs, bool caseSensitive)
		{
			return caseSensitive ? lhs == rhs : std::tolower(lhs) == std::tolower(rhs);
		}
	}

	String String::FromCharacters(CharacterArray characters)
	{
		characters.Add('\0');

		String string;
		string.GetCharacterArray() = std::move(characters);

		return string;
	}

	String::String(const ElementType* data) :
		String(data, Strlen(data))
	{
		/// Nothing
	}

	String::String(const ElementType* data, SizeType size)
	{
		mData.Append(data, size + 1);
		mData[size] = '\0';
	}

	String String::Substring(SizeType start) const
	{
		return Substring(start, GetSize() - start);
	}

	String String::Substring(SizeType start, SizeType count) const
	{
		return FromCharacters(mData.Slice(start, count));
	}

	String String::Append(const String& other) const
	{
		String copy(*this);
		copy.Append(InPlace, other);

		return copy;
	}

	String& String::Append(InPlaceTag, const String& other)
	{
		mData.PopLast(); // Remove the current null terminator
		mData.Append(other.mData); // Append the character, including the null terminator

		return *this;
	}

	String::ElementType& String::At(SizeType index)
	{
		return mData.At(index);
	}

	const String::ElementType& String::At(SizeType index) const
	{
		return mData.At(index);
	}

	bool String::StartsWith(const String& other, bool caseSensitive) const
	{
		if (other.GetSize() > GetSize())
			return false;

		for (SizeType i = 0; i < other.GetSize(); i++)
		{
			if (!CompareTextChar(mData[i], other.mData[i], caseSensitive))
				return false;
		}

		return true;
	}

	bool String::EndsWith(const String& other, bool caseSensitive) const
	{
		if (other.GetSize() > GetSize())
			return false;

		for (SizeType i = 0; i < other.GetSize(); i++)
		{
			const SizeType index = GetSize() - 1 - i;
			const SizeType otherIndex = other.GetSize() - 1 - i;
			if (!CompareTextChar(mData[index], other.mData[otherIndex], caseSensitive))
				return false;
		}

		return true;
	}

	int String::Compare(const String& other, bool caseSensitive) const
	{
		return Compare(other.GetData(), other.GetSize(), caseSensitive);
	}

	int String::Compare(const ElementType* other, bool caseSensitive) const
	{
		return Compare(other, Strlen(other), caseSensitive);
	}

	int String::Compare(const ElementType* other, SizeType length, bool caseSensitive) const
	{
		for (SizeType i = 0; i < std::min(GetSize(), length); i++)
		{
			if (!CompareTextChar(mData[i], other[i], caseSensitive))
				return mData[i] > other[i] ? 1 : -1;
		}

		if (GetSize() < length)
			return -1;

		if (GetSize() > length)
			return 1;

		return 0;
	}

	Array<String> String::Split(ElementType separator) const
	{
		Array<String> results;
		Split(results, separator);

		return results;
	}

	void String::Split(Array<String>& splits, ElementType separator) const
	{
		splits.Clear();

		for (unsigned int start = 0; start < GetSize();)
		{
			Array<ElementType> slice;
			Optional<unsigned int> index = mData.FindIndex(separator, start);
			if (index.IsEmpty())
			{
				slice = mData.Slice(start);
			}
			else
			{
				slice = mData.Slice(start, index.Get() - start);
			}

			String split = FromCharacters(std::move(slice));
			start += split.GetSize() + 1;

			splits.Add(std::move(split));
		}
	}

	bool String::IsEmpty() const
	{
		return mData.IsEmpty();
	}

	String::SizeType String::GetSize() const
	{
		return mData.GetSize() - 1;
	}

	String::SizeType String::GetCapacity() const
	{
		return mData.GetCapacity() - 1;
	}

	String::ElementType* String::GetData()
	{
		return mData.GetData();
	}

	const String::ElementType* String::GetData() const
	{
		return mData.GetData();
	}

	Array<String::ElementType> & String::GetCharacterArray()
	{
		return mData;
	}

	const Array<String::ElementType> & String::GetCharacterArray() const
	{
		return mData;
	}

	String::ElementType& String::operator[](SizeType index)
	{
		return mData[index];
	}

	const String::ElementType& String::operator[](SizeType index) const
	{
		return mData[index];
	}

	bool operator==(const String& lhs, const String& rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator==(const String::ElementType* lhs, const String& rhs)
	{
		return rhs.Compare(lhs) == 0;
	}

	bool operator==(const String& lhs, const String::ElementType* rhs)
	{
		return lhs.Compare(rhs) == 0;
	}

	bool operator!=(const String& lhs, const String& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const String::ElementType* lhs, const String& rhs)
	{
		return !(lhs == rhs);
	}

	bool operator!=(const String& lhs, const String::ElementType* rhs)
	{
		return !(lhs == rhs);
	}
}
