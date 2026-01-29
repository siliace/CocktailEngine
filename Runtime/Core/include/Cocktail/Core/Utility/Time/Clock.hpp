#ifndef COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP
#define COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP

#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
    /**
     * \brief Utility class measuring elapsed time from a starting point
     *
     * Clock provides an easy way to measure elapsed time from a specific start
     * point. It supports starting, stopping, resuming, and querying the elapsed
     * duration in a user-defined time unit.
     */
    class COCKTAIL_CORE_API Clock
    {
    public:

        /**
         * \brief Constructor
         *
         * \param timeUnit The time unit the clock should use to count the elapsed time
         */
        explicit Clock(const TimeUnit& timeUnit = TimeUnit::Milliseconds());

        /**
         * \brief Constructor
         *
         * Create a new clock and start it from the provided start instance
         *
         * \param start The time from which to count the elapsed time
         * \param timeUnit The time unit the clock should use to count the elapsed time
         */
        explicit Clock(const Instant& start, const TimeUnit& timeUnit = TimeUnit::Milliseconds());

        /**
         * \brief Start counting time
         *
         * \param start The time from which to count the elapsed time
         */
        void Start(const Instant& start = Instant::Now());

        /**
         * \brief Resume counting time
         *
         * Resumes the clock after it has been stopped.
         */
        void Resume();

        /**
         * \brief Stop counting time
         *
         * Stops the clock without resetting the start time. Elapsed time can still
         * be queried until resumed or restarted.
         */
        void Stop();

        /**
         * \brief Check whether the clock is currently running
         *
         * \return True if the clock is running, false otherwise
         */
        bool IsStarted() const;

        /**
         * \brief Get the elapsed duration since the clock was started
         *
         * \return The elapsed time as a \ref Duration
         */
        Duration GetElapsedDuration() const;

        /**
         * \brief Returns the starting instant of the clock
         *
         * Retrieves the \ref Instant from which the clock started measuring elapsed
         * time. This corresponds to the instant passed to \ref Start or set during
         * construction or reset.
         *
         * \note If the clock has been reset, the returned instant corresponds to the
         *       most recent start time.
         *
         * \return The starting instant of the clock
         */
        Instant GetStart() const;

        /**
         * \brief Get the time unit used by the clock
         *
         * \return The \ref TimeUnit instance representing the clock's resolution
         */
        TimeUnit GetTimeUnit() const;

    private:

        bool mStarted; /*!< True if the clock is currently running */
        Instant mStart; /*!< Instant when the clock was started */
        Instant mStop; /*!< Instant when the clock was stopped */
        TimeUnit mTimeUnit; /*!< Time unit used for measuring elapsed time */
    };
}

#endif // COCKTAIL_CORE_UTILITY_TIME_CLOCK_HPP
