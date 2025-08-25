#ifndef COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP
#define COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP

#include <cassert>
#include <string>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/TranslatorCast.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_FROM(InvalidPropertyPathException, RuntimeException);

	template <typename Char, typename Traits = std::char_traits<Char>, typename Alloc = std::allocator<Char>>
	class PropertyTreePath
	{
	public:

		using String = std::basic_string<Char, Traits, Alloc>;

		/**
		 * \brief Default constructor
		 * Create an empty path
		 */
		PropertyTreePath() = default;

		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Tr 
		 * \param value 
		 * \param separator 
		 * \param translator 
		 */
		template <typename T, typename Tr = typename TranslatorBetween<String, T>::Type>
		PropertyTreePath(const T& value, Char separator = '.', const Tr& translator = Tr()) :
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
		PropertyTreePath(const String& path, Char separator = '.') :
			mSeparator(separator),
			mPath(path)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * Create a path by parsing the null-terminated Char buffer \p path
		 * \param path The string to parse, possibly with separators
		 * \param separator The separator used to parse the \p path
		 */
		PropertyTreePath(const Char* path, Char separator = '.') :
			mSeparator(separator),
			mPath(path)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \return
		 */
		bool IsEmpty() const
		{
			return mPath.empty();
		}

		/**
		 * \brief
		 * \return
		 */
		bool IsSingle() const
		{
			return mPath.find(mSeparator) == String::npos;
		}

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		PropertyTreePath& Append(const PropertyTreePath<Char, Traits, Alloc>& other)
		{
			assert(mSeparator == other.mSeparator || other.IsEmpty() || other.IsSingle());
			if (!other.IsEmpty())
			{
				if (!IsEmpty())
					mPath.push_back(mSeparator);

				mPath.append(other.mPath);
			}

			return *this;
		}

		/**
		 * \brief 
		 * \tparam T 
		 * \tparam Tr 
		 * \param other 
		 * \param translator 
		 * \return 
		 */
		template <typename T, typename Tr = typename TranslatorBetween<String, T>::Type>
		PropertyTreePath& Append(const T& other, const Tr& translator = Tr())
		{
			return Append(PropertyTreePath(other, mSeparator, translator));
		}

		/**
		 * \brief Remove the front element of the path and return it
		 * Takes the front element of the path, removes it from the path and return it.
		 * This function should not be called it the path is empty
		 * \return The front element
		 */
		template <typename T = String, typename Tr = typename TranslatorBetween<String, T>::Type>
		T Reduce(const Tr& translator = Tr())
		{
			assert(!IsEmpty() && "Cannot reduce an empty path");

			std::size_t separatorIndex = mPath.find_first_of(mSeparator);
			String frontSplit = mPath.substr(0, separatorIndex);
			mPath = mPath.substr(separatorIndex + 1);

			return translator(frontSplit).Get();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Char GetSeparator() const
		{
			return mSeparator;
		}

		/**
		 * \brief 
		 * \return 
		 */
		const String& ToString() const
		{
			return mPath;
		}

	private:

		Char mSeparator;
		String mPath;
	};

	template <typename T>
	struct PropertyTreePathOf
	{
	};

	template <typename Char, typename Traits, typename Alloc>
	struct PropertyTreePathOf<std::basic_string<Char, Traits, Alloc>>
	{
		using Type = PropertyTreePath<Char, Traits, Alloc>;
	};

	using PropertyPath = PropertyTreePath<char>;
}

#endif // COCKTAIL_CORE_UTILITY_PROPERTYTREE_PROPERTYTREEPATH_HPP
