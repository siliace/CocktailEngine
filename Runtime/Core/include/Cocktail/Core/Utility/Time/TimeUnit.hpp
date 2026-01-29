#ifndef COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP
#define COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    /**
     * \class TimeUnit
     * \brief Represents a discrete time quantum based on a rational ratio
     *
     * A TimeUnit defines a time scale expressed as a ratio
     * (numerator / denominator), optionally multiplied by a repetition factor.
     *
     * This class is designed to represent fixed time quanta such as:
     * - application cycles (eg 16 ms per frame),
     * - simulation ticks,
     * - logical time steps
     *
     * A TimeUnit is not a duration by itself, but a base unit used
     * to express durations, conversions and alignments between time scales.
     */
    class COCKTAIL_CORE_API TimeUnit
    {
    public:

        /**
         * \brief Creates a time unit representing a number of nanoseconds
         *
         * \param repeat Number of nanoseconds composing the unit
         *
         * \return A TimeUnit representing repeat nanoseconds
         */
        static TimeUnit Nanoseconds(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of microseconds
         *
         * \param repeat Number of microseconds composing the unit
         *
         * \return A TimeUnit representing repeat microseconds
         */
        static TimeUnit Microseconds(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of milliseconds
         *
         * \param repeat Number of milliseconds composing the unit
         *
         * \return A TimeUnit representing repeat milliseconds
         */
        static TimeUnit Milliseconds(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of seconds
         *
         * \param repeat Number of seconds composing the unit
         *
         * \return A TimeUnit representing repeat seconds
         */
        static TimeUnit Seconds(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of minutes
         *
         * \param repeat Number of minutes composing the unit
         *
         * \return A TimeUnit representing repeat minutes
         */
        static TimeUnit Minutes(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of hours
         *
         * \param repeat Number of hours composing the unit
         *
         * \return A TimeUnit representing repeat hours
         */
        static TimeUnit Hours(Uint64 repeat = 1);

        /**
         * \brief Creates a time unit representing a number of days
         *
         * \param repeat Number of days composing the unit
         *
         * \return A TimeUnit representing repeat days
         */
        static TimeUnit Days(Uint64 repeat = 1);

        /**
         * \brief Constructs a time unit from a rational ratio
         *
         * The ratio represents the duration of the unit as:
         *   ratioNumerator / ratioDenominator seconds.
         *
         * \param ratioNumerator Numerator of the time ratio
         * \param ratioDenominator Denominator of the time ratio
         */
        TimeUnit(Uint64 ratioNumerator, Uint64 ratioDenominator);

        /**
         * \brief Checks whether this time unit is an exact multiple of another
         *
         * This method returns true if this unit can be expressed as
         * an integer number of repetitions of the other unit.
         *
         * Example:
         * - 16 milliseconds is a multiple of 1 millisecond
         * - 1 second is a multiple of 250 milliseconds
         *
         * \param other The reference time unit
         *
         * \return True if this unit is a multiple of the other unit
         */
        bool IsMultipleOf(const TimeUnit& other) const;

        /**
         * \brief Checks whether this time unit represents a larger time quantum
         *        than another
         *
         * \param other The time unit to compare against
         * \return True if this unit is larger than the other
         */
        bool IsBigger(const TimeUnit& other) const;

        /**
         * \brief Checks whether this time unit represents a smaller time quantum
         *        than another
         *
         * \param other The time unit to compare against
         * \return True if this unit is smaller than the other
         */
        bool IsSmaller(const TimeUnit& other) const;

        /**
         * \brief Converts a count expressed in this unit to another unit
         *
         * This method converts a quantity expressed in this time unit
         * to an equivalent quantity expressed in the destination unit.
         *
         * \param count The number of units to convert
         * \param destinationUnit The target time unit
         * \return The converted count expressed in the destination unit
         *
         * \note This conversion may overflow if the resulting value
         *       exceeds the range of Uint64
         */
        Uint64 ConvertTo(Uint64 count, const TimeUnit& destinationUnit) const;

        /**
         * \brief Converts a count expressed in another unit to this unit
         *
         * \param count The number of source units to convert
         * \param sourceUnit The unit in which the count is expressed
         *
         * \return The converted count expressed in this unit
         *
         * \note This conversion may overflow if the resulting value
         *       exceeds the range of Uint64
         */
        Uint64 ConvertFrom(Uint64 count, const TimeUnit& sourceUnit) const;

        /**
         * \brief Checks whether two time units are equivalent
         *
         * \param rhs The time unit to compare against
         *
         * \return True if both units represent the same time quantum
         */
        bool operator==(const TimeUnit& rhs) const;

        /**
         * \brief Checks whether two time units are different
         *
         * \param rhs The time unit to compare against
         * 
         * \return True if the units are different
         */
        bool operator!=(const TimeUnit& rhs) const;

    private:

        Uint64 mRatioNumerator;
        Uint64 mRatioDenominator;
    };
}

#endif // COCKTAIL_CORE_UTILITY_TIME_TIMEUNIT_HPP
