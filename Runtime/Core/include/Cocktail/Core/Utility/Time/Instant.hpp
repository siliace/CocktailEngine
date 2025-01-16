#ifndef COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP
#define COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class Duration;

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Instant
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Instant Now();

		/**
		 * \brief 
		 * \param milliseconds 
		 * \return 
		 */
		static Instant EpochMilliseconds(Uint64 milliseconds);

		/**
		 * \brief 
		 * \param seconds 
		 * \param nanoseconds 
		 * \return 
		 */
		static Instant EpochSeconds(Uint64 seconds, Uint64 nanoseconds);

		/**
		 * \brief 
		 */
		Instant();

		/**
		 * \brief 
		 * \param offset 
		 * \return 
		 */
		Instant After(const Duration& offset) const;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		bool IsAfter(const Instant& other) const;

		/**
		 * \brief 
		 * \param offset 
		 * \return 
		 */
		Instant Before(const Duration& offset) const;

		/**
		 * \brief
		 * \param other
		 * \return
		 */
		bool IsBefore(const Instant& other) const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetSeconds() const;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 GetNanoseconds() const;

	private:

		/**
		 * \brief 
		 * \param seconds 
		 * \param nanoseconds 
		 */
		Instant(Uint64 seconds, Uint64 nanoseconds);

		Uint64 mSeconds;
		Uint64 mNanoseconds;
	};
}
#endif // COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP
