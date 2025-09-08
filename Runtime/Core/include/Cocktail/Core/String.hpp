#ifndef COCKTAIL_CORE_STRING_HPP
#define COCKTAIL_CORE_STRING_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/StringView.hpp>

namespace Ck
{
	class StringView;

	class COCKTAIL_CORE_API String
	{
	public:

		using SizeType = Array<TextChar>::SizeType;

		static SizeType Length(const TextChar* string);

		/**
		 * \brief Utility constant value to return a reference/pointer to an empty String
		 */
		static const String Empty;

		String() = default;
		String(const TextChar* string);
		String(const TextChar* string, SizeType length);

		String& Append(const String& other);
		String& Append(const TextChar* string);
		String& Append(const TextChar* string, SizeType length);

		String& Prepend(const String& other);
		String& Prepend(const TextChar* string);
		String& Prepend(const TextChar* string, SizeType length);

		TextChar& At(SizeType index);
		const TextChar& At(SizeType index) const;

		Optional<TextChar&> TryAt(SizeType index);
		Optional<const TextChar&> TryAt(SizeType index) const;

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
		bool EndsWith(StringView stringView, bool caseSensitive = true) const;
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
				if (split.mCharacters.Last() != '\0')
					split.mCharacters.Add('\0');

				start += split.GetLength() + 1;

				splits.Add(std::move(split));
			}
		}

		String& ToLowercase();
		String& ToUppercase();

		bool IsEmpty() const;
		SizeType GetLength() const;

		const TextChar* GetData() const;

		TextChar& operator[](SizeType index);
		const TextChar& operator[](SizeType index) const;

	private:

		Array<TextChar> mCharacters;
	};

	COCKTAIL_CORE_API bool operator==(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(const String& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator==(const TextChar* lhs, const String& rhs);

	COCKTAIL_CORE_API bool operator!=(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(const String& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator!=(const TextChar* lhs, const String& rhs);
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
