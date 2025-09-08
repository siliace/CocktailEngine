#ifndef COCKTAIL_CORE_STRINGVIEW_HPP
#define COCKTAIL_CORE_STRINGVIEW_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
	class String;

	class COCKTAIL_CORE_API StringView
	{
	public:

		using SizeType = unsigned int;

		/**
		 * \brief Utility constant value to return a reference/pointer to an empty StringView
		 */
		static const StringView Empty;

		StringView();
		StringView(const String& string);
		StringView(const TextChar* string);
		StringView(const TextChar* string, SizeType length);

		StringView& operator=(StringView other);
		StringView& operator=(const String& string);
		StringView& operator=(const TextChar* string);

		const TextChar& At(SizeType index) const;
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

		StringView SubStringView(SizeType start) const;
		StringView SubStringView(SizeType start, SizeType length) const;

		int Compare(StringView other, bool caseSensitive = true) const;
		int Compare(const String& other, bool caseSensitive = true) const;
		int Compare(const TextChar* string, bool caseSensitive = true) const;
		int Compare(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool StartsWith(TextChar character, bool caseSensitive = true) const;
		bool StartsWith(StringView search, bool caseSensitive = true) const;
		bool StartsWith(const String& search, bool caseSensitive = true) const;
		bool StartsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool EndsWith(TextChar search, bool caseSensitive = true) const;
		bool EndsWith(StringView search, bool caseSensitive = true) const;
		bool EndsWith(const String& search, bool caseSensitive = true) const;
		bool EndsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool IsNumeric() const;

		bool IsEmpty() const;
		SizeType GetLength() const;

		const TextChar* GetData() const;

		const TextChar& operator[](SizeType index) const;

	private:

		const TextChar* mString;
		SizeType mLength;
	};

	COCKTAIL_CORE_API bool operator==(StringView lhs, StringView rhs);
	COCKTAIL_CORE_API bool operator==(StringView lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(StringView lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator==(const TextChar* lhs, StringView rhs);

	COCKTAIL_CORE_API bool operator!=(StringView lhs, StringView rhs);
	COCKTAIL_CORE_API bool operator!=(StringView lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(StringView lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator!=(const TextChar* lhs, StringView rhs);
}

namespace std
{
	template <>
	struct hash<Ck::StringView>
	{
		size_t operator()(const Ck::StringView& stringView) const noexcept;
	};
}

#endif // COCKTAIL_CORE_STRINGVIEW_HPP
