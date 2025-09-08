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

		StringView();
		StringView(const TextChar* string);
		StringView(const TextChar* string, SizeType length);

		const TextChar& At(SizeType index) const;
		Optional<const TextChar&> TryAt(SizeType index) const;

		int Compare(StringView stringView, bool caseSensitive = true) const;
		int Compare(const String& other, bool caseSensitive = true) const;
		int Compare(const TextChar* string, bool caseSensitive = true) const;
		int Compare(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool StartsWith(TextChar character, bool caseSensitive = true);
		bool StartsWith(StringView stringView, bool caseSensitive = true);
		bool StartsWith(const String& other, bool caseSensitive = true) const;
		bool StartsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool EndsWith(TextChar character, bool caseSensitive = true);
		bool EndsWith(StringView stringView, bool caseSensitive = true);
		bool EndsWith(const String& other, bool caseSensitive = true) const;
		bool EndsWith(const TextChar* string, SizeType length, bool caseSensitive = true) const;

		bool IsEmpty() const;
		SizeType GetLength() const;

		const TextChar* GetData() const;

		const TextChar& operator[](SizeType index) const;

	private:

		const TextChar* mString;
		SizeType mLength;
	};

	COCKTAIL_CORE_API bool operator==(const StringView& lhs, const StringView& rhs);
	COCKTAIL_CORE_API bool operator==(const StringView& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator==(const TextChar* lhs, const StringView& rhs);
	COCKTAIL_CORE_API bool operator==(const StringView& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(const String& lhs, const StringView& rhs);

	COCKTAIL_CORE_API bool operator!=(const StringView& lhs, const StringView& rhs);
	COCKTAIL_CORE_API bool operator!=(const StringView& lhs, const TextChar* rhs);
	COCKTAIL_CORE_API bool operator!=(const TextChar* lhs, const StringView& rhs);
	COCKTAIL_CORE_API bool operator!=(const StringView& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(const String& lhs, const StringView& rhs);
}

#endif // COCKTAIL_CORE_STRINGVIEW_HPP
