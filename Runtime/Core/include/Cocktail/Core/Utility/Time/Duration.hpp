#ifndef COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP
#define COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP

#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
    class Instant;

    /**
     * \brief Utility class representing the amount of time between two Instants
     *
     * A Duration stores a time interval using an integer count and a unit of time.
     * It can represent durations from nanoseconds to days, and supports infinite
     * durations.
     *
     * \see Instant
     * \see TimeUnit
     */
    class COCKTAIL_CORE_API Duration
    {
    public:

        /**
         * \brief Returns a Duration representing an infinite interval
         *
         * \return A Duration considered infinite
         */
        static Duration Infinite();

        /**
         * \brief Creates a Duration representing a number of nanoseconds
         *
         * \param count Number of nanoseconds
         *
         * \return Duration corresponding to the specified number of nanoseconds
         */
        static Duration Nanoseconds(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of microseconds
         *
         * \param count Number of microseconds
         *
         * \return Duration corresponding to the specified number of microseconds
         */
        static Duration Microseconds(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of milliseconds
         *
         * \param count Number of milliseconds
         *
         * \return Duration corresponding to the specified number of milliseconds
         */
        static Duration Milliseconds(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of seconds
         *
         * \param count Number of seconds
         *
         * \return Duration corresponding to the specified number of seconds
         */
        static Duration Seconds(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of minutes
         *
         * \param count Number of minutes
         *
         * \return Duration corresponding to the specified number of minutes
         */
        static Duration Minutes(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of hours
         *
         * \param count Number of hours
         *
         * \return Duration corresponding to the specified number of hours
         */
        static Duration Hours(Uint64 count);

        /**
         * \brief Creates a Duration representing a number of days
         *
         * \param count Number of days
         *
         * \return Duration corresponding to the specified number of days
         */
        static Duration Days(Uint64 count);

        /**
         * \brief Computes the Duration between two Instants
         *
         * \param lhs First instant
         * \param rhs Second instant
         *
         * \return Duration representing the difference rhs - lhs
         */
        static Duration Between(const Instant& lhs, const Instant& rhs);

        /**
         * \brief Computes the difference between two Durations
         *
         * \param lhs First Duration
         * \param rhs Second Duration
         *
         * \return Duration corresponding to rhs - lhs
         */
        static Duration Between(const Duration& lhs, const Duration& rhs);

        /**
         * \brief Default constructor
         *
         * Initializes a Duration to 0 seconds.
         */
        Duration();

        /**
         * \brief Constructs a Duration from a count and unit
         *
         * \param count Number of units
         * \param unit Time unit
         */
        Duration(Uint64 count, const TimeUnit& unit);

        /**
         * \brief Converts the Duration to another time unit
         *
         * \param unit Target time unit
         *
         * \return Duration expressed in the requested unit
         */
        Duration As(const TimeUnit& unit) const;

        /**
         * \brief Checks whether the Duration is infinite
         *
         * \return true if the Duration represents an infinite interval, false otherwise
         */
        bool IsInfinite() const;

        /**
         * \brief Returns the internal count of time units
         *
         * \return Count of time units stored in the Duration
         */
        Uint64 GetCount() const;

        /**
         * \brief Returns the count converted to a specified time unit
         *
         * \param unit Target time unit
         *
         * \return Count of time in the requested unit
         */
        Uint64 GetCount(const TimeUnit& unit) const;

        /**
         * \brief Returns the unit used internally by this Duration
         *
         * \return Reference to the TimeUnit of this Duration
         */
        const TimeUnit& GetUnit() const;

    private:

        Uint64 mCount; /*!< Number of time units */
        TimeUnit mUnit; /*!< Time unit used */
    };

    /**
     * \brief Equality operator for Duration
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if both Durations represent the same interval, false otherwise
     */
    COCKTAIL_CORE_API bool operator==(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Inequality operator for Duration
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if the Durations are not equal, false otherwise
     */
    COCKTAIL_CORE_API bool operator!=(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Adds two Durations
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return Duration representing lhs + rhs
     */
    COCKTAIL_CORE_API Duration operator+(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Adds a Duration to another Duration in-place
     *
     * \param lhs Duration to update
     * \param rhs Duration to add
     * \return Reference to the updated lhs
     */
    COCKTAIL_CORE_API Duration& operator+=(Duration& lhs, const Duration& rhs);

    /**
     * \brief Less-than comparison between two Durations
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if lhs is smaller than rhs, false otherwise
     */
    COCKTAIL_CORE_API bool operator<(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Less-than-or-equal comparison between two Durations
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if lhs is smaller than or equal to rhs, false otherwise
     */
    COCKTAIL_CORE_API bool operator<=(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Greater-than comparison between two Durations
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if lhs is greater than rhs, false otherwise
     */
    COCKTAIL_CORE_API bool operator>(const Duration& lhs, const Duration& rhs);

    /**
     * \brief Greater-than-or-equal comparison between two Durations
     *
     * \param lhs First Duration
     * \param rhs Second Duration
     * \return true if lhs is greater than or equal to rhs, false otherwise
     */
    COCKTAIL_CORE_API bool operator>=(const Duration& lhs, const Duration& rhs);
}

#endif // COCKTAIL_CORE_UTILITY_TIME_DURATION_HPP
