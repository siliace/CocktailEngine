#ifndef COCKTAIL_CORE_UTILITY_TIME_CHRONOMETER_HPP
#define COCKTAIL_CORE_UTILITY_TIME_CHRONOMETER_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Utility/Time/Clock.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    /**
     * \brief A chronometer for measuring elapsed time and recording laps
     *
     * Chronometer records laps, which are intervals of time measured from the last mark or start.
     * Each lap stores its start instant and its duration. Chronometer is useful for timing events,
     * benchmarking, or measuring repeated intervals.
     */
    class COCKTAIL_CORE_API Chronometer
    {
    public:

        /**
         * \brief Represents a single lap measured by the Chronometer
         */
        struct Lap
        {
            Instant Start; /*!< The starting instant of the lap */
            Duration Time; /*!< The duration of the lap */
        };

        /**
         * \brief Constructs a chronometer and starts counting from now
         *
         * \param timeUnit The time unit to use for elapsed durations
         */
        explicit Chronometer(const TimeUnit& timeUnit = TimeUnit::Milliseconds());

        /**
         * \brief Marks the end of the current lap and starts a new lap
         *
         * Records the elapsed time since the last mark (or start) and appends a
         * new lap to the internal lap list. Then resets the current lap start to
         * the current instant.
         *
         * \warning The chronometer must be started when calling this method.
         */
        void Mark();

        /**
         * \brief Resets the chronometer and clears all recorded laps
         *
         * Clears the internal lap list and resets the current lap to start at
         * the current instant with zero duration.
         */
        void Reset();

        /**
         * \brief Returns the list of all recorded laps
         *
         * \return A constant reference to the array of laps
         */
        const Array<Lap>& GetLaps() const;

    private:

        /**
         * \brief Starts a new lap from the current instant
         *
         * Resets and starts the internal clock, initializes the current lap start
         * instant and resets its duration to zero using the clock's time unit.
         */
        void OnBeginLap();

        Clock mClock; /*!< Internal clock used to measure lap durations */
        Lap mCurrentLap; /*!< The lap currently being recorded */
        Array<Lap> mLaps; /*!< All completed laps */
    };
}

#endif // COCKTAIL_CORE_UTILITY_TIME_CHRONOMETER_HPP
