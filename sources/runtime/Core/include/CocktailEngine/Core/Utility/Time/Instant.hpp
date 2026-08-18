#ifndef COCKTAILENGINE_CORE_UTILITY_TIME_INSTANT_HPP
#define COCKTAILENGINE_CORE_UTILITY_TIME_INSTANT_HPP

#include <CocktailEngine/Core/Export.hpp>

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
     *
     * \note Every Instant counts from the Unix epoch, 1st january 1970 UTC, on every
     * platform. Two instants are therefore comparable whichever they came from, be it
     * Now, a file time read from the file system, or an epoch count supplied by the
     * caller. Platform back-ends are responsible for reconciling their native origin
     * with this one.
     *
     * \note An Instant is always stored normalized: the nanoseconds component is
     * guaranteed to be in the [0, 999999999] range, any excess being carried over
     * to the seconds component. Platform APIs expecting a POSIX timespec rely on
     * this invariant.
     *
     * \warning Counting from the epoch means reading a wall clock, which an operator
     * or a time synchronisation daemon can move. An Instant is the right type to
     * record when something happened, and a deadline built from one can be reached
     * earlier or later than the wall time suggests if the clock is adjusted while a
     * thread waits on it. Use a Duration, which is measured and not dated, when an
     * interval is what matters.
     */
    class COCKTAILENGINE_CORE_API Instant
    {
    public:

        /**
         * \brief Returns the current instant
         *
         * Reads the system wall clock, so the result counts from the epoch and can be
         * compared against any other Instant. Successive calls are not guaranteed to
         * be ordered: adjusting the system clock moves this value, backwards included.
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
         * The nanoseconds component does not have to be smaller than one second,
         * any excess is carried over to the seconds component.
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
         * \return Nanoseconds offset within the current second, always smaller
         *         than one second
         */
        Uint32 GetNanoseconds() const;

    private:

        /**
         * \brief Constructs an instant from seconds and nanoseconds
         *
         * The components are normalized so that the stored nanoseconds offset is
         * always smaller than one second.
         *
         * \param seconds Number of seconds since the epoch
         * \param nanoseconds Nanoseconds offset, may be larger than one second
         */
        Instant(Uint64 seconds, Uint64 nanoseconds);

        Uint64 mSeconds;
        Uint32 mNanoseconds;
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_TIME_INSTANT_HPP
