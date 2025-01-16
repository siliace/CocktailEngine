#ifndef COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP
#define COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP

#include <any>
#include <locale>
#include <sstream>

#include <Cocktail/Core/Enum.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
	namespace Detail
	{
		template <typename Ch, typename Traits, typename E, typename Enabler = void>
		struct customize_stream
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, const E& e)
			{
				s << e;
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, E& e)
			{
				s >> e;
				if (!s.eof())
					s >> std::ws;
			}
		};

		// No whitespace skipping for single characters.
		template <typename Ch, typename Traits>
		struct customize_stream<Ch, Traits, Ch, void>
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, Ch e)
			{
				s << e;
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, Ch& e)
			{
				s.unsetf(std::ios_base::skipws);
				s >> e;
			}
		};

		template <typename Ch, typename Traits, typename F>
		struct customize_stream<Ch, Traits, F, std::enable_if_t<std::is_floating_point_v<F>>>
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, const F& e)
			{
				s.precision(std::numeric_limits<F>::digits10 + 2);
				s << e;
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, F& e)
			{
				s >> e;
				if (!s.eof())
				{
					s >> std::ws;
				}
			}
		};

		template <typename Ch, typename Traits>
		struct customize_stream<Ch, Traits, bool, void>
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, bool e)
			{
				s.setf(std::ios_base::boolalpha);
				s << e;
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, bool& e)
			{
				s >> e;
				if (s.fail())
				{
					// Try again in word form.
					s.clear();
					s.setf(std::ios_base::boolalpha);
					s >> e;
				}
				if (!s.eof())
				{
					s >> std::ws;
				}
			}
		};

		template <typename Ch, typename Traits>
		struct customize_stream<Ch, Traits, signed char, void>
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, signed char e)
			{
				s << static_cast<int>(e);
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, signed char& e)
			{
				int i;
				s >> i;
				// out of range?
				if (i > (std::numeric_limits<signed char>::max)() ||
					i < (std::numeric_limits<signed char>::min)())
				{
					s.clear(); // guarantees eof to be unset
					e = 0;
					s.setstate(std::ios_base::badbit);
					return;
				}
				e = static_cast<signed char>(i);
				if (!s.eof())
				{
					s >> std::ws;
				}
			}
		};

		template <typename Ch, typename Traits>
		struct customize_stream<Ch, Traits, unsigned char, void>
		{
			static void Insert(std::basic_ostream<Ch, Traits>& s, unsigned char e)
			{
				s << static_cast<unsigned int>(e);
			}

			static void Extract(std::basic_istream<Ch, Traits>& s, unsigned char& e)
			{
				unsigned i;
				s >> i;
				// out of range?
				if (i > (std::numeric_limits<unsigned char>::max)())
				{
					s.clear(); // guarantees eof to be unset
					e = 0;
					s.setstate(std::ios_base::badbit);
					return;
				}
				e = static_cast<unsigned char>(i);
				if (!s.eof())
				{
					s >> std::ws;
				}
			}
		};
	}

	template <typename Internal, typename External>
	struct TranslatorBetween
	{
		using InputType = Internal;
		using OutputType = External;

		Optional<OutputType> operator()(const InputType& value) const
		{
			return Optional<OutputType>::Of(value);
		}

		Optional<InputType> operator()(const OutputType& value) const
		{
			return Optional<InputType>::Of(value);
		}
	};

	template <typename T>
	struct IdentityTranslator
	{
		using InputType = T;
		using OutputType = T;

		Optional<T> operator()(const T& value) const
		{
			return Optional<T>::Of(value);
		}
	};

	template <typename Ch, typename Traits, typename Alloc, typename E>
	class StreamTranslator
	{
	public:

		using InputType = std::basic_string<Ch, Traits, Alloc>;
		using OutputType = E;

		static constexpr bool HandleEnum = std::is_enum_v<E>;

		explicit StreamTranslator(const std::locale& locale = std::locale()) :
			mLocale(locale)
		{
			/// Nothing
		}

		Optional<E> operator()(const InputType& value) const
		{
			std::basic_istringstream<Ch, Traits, Alloc> iss(value);
			iss.imbue(mLocale);

			if constexpr (HandleEnum)
			{
				using TypedStream = Detail::customize_stream<Ch, Traits, std::basic_string<Ch, Traits, Alloc>>;

				std::basic_string<Ch, Traits, Alloc> underlying;
				TypedStream::Extract(iss, underlying);
				if (iss.fail() || iss.bad() || iss.get() != Traits::eof())
					Optional<E>::Empty();

				return Optional<E>::Of(
					Enum<E>::FromString(underlying)
				);
			}
			else
			{
				using TypedStream = Detail::customize_stream<Ch, Traits, E>;

				E e;
				TypedStream::Extract(iss, e);
				if (iss.fail() || iss.bad() || iss.get() != Traits::eof())
					Optional<E>::Empty();

				return Optional<E>::Of(e);
			}
		}

		Optional<InputType> operator()(const E& v) const
		{
			std::basic_ostringstream<Ch, Traits, Alloc> oss;
			oss.imbue(mLocale);

			if constexpr (HandleEnum)
			{
				using TypedStream = Detail::customize_stream<Ch, Traits, std::basic_string<Ch, Traits, Alloc>>;
				TypedStream::Insert(oss, Enum<E>::ToString(v));
			}
			else
			{
				using TypedStream = Detail::customize_stream<Ch, Traits, E>;
				TypedStream::Insert(oss, v);
			}

			if (!oss)
				return Optional<InputType>::Empty();

			return Optional<InputType>::Of(oss.str());
		}

	private:

		std::locale mLocale;
	};

	template <typename E>
	class AnyTranslator
	{
	public:

		using InputType = std::any;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const
		{
			try
			{
				return Optional<OutputType>::Of(
					std::any_cast<E>(value)
				);
			}
			catch (std::bad_any_cast)
			{
				return Optional<OutputType>::Empty();
			}
		}

		Optional<InputType> operator()(const OutputType& value) const
		{
			return Optional<InputType>::Of(std::any(value));
		}
	};

	template <typename T>
	struct TranslatorBetween<T, T>
	{
		using Type = IdentityTranslator<T>;
	};

	template <typename Ch, typename Traits, typename Alloc>
	struct TranslatorBetween<std::basic_string<Ch, Traits, Alloc>, std::basic_string<Ch, Traits, Alloc>>
	{
		using Type = IdentityTranslator<std::basic_string<Ch, Traits, Alloc>>;
	};

	template <typename Ch, typename Traits, typename Alloc, typename E>
	struct TranslatorBetween<std::basic_string<Ch, Traits, Alloc>, E>
	{
		using Type = StreamTranslator<Ch, Traits, Alloc, E>;
	};

	template <typename Ch, typename Traits, typename Alloc, typename E>
	struct TranslatorBetween<E, std::basic_string<Ch, Traits, Alloc>>
	{
		using Type = StreamTranslator<Ch, Traits, Alloc, E>;
	};

	template <typename Ch, typename E>
	struct TranslatorBetween<Ch*, E>
	{
		using Type = StreamTranslator<Ch, std::char_traits<Ch>, std::allocator<Ch>, E>;
	};

	template <typename Ch, typename E>
	struct TranslatorBetween<const Ch*, E>
	{
		using Type = StreamTranslator<Ch, std::char_traits<Ch>, std::allocator<Ch>, E>;
	};

	template <typename T, typename U, typename Tr = typename TranslatorBetween<T, U>::Type>
	Optional<T> TryTranslatorCast(const U& value, const Tr& translator = Tr())
	{
		return translator(value);
	}

	template <typename T, typename U, typename Tr = typename TranslatorBetween<T, U>::Type>
	T TranslatorCast(const U& value, const Tr& translator = Tr())
	{
		return TryTranslatorCast<T, U>(value, translator).template GetOrThrow<std::bad_cast>();
	}
}

#endif // COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP
