#ifndef COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP
#define COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP

#include <sstream>
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
		 * \param end
		 * \return
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool EndsWith(const std::basic_string<Char, Traits, Alloc>& string, const std::basic_string<Char, Traits, Alloc>& end)
		{
			return string.substr(end.length() + 1) == end;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \tparam Alloc
		 * \param string
		 * \param end
		 * \return
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool EndsWith(const std::basic_string<Char, Traits, Alloc>& string, std::basic_string_view<Char, Traits> end)
		{
			return string.substr(end.length() + 1) == end;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \tparam Alloc
		 * \param string
		 * \param end
		 * \return
		 */
		template <typename Char, typename Traits, typename Alloc>
		static bool EndsWith(const std::basic_string<Char, Traits, Alloc>& string, const Char* end)
		{
			return EndsWith(string, std::basic_string_view<Char, Traits>(end));
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param string
		 * \param end
		 * \return
		 */
		template <typename Char, typename Traits>
		static bool EndsWith(const std::basic_string_view<Char, Traits>& string, const std::basic_string_view<Char, Traits>& end)
		{
			return string.substr(end.length() + 1) == end;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param string
		 * \param end
		 * \return
		 */
		template <typename Char, typename Traits>
		static bool EndsWith(const std::basic_string_view<Char, Traits>& string, const Char* end)
		{
			return EndsWith(string, std::basic_string_view<Char, Traits>(end));
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param string
		 * \param end
		 * \return
		 */
		template <typename Char>
		static bool EndsWith(const Char* string, const Char* end)
		{
			return EndsWith(std::basic_string_view<Char>(string), std::basic_string_view<Char>(end));
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
			Split(tokens, string, delimiter);

			return tokens;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param string
		 * \param delimiter
		 */
		template <typename Char, typename Traits>
		static std::vector<std::basic_string<Char, Traits>> Split(const std::basic_string_view<Char, Traits>& string, Char delimiter)
		{
			std::vector<std::basic_string<Char, Traits>> tokens;
			Split(tokens, string, delimiter);

			return tokens;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param string
		 * \param delimiter
		 */
		template <typename Char>
		static std::vector<std::basic_string<Char>> Split(const Char* string, Char delimiter)
		{
			std::vector<std::basic_string<Char>> tokens;
			Split(tokens, string, delimiter);

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
		}

		/**
		 * \brief
		 * \tparam Char
		 * \tparam Traits
		 * \param tokens
		 * \param string
		 * \param delimiter
		 */
		template <typename Char, typename Traits>
		static void Split(std::vector<std::basic_string<Char, Traits>>& tokens, const std::basic_string_view<Char, Traits>& string, Char delimiter)
		{
			Split(tokens, std::basic_string<Char, Traits>(string), delimiter);
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param tokens
		 * \param string
		 * \param delimiter
		 */
		template <typename Char>
		static void Split(std::vector<std::basic_string<Char>>& tokens, const char* string, Char delimiter)
		{
			Split(tokens, std::basic_string_view<Char>(string), delimiter);
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
				if constexpr (std::is_arithmetic_v<typename std::iterator_traits<It>::value_type>)
				{
					oss << std::to_string(*begin);
				}
				else
				{
					oss << *begin;
				}

				if (std::next(begin) != end)
					oss << glue;

				++begin;
			}

			return oss.str();
		}

		/**
		 * \brief
		 * \tparam It
		 * \tparam Char
		 * \tparam Traits
		 * \param begin
		 * \param end
		 * \param glue
		 * \return
		 */
		template <typename It, typename Char, typename Traits>
		static std::basic_string<Char, Traits> Join(It begin, It end, const std::basic_string_view<Char, Traits>& glue)
		{
			return Join(begin, end, std::basic_string<Char, Traits>(glue));
		}

		/**
		 * \brief
		 * \tparam It
		 * \tparam Char
		 * \param begin
		 * \param end
		 * \param glue
		 * \return
		 */
		template <typename It, typename Char>
		static std::basic_string<Char> Join(It begin, It end, const Char* glue)
		{
			return Join(begin, end, std::basic_string<Char>(glue));
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
		static std::basic_string<Char, Traits, Alloc> Capitalize(std::basic_string_view<Char, Traits> string)
		{
			std::basic_string<Char, Traits, Alloc> capitalized(string);
			for (typename std::basic_string<Char, Traits, Alloc>::size_type i = 0; i < string.length(); ++i)
			{
				if (std::isalpha(capitalized[i]))
					capitalized[i] = static_cast<Char>(std::toupper(capitalized[i]));
			}

			return capitalized;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param string
		 * \return
		 */
		template <typename Char>
		static std::basic_string<Char> Capitalize(const Char* string)
		{
			return Capitalize(std::basic_string_view<Char>(string));
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
		static std::basic_string<Char, Traits, Alloc> Minusculize(const std::basic_string<Char, Traits, Alloc>& string)
		{
			return Minusculize(std::basic_string_view<Char, Traits>(string));
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
		static std::basic_string<Char, Traits, Alloc> Minusculize(std::basic_string_view<Char, Traits> string)
		{
			std::basic_string<Char, Traits, Alloc> minusculized(string);
			for (typename std::basic_string<Char, Traits, Alloc>::size_type i = 0; i < string.length(); ++i)
			{
				if (std::isalpha(minusculized[i]))
					minusculized[i] = static_cast<Char>(std::tolower(minusculized[i]));
			}

			return minusculized;
		}

		/**
		 * \brief
		 * \tparam Char
		 * \param string
		 * \return
		 */
		template <typename Char>
		static std::basic_string<Char> Minusculize(const Char* string)
		{
			return Minusculize(std::basic_string_view<Char>(string));
		}
	};
}

#endif // COCKTAIL_CORE_UTILITY_STRINGUTILS_HPP
