#ifndef COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP
#define COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP

#include <string>
#include <vector>

namespace Ck
{
	/**
	 * \brief 
	 */
	class StringUtils
	{
	public:

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \tparam Alloc
		 * \param string
		 * \param begin
		 * \return
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool StartsWith(const std::basic_string<Char, Traits, Alloc>& string, const std::basic_string<Char, Traits, Alloc>& begin)
		{
			return string.find(begin) == 0;
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \param begin 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool StartsWith(const std::basic_string<Char, Traits, Alloc>& string, std::basic_string_view<Char, Traits> begin)
		{
			return string.find(begin) == 0;
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \param begin 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool StartsWith(const std::basic_string<Char, Traits, Alloc>& string, const Char* begin)
		{
			return StartsWith(string, std::basic_string_view<Char, Traits>(begin));
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param string
		 * \param begin
		 * \return
		 */
		template <typename Char, typename Traits>
		static bool StartsWith(const std::basic_string_view<Char, Traits>& string, const std::basic_string_view<Char, Traits>& begin)
		{
			return string.find(begin) == 0;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param string
		 * \param begin
		 * \return
		 */
		template <typename Char, typename Traits>
		static bool StartsWith(const std::basic_string_view<Char, Traits>& string, const Char* begin)
		{
			return StartsWith(string, std::basic_string_view<Char, Traits>(begin));
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param string
		 * \param begin
		 * \return
		 */
		template <typename Char>
		static bool StartsWith(const Char* string, const Char* begin)
		{
			return StartsWith(std::basic_string_view<Char>(string), std::basic_string_view<Char>(begin));
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \param delimiter 
		 */
		template <typename Char, typename Traits, typename Alloc>
		static std::vector<std::basic_string<Char, Traits, Alloc>> Split(const std::basic_string<Char, Traits, Alloc>& string, Char delimiter)
		{
			std::vector<std::basic_string<Char, Traits, Alloc>> tokens;

			std::basic_string<Char, Traits, Alloc> token;
			std::basic_istringstream<Char, Traits, Alloc> iss(string);
			while (std::getline(iss, token, delimiter))
			{
				if (!token.empty())
					tokens.push_back(token);
			}

			return tokens;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \tparam Alloc
		 * \param tokens
		 * \param string
		 * \param delimiter
		 */
		template <typename Char, typename Traits, typename Alloc>
		static void Split(std::vector<std::basic_string<Char, Traits, Alloc>>& tokens, const std::basic_string<Char, Traits, Alloc>& string, Char delimiter)
		{
			tokens.clear();

			std::basic_string<Char, Traits, Alloc> token;
			std::basic_istringstream<Char, Traits, Alloc> iss(string);
			while (std::getline(iss, token, delimiter))
			{
				if (!token.empty())
					tokens.push_back(token);
			}

			return tokens;
		}

		/**
		 * \brief 
		 * \tparam It 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param begin 
		 * \param end 
		 * \param glue 
		 * \return 
		 */
		template <typename It, typename Char, typename Traits, typename Alloc>
		static std::basic_string<Char, Traits, Alloc> Join(It begin, It end, const std::basic_string<Char, Traits, Alloc>& glue)
		{
			std::basic_ostringstream<Char, Traits, Alloc> oss;
			while (begin != end)
			{
				oss << *begin;
				if (std::next(begin) != end)
					oss << glue;

				++begin;
			}

			return oss.str();
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc = std::allocator<Char>>
		static std::basic_string<Char, Traits, Alloc> Capitalize(std::basic_string_view<Char, Traits> string)
		{
			std::basic_string<Char, Traits, Alloc> capitalized(string);
			for (typename std::basic_string<Char, Traits, Alloc>::size_type i = 0; i < string.length(); i++)
			{
				if (std::isalpha(capitalized[i]))
				{
					capitalized[i] = std::toupper(capitalized[i]);
					break;
				}
			}

			return capitalized;
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc>
		static std::basic_string<Char, Traits, Alloc> Capitalize(const std::basic_string<Char, Traits, Alloc>& string)
		{
			return Capitalize(std::basic_string_view<Char, Traits>(string));
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc = std::allocator<Char>>
		static std::basic_string<Char, Traits, Alloc> Minisculize(std::basic_string_view<Char, Traits> string)
		{
			std::basic_string<Char, Traits, Alloc> minusculized(string);
			for (typename std::basic_string<Char, Traits, Alloc>::size_type i = 0; i < string.length(); i++)
			{
				if (std::isalpha(minusculized[i]))
				{
					minusculized[i] = std::tolower(minusculized[i]);
					break;
				}
			}

			return minusculized;
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \tparam Alloc 
		 * \param string 
		 * \return 
		 */
		template <typename Char, typename Traits, typename Alloc>
		static std::basic_string<Char, Traits, Alloc> Minisculize(const std::basic_string<Char, Traits, Alloc>& string)
		{
			return Minisculize(std::basic_string_view<Char, Traits>(string));
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param lhs
		 * \param rhs
		 * \param caseSensitive
		 * \return
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool Compare(const std::basic_string<Char, Traits, Alloc>& lhs, const std::basic_string<Char, Traits, Alloc>& rhs, bool caseSensitive = true)
		{
			return Compare(std::basic_string_view<Char, Traits>(lhs), std::basic_string_view<Char, Traits>(rhs), caseSensitive);
		}

		/**
		 * \brief 
		 * \tparam Char 
		 * \tparam Traits 
		 * \param lhs 
		 * \param rhs 
		 * \param caseSensitive 
		 * \return 
		 */
		template <typename Char, typename Traits>
		static bool Compare(std::basic_string_view<Char, Traits> lhs, std::basic_string_view<Char, Traits> rhs, bool caseSensitive = true)
		{
			if (lhs.size() != rhs.size())
				return false;

			if (caseSensitive)
				return lhs == rhs;

			for (typename std::basic_string_view<Char, Traits>::size_type i = 0; i < rhs.size(); ++i)
			{
				if (std::tolower(lhs[i]) != std::tolower(rhs[i]))
					return false;
			}
			return false;
		}
	};
}

#endif // COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP
