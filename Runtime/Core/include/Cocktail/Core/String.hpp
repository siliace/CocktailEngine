#ifndef COCKTAIL_CORE_STRING_HPP
#define COCKTAIL_CORE_STRING_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API String
	{
	public:

		using AllocatorType = SizedHeapAllocator<32>;
		using ElementType = TextChar;
		using SizeType = AllocatorType::SizeType;
		using CharacterArray = Array<ElementType, AllocatorType>;

		/**
		 * \brief Create a new instance of String form a non null-terminated character array
		 * \param characters Character to set to the string to create
		 * \return The created string
		 */
		static String FromCharacters(CharacterArray characters);

		/**
		 * \brief 
		 */
		static const String Empty;

		/**
		 * \brief 
		 */
		String() = default;

		/**
		 * \brief 
		 * \param data 
		 */
		String(const ElementType* data);

		/**
		 * \brief 
		 * \param data 
		 * \param size 
		 */
		String(const ElementType* data, SizeType size);

		/**
		 * \brief 
		 * \param other 
		 */
		String(const String& other) = default;

		/**
		 * \brief 
		 * \param other 
		 */
		String(String&& other) = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		String& operator=(const String& other) = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		String& operator=(String&& other) = default;

		/**
		 * \brief 
		 * \param start 
		 * \return 
		 */
		String Substring(SizeType start) const;

		/**
		 * \brief 
		 * \param start 
		 * \param count 
		 * \return 
		 */
		String Substring(SizeType start, SizeType count) const;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		String Append(const String& other) const;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		String& Append(InPlaceTag, const String& other);

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		ElementType& At(SizeType index);

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const ElementType& At(SizeType index) const;

		/**
		 * \brief 
		 * \param other 
		 * \param caseSensitive 
		 * \return 
		 */
		bool StartsWith(const String& other, bool caseSensitive = true) const;

		/**
		 * \brief 
		 * \param other 
		 * \param caseSensitive 
		 * \return 
		 */
		bool EndsWith(const String& other, bool caseSensitive = true) const;

		/**
		 * \brief 
		 * \param other 
		 * \param caseSensitive 
		 * \return 
		 */
		int Compare(const String& other, bool caseSensitive = true) const;

		/**
		 * \brief 
		 * \param other 
		 * \param caseSensitive 
		 * \return 
		 */
		int Compare(const ElementType* other, bool caseSensitive = true) const;

		/**
		 * \brief 
		 * \param other 
		 * \param length
		 * \param caseSensitive 
		 * \return 
		 */
		int Compare(const ElementType* other, SizeType length, bool caseSensitive = true) const;

		/**
		 * \brief
		 * \param separator
		 * \return
		 */
		Array<String> Split(ElementType separator) const;

		/**
		 * \brief
		 * \param splits
		 * \param separator
		 * \return
		 */
		 void Split(Array<String>& splits, ElementType separator) const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEmpty() const;

		/**
		 * \brief 
		 * \return 
		 */
		SizeType GetSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		SizeType GetCapacity() const;

		/**
		 * \brief 
		 * \return 
		 */
		ElementType* GetData();

		/**
		 * \brief 
		 * \return 
		 */
		const ElementType* GetData() const;

		Array<ElementType>& GetCharacterArray();

		const Array<ElementType>& GetCharacterArray() const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		ElementType& operator[](SizeType index);

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const ElementType& operator[](SizeType index) const;

	private:

		CharacterArray mData;
	};

	COCKTAIL_CORE_API bool operator==(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(const String::ElementType* lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator==(const String& lhs, const String::ElementType* rhs);

	COCKTAIL_CORE_API bool operator!=(const String& lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(const String::ElementType* lhs, const String& rhs);
	COCKTAIL_CORE_API bool operator!=(const String& lhs, const String::ElementType* rhs);
}

#endif // COCKTAIL_CORE_STRING_HPP
