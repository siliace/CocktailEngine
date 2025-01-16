#ifndef COCKTAIL_CORE_UTILITY_ENUMMAP_HPP
#define COCKTAIL_CORE_UTILITY_ENUMMAP_HPP

#include <array>
#include <optional>

#include <Cocktail/Core/Enum.hpp>

namespace Ck
{
	/**
	 * \brief Utility class mapping a value of an enum to another value
	 * \tparam Key 
	 * \tparam Value 
	 */
	template <typename Key, typename Value, typename = std::enable_if_t<std::is_enum_v<Key>>>
	class EnumMap
	{
	public:

		/**
		 * \brief 
		 */
		EnumMap() :
			mValues{}
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param other 
		 */
		EnumMap(const EnumMap& other) :
			mValues{}
		{
			std::copy(other.mValues.begin(), other.mValues.end(), mValues.begin());
		}

		/**
		 * \brief 
		 * \param other 
		 */
		EnumMap(EnumMap&& other) noexcept :
			mValues(std::move(other.mValues))
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		EnumMap& operator=(const EnumMap& other)
		{
			std::copy(other.mValues.begin(), other.mValues.end(), mValues.begin());

			return *this;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		EnumMap& operator=(EnumMap&& other) noexcept
		{
			mValues = std::move(other.mValues);
			
			return *this;
		}

		/**
		 * \brief 
		 * \param key 
		 * \return 
		 */
		Value& operator[](Key key)
		{
			return mValues[
				Enum<Key>::IndexOf(key)
			];
		}

		/**
		 * \brief 
		 * \param key 
		 * \return 
		 */
		const Value& operator[](Key key) const
		{
			return mValues[
				Enum<Key>::IndexOf(key)
			];
		}

	private:

		std::array<Value, Enum<Key>::ValueCount> mValues;
	};
}

#endif // COCKTAIL_CORE_UTILITY_ENUMMAP_HPP
