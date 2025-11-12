#ifndef COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP
#define COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP

#include <any>

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	namespace Detail
	{
		template <typename T>
		struct Naked
		{
			using Type = std::remove_cv_t<
				std::remove_pointer_t<
					std::decay_t<T>
				>
			>;
		};

		template <typename T>
		using NakedType = typename Naked<T>::Type;
	}

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

	template <typename E>
	struct ToAnyTranslator
	{
		using InputType = E;
		using OutputType = std::any;

		Optional<OutputType> operator()(const InputType& value) const
		{
			return Optional<OutputType>::Of(
				std::make_any<InputType>(value)
			);
		}
	};

	template <typename E>
	struct FromAnyTranslator
	{
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
	};

	template <typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct StringParseIntegerTranslator
	{
		using InputType = E;
		using OutputType = String;

		Optional<OutputType> operator()(InputType value) const noexcept
		{
			using U = std::make_unsigned_t<InputType>;

			if (value == 0)
				return Optional<String>::Of(InPlace, CK_TEXT("0"));

			String result;
			if constexpr (std::is_signed_v<InputType>)
			{
				const bool isNegative = value < 0;
				U n = isNegative ? static_cast<U>(-(value + 1) + 1) : value;

				while (n > 0)
				{
					result.Prepend(CK_CHAR('0') + n % 10);
					n /= 10;
				}

				if (isNegative)
					result.Prepend(CK_CHAR('-'));
			}
			else
			{
				while (value > 0)
				{
					result.Prepend(static_cast<String::CharType>(CK_CHAR('0') + value % 10));
					value /= 10;
				}
			}

			return Optional<String>::Of(std::move(result));
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct StringParseFloatingPointTranslator
	{
		using InputType = E;
		using OutputType = String;

		Optional<OutputType> operator()(InputType value) const
		{
			bool negative = value < 0;
			if (negative)
				value = -value;

			Uint32 integerPart = static_cast<Uint32>(value);
			InputType fractionalPart = value - static_cast<InputType>(integerPart);

			String result = StringParseIntegerTranslator<long long>()(integerPart).Get();
			if (negative)
				result.Prepend('-');

			result.Append('.');
			for (int i = 0; i < std::numeric_limits<InputType>::digits; i++)
			{
				fractionalPart *= 10;
				int digit = static_cast<int>(fractionalPart);
				result.Append(static_cast<String::CharType>('0' + digit));

				fractionalPart -= digit;
			}

			return Optional<String>::Of(std::move(result));
		}
	};
	
	template <typename T, typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct CharsToIntegerTranslator
	{
		using InputType = T;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const
		{
			return StringUtils<Detail::NakedType<T>, Uint32>::template TryToInteger<E>(value);
		}
	};

	template <typename T, typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct CharsToFloatingPointTranslator
	{
		using InputType = T;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const
		{
			return StringUtils<Detail::NakedType<T>, Uint32>::template TryToFloatingPoint<E>(value);
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct StringToIntegerTranslator
	{
		using InputType = String;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			return StringUtils<String::CharType, String::SizeType>::TryToInteger<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct StringToFloatingPointTranslator
	{
		using InputType = String;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			return StringUtils<String::CharType, String::SizeType>::TryToFloatingPoint<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct StringViewToIntegerTranslator
	{
		using InputType = StringView;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			return StringUtils<String::CharType, String::SizeType>::TryToInteger<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct StringViewToFloatingPointTranslator
	{
		using InputType = StringView;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			return StringUtils<String::CharType, String::SizeType>::TryToFloatingPoint<E>(value.GetData());
		}
	};

	template <typename T>
	struct StringConstructionTranslator
	{
		using InputType = T;
		using OutputType = String;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			using Decayed = std::decay_t<InputType>;

			if constexpr (std::is_same_v<Decayed, const AnsiChar*> || std::is_same_v<Decayed, AnsiChar*>)
			{
				return Optional<OutputType>::Of(InPlace, value);
			}
			else if constexpr (std::is_same_v<Decayed, const String::CharType*> || std::is_same_v<Decayed, String::CharType*>)
			{
				return Optional<OutputType>::Of(InPlace, value);
			}
			else
			{
				static_assert(!std::is_same_v<T, T>, "Incompatible char type for String constructor");
				return Optional<OutputType>::Empty();
			}
		}
	};

	template <typename T>
	struct ToStringTranslator
	{
		using InputType = T;
		using OutputType = String;

		String operator()(const InputType& input) const noexcept
		{
			return input.ToString();
		}
	};

	template <typename TIn, typename TOut, typename Enabler = void>
	struct TranslatorBetween
	{
		static_assert(false, "No translator available between TIn and TOut");
	};

	template <typename T> struct TranslatorBetween<T, T> { using Type = IdentityTranslator<T>; };

	template <typename T> struct TranslatorBetween<T, std::any> { using Type = ToAnyTranslator<T>; };
	template <typename T> struct TranslatorBetween<std::any, T> { using Type = FromAnyTranslator<T>; };

	template <> struct TranslatorBetween<const AnsiChar*, Int8> { using Type = CharsToIntegerTranslator<const AnsiChar*, Int8>; };
	template <> struct TranslatorBetween<const AnsiChar*, Int16> { using Type = CharsToIntegerTranslator<const AnsiChar*, Int16>; };
	template <> struct TranslatorBetween<const AnsiChar*, Int32> { using Type = CharsToIntegerTranslator<const AnsiChar*, Int32>; };
	template <> struct TranslatorBetween<const AnsiChar*, Int64> { using Type = CharsToIntegerTranslator<const AnsiChar*, Int64>; };
	template <> struct TranslatorBetween<const AnsiChar*, Uint8> { using Type = CharsToIntegerTranslator<const AnsiChar*, Uint8>; };
	template <> struct TranslatorBetween<const AnsiChar*, Uint16> { using Type = CharsToIntegerTranslator<const AnsiChar*, Uint16>; };
	template <> struct TranslatorBetween<const AnsiChar*, Uint32> { using Type = CharsToIntegerTranslator<const AnsiChar*, Uint32>; };
	template <> struct TranslatorBetween<const AnsiChar*, Uint64> { using Type = CharsToIntegerTranslator<const AnsiChar*, Uint64>; };
	template <> struct TranslatorBetween<const AnsiChar*, float> { using Type = CharsToFloatingPointTranslator<const AnsiChar*, float>; };
	template <> struct TranslatorBetween<const AnsiChar*, double> { using Type = CharsToFloatingPointTranslator<const AnsiChar*, double>; };

	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Int8> { using Type = CharsToIntegerTranslator<AnsiChar[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Int16> { using Type = CharsToIntegerTranslator<AnsiChar[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Int32> { using Type = CharsToIntegerTranslator<AnsiChar[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Int64> { using Type = CharsToIntegerTranslator<AnsiChar[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Uint8> { using Type = CharsToIntegerTranslator<AnsiChar[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Uint16> { using Type = CharsToIntegerTranslator<AnsiChar[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Uint32> { using Type = CharsToIntegerTranslator<AnsiChar[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], Uint64> { using Type = CharsToIntegerTranslator<AnsiChar[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], float> { using Type = CharsToFloatingPointTranslator<AnsiChar[N], float>; };
	template <std::size_t N> struct TranslatorBetween<AnsiChar[N], double> { using Type = CharsToFloatingPointTranslator<AnsiChar[N], double>; };

	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Int8> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Int16> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Int32> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Int64> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Uint8> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Uint16> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Uint32> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], Uint64> { using Type = CharsToIntegerTranslator<const AnsiChar[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], float> { using Type = CharsToFloatingPointTranslator<const AnsiChar[N], float>; };
	template <std::size_t N> struct TranslatorBetween<const AnsiChar[N], double> { using Type = CharsToFloatingPointTranslator<const AnsiChar[N], double>; };

	template <> struct TranslatorBetween<const Utf8Char*, Int8> { using Type = CharsToIntegerTranslator<const Utf8Char*, Int8>; };
	template <> struct TranslatorBetween<const Utf8Char*, Int16> { using Type = CharsToIntegerTranslator<const Utf8Char*, Int16>; };
	template <> struct TranslatorBetween<const Utf8Char*, Int32> { using Type = CharsToIntegerTranslator<const Utf8Char*, Int32>; };
	template <> struct TranslatorBetween<const Utf8Char*, Int64> { using Type = CharsToIntegerTranslator<const Utf8Char*, Int64>; };
	template <> struct TranslatorBetween<const Utf8Char*, Uint8> { using Type = CharsToIntegerTranslator<const Utf8Char*, Uint8>; };
	template <> struct TranslatorBetween<const Utf8Char*, Uint16> { using Type = CharsToIntegerTranslator<const Utf8Char*, Uint16>; };
	template <> struct TranslatorBetween<const Utf8Char*, Uint32> { using Type = CharsToIntegerTranslator<const Utf8Char*, Uint32>; };
	template <> struct TranslatorBetween<const Utf8Char*, Uint64> { using Type = CharsToIntegerTranslator<const Utf8Char*, Uint64>; };
	template <> struct TranslatorBetween<const Utf8Char*, float> { using Type = CharsToFloatingPointTranslator<const Utf8Char*, float>; };
	template <> struct TranslatorBetween<const Utf8Char*, double> { using Type = CharsToFloatingPointTranslator<const Utf8Char*, double>; };

	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Int8> { using Type = CharsToIntegerTranslator<Utf8Char[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Int16> { using Type = CharsToIntegerTranslator<Utf8Char[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Int32> { using Type = CharsToIntegerTranslator<Utf8Char[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Int64> { using Type = CharsToIntegerTranslator<Utf8Char[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Uint8> { using Type = CharsToIntegerTranslator<Utf8Char[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Uint16> { using Type = CharsToIntegerTranslator<Utf8Char[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Uint32> { using Type = CharsToIntegerTranslator<Utf8Char[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], Uint64> { using Type = CharsToIntegerTranslator<Utf8Char[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], float> { using Type = CharsToFloatingPointTranslator<Utf8Char[N], float>; };
	template <std::size_t N> struct TranslatorBetween<Utf8Char[N], double> { using Type = CharsToFloatingPointTranslator<Utf8Char[N], double>; };

	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Int8> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Int16> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Int32> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Int64> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Uint8> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Uint16> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Uint32> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], Uint64> { using Type = CharsToIntegerTranslator<const Utf8Char[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], float> { using Type = CharsToFloatingPointTranslator<const Utf8Char[N], float>; };
	template <std::size_t N> struct TranslatorBetween<const Utf8Char[N], double> { using Type = CharsToFloatingPointTranslator<const Utf8Char[N], double>; };

	template <> struct TranslatorBetween<const Utf16Char*, Int8> { using Type = CharsToIntegerTranslator<const Utf16Char*, Int8>; };
	template <> struct TranslatorBetween<const Utf16Char*, Int16> { using Type = CharsToIntegerTranslator<const Utf16Char*, Int16>; };
	template <> struct TranslatorBetween<const Utf16Char*, Int32> { using Type = CharsToIntegerTranslator<const Utf16Char*, Int32>; };
	template <> struct TranslatorBetween<const Utf16Char*, Int64> { using Type = CharsToIntegerTranslator<const Utf16Char*, Int64>; };
	template <> struct TranslatorBetween<const Utf16Char*, Uint8> { using Type = CharsToIntegerTranslator<const Utf16Char*, Uint8>; };
	template <> struct TranslatorBetween<const Utf16Char*, Uint16> { using Type = CharsToIntegerTranslator<const Utf16Char*, Uint16>; };
	template <> struct TranslatorBetween<const Utf16Char*, Uint32> { using Type = CharsToIntegerTranslator<const Utf16Char*, Uint32>; };
	template <> struct TranslatorBetween<const Utf16Char*, Uint64> { using Type = CharsToIntegerTranslator<const Utf16Char*, Uint64>; };
	template <> struct TranslatorBetween<const Utf16Char*, float> { using Type = CharsToFloatingPointTranslator<const Utf16Char*, float>; };
	template <> struct TranslatorBetween<const Utf16Char*, double> { using Type = CharsToFloatingPointTranslator<const Utf16Char*, double>; };

	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Int8> { using Type = CharsToIntegerTranslator<Utf16Char[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Int16> { using Type = CharsToIntegerTranslator<Utf16Char[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Int32> { using Type = CharsToIntegerTranslator<Utf16Char[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Int64> { using Type = CharsToIntegerTranslator<Utf16Char[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Uint8> { using Type = CharsToIntegerTranslator<Utf16Char[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Uint16> { using Type = CharsToIntegerTranslator<Utf16Char[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Uint32> { using Type = CharsToIntegerTranslator<Utf16Char[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], Uint64> { using Type = CharsToIntegerTranslator<Utf16Char[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], float> { using Type = CharsToFloatingPointTranslator<Utf16Char[N], float>; };
	template <std::size_t N> struct TranslatorBetween<Utf16Char[N], double> { using Type = CharsToFloatingPointTranslator<Utf16Char[N], double>; };

	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Int8> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Int16> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Int32> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Int64> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Uint8> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Uint16> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Uint32> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], Uint64> { using Type = CharsToIntegerTranslator<const Utf16Char[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], float> { using Type = CharsToFloatingPointTranslator<const Utf16Char[N], float>; };
	template <std::size_t N> struct TranslatorBetween<const Utf16Char[N], double> { using Type = CharsToFloatingPointTranslator<const Utf16Char[N], double>; };

	template <> struct TranslatorBetween<String, Int8> { using Type = StringToIntegerTranslator<Int8>; };
	template <> struct TranslatorBetween<String, Int16> { using Type = StringToIntegerTranslator<Int16>; };
	template <> struct TranslatorBetween<String, Int32> { using Type = StringToIntegerTranslator<Int32>; };
	template <> struct TranslatorBetween<String, Int64> { using Type = StringToIntegerTranslator<Int64>; };
	template <> struct TranslatorBetween<String, Uint8> { using Type = StringToIntegerTranslator<Uint8>; };
	template <> struct TranslatorBetween<String, Uint16> { using Type = StringToIntegerTranslator<Uint16>; };
	template <> struct TranslatorBetween<String, Uint32> { using Type = StringToIntegerTranslator<Uint32>; };
	template <> struct TranslatorBetween<String, Uint64> { using Type = StringToIntegerTranslator<Uint64>; };
	template <> struct TranslatorBetween<String, float> { using Type = StringToFloatingPointTranslator<float>; };
	template <> struct TranslatorBetween<String, double> { using Type = StringToFloatingPointTranslator<double>; };

	template <> struct TranslatorBetween<StringView, Int8> { using Type = StringViewToIntegerTranslator<Int8>; };
	template <> struct TranslatorBetween<StringView, Int16> { using Type = StringViewToIntegerTranslator<Int16>; };
	template <> struct TranslatorBetween<StringView, Int32> { using Type = StringViewToIntegerTranslator<Int32>; };
	template <> struct TranslatorBetween<StringView, Int64> { using Type = StringViewToIntegerTranslator<Int64>; };
	template <> struct TranslatorBetween<StringView, Uint8> { using Type = StringViewToIntegerTranslator<Uint8>; };
	template <> struct TranslatorBetween<StringView, Uint16> { using Type = StringViewToIntegerTranslator<Uint16>; };
	template <> struct TranslatorBetween<StringView, Uint32> { using Type = StringViewToIntegerTranslator<Uint32>; };
	template <> struct TranslatorBetween<StringView, Uint64> { using Type = StringViewToIntegerTranslator<Uint64>; };
	template <> struct TranslatorBetween<StringView, float> { using Type = StringViewToFloatingPointTranslator<float>; };
	template <> struct TranslatorBetween<StringView, double> { using Type = StringViewToFloatingPointTranslator<double>; };

	template <> struct TranslatorBetween<Int8, String> { using Type = StringParseIntegerTranslator<Int8>; };
	template <> struct TranslatorBetween<Int16, String> { using Type = StringParseIntegerTranslator<Int16>; };
	template <> struct TranslatorBetween<Int32, String> { using Type = StringParseIntegerTranslator<Int32>; };
	template <> struct TranslatorBetween<Int64, String> { using Type = StringParseIntegerTranslator<Int64>; };
	template <> struct TranslatorBetween<Uint8, String> { using Type = StringParseIntegerTranslator<Uint8>; };
	template <> struct TranslatorBetween<Uint16, String> { using Type = StringParseIntegerTranslator<Uint16>; };
	template <> struct TranslatorBetween<Uint32, String> { using Type = StringParseIntegerTranslator<Uint32>; };
	template <> struct TranslatorBetween<Uint64, String> { using Type = StringParseIntegerTranslator<Uint64>; };
	template <> struct TranslatorBetween<float, String> { using Type = StringParseFloatingPointTranslator<float>; };
	template <> struct TranslatorBetween<double, String> { using Type = StringParseFloatingPointTranslator<double>; };

	template <> struct TranslatorBetween<const TextChar*, String> { using Type = StringConstructionTranslator<const TextChar*>; };
	template <unsigned int TSize> struct TranslatorBetween<TextChar[TSize], String> { using Type = StringConstructionTranslator<TextChar[TSize]>; };
	template <unsigned int TSize> struct TranslatorBetween<const TextChar[TSize], String> { using Type = StringConstructionTranslator<const TextChar[TSize]>; };

	template <typename TDst, typename TSrc, typename Tr = typename TranslatorBetween<TSrc, TDst>::Type>
	Optional<TDst> TryTranslatorCast(const TSrc& value, const Tr& translator = Tr())
	{
		return translator(value);
	}

	template <typename TDst, typename TSrc, typename Tr = typename TranslatorBetween<TSrc, TDst>::Type>
	TDst TranslatorCast(const TSrc& value, const Tr& translator = Tr())
	{
		return translator(value).template GetOrThrow<std::bad_cast>();
	}
}

#endif // COCKTAIL_CORE_UTILITY_TRANSLATORCAST_HPP
