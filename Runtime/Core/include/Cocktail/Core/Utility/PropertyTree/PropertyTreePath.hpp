#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP

#include <cassert>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(InvalidPropertyPathException, RuntimeException);

	class PropertyTreePath;

	template <>
	struct TranslatorBetween<PropertyTreePath, String>
	{
		using Type = ToStringTranslator<PropertyTreePath>;
	};

	class COCKTAIL_CORE_API PropertyTreePath
	{
	public:

		/**
		 * \brief Default constructor
		 * Create an empty path
		 */
		PropertyTreePath();

		/**
		 * \brief
		 * \tparam T
		 * \tparam Tr
		 * \param value
		 * \param separator
		 * \param translator
		 */
		template <typename T, typename Tr = typename TranslatorBetween<T, String>::Type>
		explicit PropertyTreePath(const T& value, TextChar separator = CK_CHAR('.'), const Tr& translator = Tr()) :
			mSeparator(separator)
		{
			mPath = translator(value).Get();
		}

		/**
		 * \brief Constructor
		 * Create a path by parsing the String \p path
		 * \param path The string to parse, possibly with separators
		 * \param separator The separator used to parse the \p path
		 */
		PropertyTreePath(String path, TextChar separator = CK_CHAR('.'));

		/**
		 * \brief Constructor
		 * Create a path by parsing the null-terminated Char buffer \p path
		 * \param path The string to parse, possibly with separators
		 * \param separator The separator used to parse the \p path
		 */
		PropertyTreePath(const TextChar* path, TextChar separator = CK_CHAR('.'));

		/**
		 * \brief
		 * \return
		 */
		bool IsEmpty() const;

		/**
		 * \brief
		 * \return
		 */
		bool IsSingle() const;

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		PropertyTreePath& Append(const PropertyTreePath& other);

		/**
		 * \brief
		 * \tparam T
		 * \tparam Tr
		 * \param other
		 * \param translator
		 * \return
		 */
		template <typename T, typename Tr = typename TranslatorBetween<T, String>::Type>
		PropertyTreePath& Append(const T& other, const Tr& translator = Tr())
		{
			return Append(PropertyTreePath(other, mSeparator, translator));
		}

		/**
		 * \brief Remove the front element of the path and return it
		 *
		 * Takes the front element of the path, removes it from the path and return it.
		 * This function should not be called it the path is empty.
		 *
		 * \return The front element
		 */
		template <typename T = String, typename Tr = typename TranslatorBetween<String, T>::Type>
		T Reduce(const Tr& translator = Tr())
		{
			assert(!IsEmpty() && "Cannot reduce an empty path");

		    String frontSplit;
		    unsigned int separatorIndex =  mPath.FindFirst(mSeparator).GetOr(0);
		    if (separatorIndex != 0)
		    {
		        frontSplit = mPath.SubString(0, separatorIndex);
		        mPath = mPath.SubString(separatorIndex + 1);
		    }
		    else
		    {
		        frontSplit = std::move(mPath);
		        mPath = String::Empty;
		    }

			return translator(frontSplit).Get();
		}

		/**
		 * \brief
		 * \return
		 */
		TextChar GetSeparator() const;

		/**
		 * \brief
		 * \return
		 */
		const String& ToString() const;

	private:

		TextChar mSeparator;
		String mPath;
	};
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP
