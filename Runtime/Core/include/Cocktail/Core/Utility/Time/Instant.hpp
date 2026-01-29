#ifndef COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP
#define COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
    class Duration;

    /**
     * \class Instant
     * \brief Represents an absolute point in time
     *
     * An Instant represents a fixed position on a time line.
     * It is independent of durations and only expresses ordering
     * and absolute timestamps.
     *
     * Instants can be compared to determine temporal ordering
     * and can be offset using durations to produce new instants.
     */
    class COCKTAIL_CORE_API Instant
    {
    public:

        /**
         * \brief Returns the current instant
         *
         * \return Instant representing the current time
         */
        static Instant Now();

        /**
         * \brief Creates an instant from milliseconds since the epoch
         *
         * \param milliseconds Number of milliseconds since the epoch
         *
         * \return Instant corresponding to the given epoch time
         */
        static Instant EpochMilliseconds(Uint64 milliseconds);

        /**
         * \brief Creates an instant from seconds and nanoseconds since the epoch
         *
         * \param seconds Number of seconds since the epoch
         * \param nanoseconds Additional nanoseconds offset
         *
         * \return Instant corresponding to the given epoch time
         */
        static Instant EpochSeconds(Uint64 seconds, Uint64 nanoseconds);

        /**
         * \brief Constructs an invalid or zero instant
         *
         * The default constructed instant represents a neutral
         * or zero-initialized time point
         */
        Instant();

        /**
         * \brief Returns an instant occurring after this one by a given duration
         *
         * \param offset Duration to add to this instant
         *
         * \return New instant offset forward in time
         */
        Instant After(const Duration& offset) const;

        /**
         * \brief Checks whether this instant occurs after another
         *
         * \param other Instant to compare against
         *
         * \return True if this instant is strictly after the other
         */
        bool IsAfter(const Instant& other) const;

        /**
         * \brief Returns an instant occurring before this one by a given duration
         *
         * \param offset Duration to subtract from this instant
         *
         * \return New instant offset backward in time
         */
        Instant Before(const Duration& offset) const;

        /**
         * \brief Checks whether this instant occurs before another
         *
         * \param other Instant to compare against
         *
         * \return True if this instant is strictly before the other
         */
        bool IsBefore(const Instant& other) const;

        /**
         * \brief Returns the seconds component of this instant
         *
         * \return Number of seconds since the epoch
         */
        Uint64 GetSeconds() const;

        /**
         * \brief Returns the nanoseconds component of this instant
         *
         * \return Nanoseconds offset within the current second
         */
        Uint64 GetNanoseconds() const;

    private:

        /**
         * \brief Constructs an instant from seconds and nanoseconds
         *
         * \param seconds Number of seconds since the epoch
         * \param nanoseconds Nanoseconds offset within the second
         */
        Instant(Uint64 seconds, Uint64 nanoseconds);

        Uint64 mSeconds;
        Uint64 mNanoseconds;
    };
}

#endif // COCKTAIL_CORE_UTILITY_TIME_INSTANT_HPP
