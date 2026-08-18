#ifndef COCKTAILENGINE_CORE_UTILITY_TIME_SYSTEMTIME_HPP
#define COCKTAILENGINE_CORE_UTILITY_TIME_SYSTEMTIME_HPP

#include <CocktailEngine/Core/Export.hpp>

namespace Ck
{
    class Instant;

    /**
     * \class SystemTime
     * \brief Represents a point in time broken down into calendar fields
     *
     * Where an Instant is a single count of time elapsed since the epoch, a
     * SystemTime is the same point in time split into the fields a human reads:
     * year, month, day, hour, minute, second and nanosecond. The two describe the
     * same thing and convert into one another without loss.
     *
     * Use an Instant to store, compare and offset a point in time, and a SystemTime
     * only at the edge where a date has to be read or written. SystemTime offers no
     * comparison or arithmetic on purpose: both are the job of Instant and Duration,
     * which do not have to care about the length of a month.
     *
     * \note A SystemTime is always UTC. The calendar is the proleptic Gregorian one
     * and the day is always 86400 seconds long, which is what makes the conversion to
     * and from Instant exact and free of any time zone or leap second state. Rendering
     * a date in the local time of the machine is a separate concern, and is not what
     * this type does.
     *
     * \note The fields are the real calendar ones, so January is month 1 and the
     * first day of a month is day 1. Only the nanosecond field is zero based.
     *
     * \see Instant
     */
    class COCKTAILENGINE_CORE_API SystemTime
    {
    public:

        /**
         * \brief Returns the current date and time
         *
         * Reads the same clock as Instant::Now, so the two agree.
         *
         * \return SystemTime representing the current UTC date and time
         */
        static SystemTime Now();

        /**
         * \brief Breaks an Instant down into calendar fields
         *
         * \param instant Point in time to break down
         *
         * \return SystemTime describing the same point in time
         */
        static SystemTime FromInstant(const Instant& instant);

        /**
         * \brief Creates a SystemTime from explicit UTC calendar fields
         *
         * \param year Year, which may not be earlier than 1970
         * \param month Month of the year, from 1 for January to 12 for December
         * \param day Day of the month, starting at 1
         * \param hour Hour of the day, from 0 to 23
         * \param minute Minute of the hour, from 0 to 59
         * \param second Second of the minute, from 0 to 59
         * \param nanosecond Nanoseconds within the second, smaller than one second
         *
         * \return SystemTime holding the given fields
         *
         * \remark The fields are taken as given and are not range checked against the
         *         calendar: a 31st of February is accepted and converts to the 3rd or
         *         the 2nd of March, the way a date arithmetic normally overflows
         */
        static SystemTime Utc(Uint64 year, Uint32 month, Uint32 day, Uint32 hour = 0, Uint32 minute = 0, Uint32 second = 0, Uint32 nanosecond = 0);

        /**
         * \brief Constructs the epoch itself
         *
         * The default constructed SystemTime is the 1st of january 1970 at midnight
         * UTC, which is the date a default constructed Instant also describes.
         */
        SystemTime();

        /**
         * \brief Returns the point in time this date describes
         *
         * \return Instant counting from the epoch
         *
         * \warning A SystemTime earlier than the epoch has no Instant to convert to,
         *          an Instant being unsigned, and asserts instead
         */
        Instant ToInstant() const;

        /**
         * \brief Returns the year
         *
         * \return Year, as written on a calendar
         */
        Uint64 GetYear() const;

        /**
         * \brief Returns the month of the year
         *
         * \return Month, from 1 for January to 12 for December
         */
        Uint32 GetMonth() const;

        /**
         * \brief Returns the day of the month
         *
         * \return Day of the month, starting at 1
         */
        Uint32 GetDay() const;

        /**
         * \brief Returns the hour of the day
         *
         * \return Hour, from 0 to 23
         */
        Uint32 GetHour() const;

        /**
         * \brief Returns the minute of the hour
         *
         * \return Minute, from 0 to 59
         */
        Uint32 GetMinute() const;

        /**
         * \brief Returns the second of the minute
         *
         * \return Second, from 0 to 59
         */
        Uint32 GetSecond() const;

        /**
         * \brief Returns the nanoseconds elapsed within the second
         *
         * \return Nanoseconds, always smaller than one second
         */
        Uint32 GetNanosecond() const;

    private:

        /**
         * \brief Constructs a SystemTime from its fields
         *
         * \param year Year, which may not be earlier than 1970
         * \param month Month of the year, from 1 to 12
         * \param day Day of the month, starting at 1
         * \param hour Hour of the day, from 0 to 23
         * \param minute Minute of the hour, from 0 to 59
         * \param second Second of the minute, from 0 to 59
         * \param nanosecond Nanoseconds within the second, smaller than one second
         */
        SystemTime(Uint64 year, Uint32 month, Uint32 day, Uint32 hour, Uint32 minute, Uint32 second, Uint32 nanosecond);

        Uint64 mYear; /*!< Year, as written on a calendar */
        Uint32 mMonth; /*!< Month of the year, from 1 to 12 */
        Uint32 mDay; /*!< Day of the month, starting at 1 */
        Uint32 mHour; /*!< Hour of the day, from 0 to 23 */
        Uint32 mMinute; /*!< Minute of the hour, from 0 to 59 */
        Uint32 mSecond; /*!< Second of the minute, from 0 to 59 */
        Uint32 mNanosecond; /*!< Nanoseconds within the second */
    };
}

#endif // COCKTAILENGINE_CORE_UTILITY_TIME_SYSTEMTIME_HPP
