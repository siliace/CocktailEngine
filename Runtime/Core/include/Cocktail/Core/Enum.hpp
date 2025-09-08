#ifndef COCKTAIL_CORE_ENUM_HPP
#define COCKTAIL_CORE_ENUM_HPP

#include <array>
#include <stdexcept>
#include <type_traits>

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 4096
#include <magic_enum/magic_enum.hpp>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

namespace Ck
{
	/**
	 * \brief Utility class to access to enumeration properties
	 * \tparam T 
	 */
	template <typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
	class Enum
	{
	public:

		/**
		 * \brief 
		 * \tparam U 
		 * \param value 
		 * \return 
		 */
		template <typename U, typename = std::enable_if_t<std::is_enum_v<U>>>
		static T Cast(U value)
		{
			return Enum<T>::FromValue(static_cast<std::underlying_type_t<T>>(value));
		}

		/**
		 * \brief
		 * \tparam U
		 * \param value
		 * \return
		 */
		template <typename U, typename = std::enable_if_t<std::is_enum_v<U>>>
		static T BitmaskCast(U value)
		{
			auto underlying = Enum<U>::UnderlyingCast(value);
			auto bitmask = Bit(underlying);
			return Enum<T>::FromValue(bitmask);
		}

		/**
		 * \brief 
		 * \param value 
		 * \return 
		 */
		static std::underlying_type_t<T> UnderlyingCast(T value)
		{
			return static_cast<std::underlying_type_t<T>>(value);
		}

		/**
		 * \brief 
		 * \param value 
		 * \return 
		 */
		static T FromValue(std::underlying_type_t<T> value)
		{
			auto enumValue = magic_enum::enum_cast<T>(value);
			if (!enumValue.has_value())
				throw std::invalid_argument("Failed to number value to enumeration value");

			return enumValue.value();
		}

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		static T FromString(const AnsiChar* name)
		{
			auto enumValue = magic_enum::enum_cast<T>(name);
			if (!enumValue.has_value())
				throw std::invalid_argument("Failed to convert string to enumeration value");

			return enumValue.value();
		}

		/**
		 * \brief 
		 * \param value 
		 * \return 
		 */
		static const AnsiChar* ToString(T value)
		{
			return magic_enum::enum_name(value).data();
		}

		/**
		 * \brief 
		 * \param value 
		 * \return 
		 */
		static std::size_t IndexOf(T value)
		{
			auto opt = magic_enum::enum_index<T>(value);
			if (opt.has_value())
				return opt.value();

			throw std::invalid_argument("Failed to get index of enumeration value");
		}

		/**
		 * \brief 
		 */
		constexpr static std::size_t ValueCount =  magic_enum::enum_count<T>();

		/**
		 * \brief 
		 */
		constexpr static std::array<T, ValueCount> Values = magic_enum::enum_values<T>();
	};
}

#endif // COCKTAIL_CORE_ENUM_HPP
