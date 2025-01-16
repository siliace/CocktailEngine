#ifndef COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP
#define COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API TimeUnit
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static TimeUnit Nanoseconds(Uint64 repeat = 1);

		/**
		 * \brief 
		 * \return 
		 */
		static TimeUnit Microseconds(Uint64 repeat = 1);

		/**
		 * \brief 
		 * \return 
		 */
		static TimeUnit Milliseconds(Uint64 repeat = 1);

		/**
		 * \brief 
		 * \return 
		 */
		static TimeUnit Seconds(Uint64 repeat = 1);

		/**
		 * \brief
		 * \param repeat
		 * \return 
		 */
		static TimeUnit Minutes(Uint64 repeat = 1);

		/**
		 * \brief
		 * \return
		 */
		static TimeUnit Hours(Uint64 repeat = 1);

		/**
		 * \brief
		 * \return
		 */
		static TimeUnit Days(Uint64 repeat = 1);

		/**
		 * \brief Constructor
		 */
		TimeUnit(Uint64 ratioNumerator, Uint64 ratioDenominator);

		/**
		 * \brief
		 *
		 * \param other 
		 * \return 
		 */
		bool IsBigger(const TimeUnit& other) const;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		bool IsSmaller(const TimeUnit& other) const;

		/**
		 * \brief
		 * \param count
		 * \param destinationUnit
		 * \return
		 */
		Uint64 ConvertTo(Uint64 count, const TimeUnit& destinationUnit) const;

		/**
		 * \brief 
		 * \param count 
		 * \param sourceUnit 
		 * \return 
		 */
		Uint64 ConvertFrom(Uint64 count, const TimeUnit& sourceUnit) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator==(const TimeUnit& rhs) const;

		/**
		 * \brief 
		 * \param rhs 
		 * \return 
		 */
		bool operator!=(const TimeUnit& rhs) const;

	private:

		Uint64 mRatioNumerator;
		Uint64 mRatioDenominator;
	};
}

#endif // COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP
