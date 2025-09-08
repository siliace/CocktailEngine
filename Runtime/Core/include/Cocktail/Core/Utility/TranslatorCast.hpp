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
					result.Prepend(static_cast<TextChar>(CK_TEXT('0') + (n % 10)));
					n /= 10;
				}

				if (isNegative)
					result.Prepend(CK_TEXT('-'));
			}
			else
			{
				while (value > 0)
				{
					result.Prepend(static_cast<TextChar>(CK_TEXT('0') + (value % 10)));
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
				result.Prepend(CK_TEXT('-'));

			result.Append(CK_TEXT("."));
			for (int i = 0; i < std::numeric_limits<InputType>::digits; i++)
			{
				fractionalPart *= 10;
				int digit = static_cast<int>(fractionalPart);
				result.Append(static_cast<TextChar>(CK_TEXT('0') + digit));

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
			return StringUtils<Detail::NakedType<T>>::template TryToInteger<E>(value);
		}
	};

	template <typename T, typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct CharsToFloatingPointTranslator
	{
		using InputType = T;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const
		{
			return StringUtils<Detail::NakedType<T>>::template TryToFloatingPoint<E>(value);
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct StringToIntegerTranslator
	{
		using InputType = String;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			if (!value.IsNumeric())
				return Optional<OutputType>::Empty();

			return StringUtils<TextChar>::TryToInteger<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct StringToFloatingPointTranslator
	{
		using InputType = String;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			if (!value.IsNumeric())
				return Optional<OutputType>::Empty();

			return StringUtils<TextChar>::TryToFloatingPoint<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_integral_v<E>>>
	struct StringViewToIntegerTranslator
	{
		using InputType = StringView;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			if (!value.IsNumeric())
				return Optional<OutputType>::Empty();

			return StringUtils<TextChar>::TryToInteger<E>(value.GetData());
		}
	};

	template <typename E, typename = std::enable_if_t<std::is_floating_point_v<E>>>
	struct StringViewToFloatingPointTranslator
	{
		using InputType = StringView;
		using OutputType = E;

		Optional<OutputType> operator()(const InputType& value) const noexcept
		{
			if (!value.IsNumeric())
				return Optional<OutputType>::Empty();

			return StringUtils<TextChar>::TryToFloatingPoint<E>(value.GetData());
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
				return Optional<OutputType>::Of(InPlace, CK_ANSI_TO_TEXT(value));
			}
			else if constexpr (std::is_same_v<Decayed, const TextChar*> || std::is_same_v<Decayed, TextChar*>)
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

	template <> struct TranslatorBetween<const TextChar*, Int8> { using Type = CharsToIntegerTranslator<const TextChar*, Int8>; };
	template <> struct TranslatorBetween<const TextChar*, Int16> { using Type = CharsToIntegerTranslator<const TextChar*, Int16>; };
	template <> struct TranslatorBetween<const TextChar*, Int32> { using Type = CharsToIntegerTranslator<const TextChar*, Int32>; };
	template <> struct TranslatorBetween<const TextChar*, Int64> { using Type = CharsToIntegerTranslator<const TextChar*, Int64>; };
	template <> struct TranslatorBetween<const TextChar*, Uint8> { using Type = CharsToIntegerTranslator<const TextChar*, Uint8>; };
	template <> struct TranslatorBetween<const TextChar*, Uint16> { using Type = CharsToIntegerTranslator<const TextChar*, Uint16>; };
	template <> struct TranslatorBetween<const TextChar*, Uint32> { using Type = CharsToIntegerTranslator<const TextChar*, Uint32>; };
	template <> struct TranslatorBetween<const TextChar*, Uint64> { using Type = CharsToIntegerTranslator<const TextChar*, Uint64>; };
	template <> struct TranslatorBetween<const TextChar*, float> { using Type = CharsToFloatingPointTranslator<const TextChar*, float>; };
	template <> struct TranslatorBetween<const TextChar*, double> { using Type = CharsToFloatingPointTranslator<const TextChar*, double>; };

	template <std::size_t N> struct TranslatorBetween<TextChar[N], Int8> { using Type = CharsToIntegerTranslator<TextChar[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Int16> { using Type = CharsToIntegerTranslator<TextChar[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Int32> { using Type = CharsToIntegerTranslator<TextChar[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Int64> { using Type = CharsToIntegerTranslator<TextChar[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Uint8> { using Type = CharsToIntegerTranslator<TextChar[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Uint16> { using Type = CharsToIntegerTranslator<TextChar[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Uint32> { using Type = CharsToIntegerTranslator<TextChar[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], Uint64> { using Type = CharsToIntegerTranslator<TextChar[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], float> { using Type = CharsToFloatingPointTranslator<TextChar[N], float>; };
	template <std::size_t N> struct TranslatorBetween<TextChar[N], double> { using Type = CharsToFloatingPointTranslator<TextChar[N], double>; };

	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Int8> { using Type = CharsToIntegerTranslator<const TextChar[N], Int8>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Int16> { using Type = CharsToIntegerTranslator<const TextChar[N], Int16>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Int32> { using Type = CharsToIntegerTranslator<const TextChar[N], Int32>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Int64> { using Type = CharsToIntegerTranslator<const TextChar[N], Int64>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Uint8> { using Type = CharsToIntegerTranslator<const TextChar[N], Uint8>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Uint16> { using Type = CharsToIntegerTranslator<const TextChar[N], Uint16>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Uint32> { using Type = CharsToIntegerTranslator<const TextChar[N], Uint32>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], Uint64> { using Type = CharsToIntegerTranslator<const TextChar[N], Uint64>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], float> { using Type = CharsToFloatingPointTranslator<const TextChar[N], float>; };
	template <std::size_t N> struct TranslatorBetween<const TextChar[N], double> { using Type = CharsToFloatingPointTranslator<const TextChar[N], double>; };

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

	template <> struct TranslatorBetween<const AnsiChar*, String> { using Type = StringConstructionTranslator<const AnsiChar*>; };
	template <> struct TranslatorBetween<const WildChar*, String> { using Type = StringConstructionTranslator<const WildChar*>; };

	template <unsigned int TSize> struct TranslatorBetween<AnsiChar[TSize], String> { using Type = StringConstructionTranslator<AnsiChar[TSize]>; };
	template <unsigned int TSize> struct TranslatorBetween<const AnsiChar[TSize], String> { using Type = StringConstructionTranslator<const AnsiChar[TSize]>; };
	template <unsigned int TSize> struct TranslatorBetween<WildChar[TSize], String> { using Type = StringConstructionTranslator<WildChar[TSize]>; };
	template <unsigned int TSize> struct TranslatorBetween<const WildChar[TSize], String> { using Type = StringConstructionTranslator<const WildChar[TSize]>; };

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
