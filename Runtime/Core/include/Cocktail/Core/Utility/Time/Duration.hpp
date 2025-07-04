#ifndef COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP
#define COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP

#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
	class Instant;

	/**
	 * \brief Utility class representing the amount of time between two Instant
	 * \see Instant
	 * \see TimeUnit
	 */
	class COCKTAIL_CORE_API Duration
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Duration Infinite();

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Nanoseconds(Uint64 count);

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Microseconds(Uint64 count);

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Milliseconds(Uint64 count);

		/**
		 * \brief 
		 * \param count 
		 * \return 
		 */
		static Duration Seconds(Uint64 count);

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Minutes(Uint64 count);

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Hours(Uint64 count);

		/**
		 * \brief
		 * \param count
		 * \return
		 */
		static Duration Days(Uint64 count);

		/**
		 * \brief 
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		static Duration Between(const Instant& lhs, const Instant& rhs);

		/**
		 * \brief
		 * \param lhs
		 * \param rhs
		 * \return
		 */
		static Duration Between(const Duration& lhs, const Duration& rhs);

		/**
		 * \brief Default constructor
		 * Initialize a duration to 0 seconds
		 */
		Duration();

		/**
		 * \brief
		 * \param count
		 * \param unit
		 */
		Duration(Uint64 count, const TimeUnit& unit);

		/**
		 * \brief Convert the duration to another TimeUnit
		 * \param unit 
		 * \return 
		 */
		Duration As(const TimeUnit& unit) const;

		/**
		 * \brief
		 * \param
		 * \return
		 */
		bool IsInfinite() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetCount() const;

		/**
		 * \brief
		 * \return
		 */
		Uint64 GetCount(const TimeUnit& unit) const;

		/**
		 * \brief 
		 * \return 
		 */
		const TimeUnit& GetUnit() const;

	private:

		Uint64 mCount;
		TimeUnit mUnit;
	};

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	COCKTAIL_CORE_API bool operator==(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	COCKTAIL_CORE_API bool operator!=(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	COCKTAIL_CORE_API Duration operator+(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	COCKTAIL_CORE_API Duration& operator+=(Duration& lhs, const Duration& rhs);

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	COCKTAIL_CORE_API bool operator<(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	COCKTAIL_CORE_API bool operator<=(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	COCKTAIL_CORE_API bool operator>(const Duration& lhs, const Duration& rhs);

	/**
	 * \brief 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	COCKTAIL_CORE_API bool operator>=(const Duration& lhs, const Duration& rhs);
}

#endif // COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP
