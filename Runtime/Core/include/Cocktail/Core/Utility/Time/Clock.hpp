#ifndef COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP
#define COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP

#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
	/**
	 * \brief Utility class measuring elapsed time from a starting point
	 */
	class COCKTAIL_CORE_API Clock
	{
	public:

		/**
		 * \brief Constructor
		 * \param timeUnit The time unit the clock should use to count the elapsed time
		 */
		explicit Clock(const TimeUnit& timeUnit = TimeUnit::Milliseconds());
		
		/**
		 * \brief Constructor
		 * Create a new clock and start it from the provided start instance
		 * \param start The time from which count the elapsed time 
		 * \param timeUnit The time unit the clock should use to count the elapsed time
		 */
		explicit Clock(const Instant& start, const TimeUnit& timeUnit = TimeUnit::Milliseconds());

		/**
		 * \brief Start counting time
		 * \param start The time from which count the elapsed time 
		 */
		void Start(const Instant& start = Instant::Now());

		/**
		 * \brief Resume to count time
		 */
		void Resume();

		/**
		 * \brief Stop counting time
		 */
		void Stop();

		/**
		 * \brief Tell whether the clock is counting time
		 * \return True if the clock is running, false otherwise
		 */
		bool IsStarted() const;

		/**
		 * \brief Get how must time elapsed from the start of the clock
		 * \return The elapsed time
		 */
		Duration GetElapsedDuration() const;

	private:

		bool mStarted;
		Instant mStart;
		Instant mStop;
		TimeUnit mTimeUnit;
	};
}

#endif // COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP
