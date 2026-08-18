#include <cassert>

#include <CocktailEngine/Core/Utility/Time/Instant.hpp>
#include <CocktailEngine/Core/Utility/Time/SystemTime.hpp>

namespace Ck
{
    namespace
    {
        constexpr Uint64 SecondsPerMinute = 60;
        constexpr Uint64 SecondsPerHour = 60 * SecondsPerMinute;
        constexpr Uint64 SecondsPerDay = 24 * SecondsPerHour;

        /// Days in the 400 year cycle the Gregorian calendar repeats on
        constexpr Int64 DaysPerEra = 146097;

        /// Days between the 1st of march of year 0 and the 1st of january 1970. The
        /// algorithms below count from march, which is what puts the leap day at the
        /// end of the year and lets a single division find the month
        constexpr Int64 DaysFromZeroToEpoch = 719468;

        /**
         * \brief A date with no time of day attached
         */
        struct CivilDate
        {
            Uint64 Year;
            Uint32 Month;
            Uint32 Day;
        };

        /**
         * \brief Returns the number of days from the epoch to a calendar date
         *
         * Shifts the year to start in march, so that february and its leap day sit
         * last and the day of year becomes a straight line in the month index. The
         * era is the 400 year cycle the Gregorian calendar repeats on, which is what
         * removes every special case for leap years.
         *
         * \param year Year, as written on a calendar
         * \param month Month of the year, from 1 to 12
         * \param day Day of the month, starting at 1
         *
         * \return Days since the 1st of january 1970, negative for an earlier date
         */
        Int64 DaysFromCivil(Int64 year, Uint32 month, Uint32 day)
        {
            /// January and february belong to the previous march based year
            year -= month <= 2;

            const Int64 era = (year >= 0 ? year : year - 399) / 400;
            const Int64 yearOfEra = year - era * 400; // [0, 399]
            const Int64 monthIndex = static_cast<Int64>(month) + (month > 2 ? -3 : 9); // [0, 11]
            const Int64 dayOfYear = (153 * monthIndex + 2) / 5 + static_cast<Int64>(day) - 1;
            const Int64 dayOfEra = yearOfEra * 365 + yearOfEra / 4 - yearOfEra / 100 + dayOfYear;

            return era * DaysPerEra + dayOfEra - DaysFromZeroToEpoch;
        }

        /**
         * \brief Returns the calendar date a number of days from the epoch falls on
         *
         * The exact inverse of DaysFromCivil.
         *
         * \param days Days since the 1st of january 1970
         *
         * \return The matching date
         */
        CivilDate CivilFromDays(Int64 days)
        {
            days += DaysFromZeroToEpoch;

            const Int64 era = (days >= 0 ? days : days - (DaysPerEra - 1)) / DaysPerEra;
            const Int64 dayOfEra = days - era * DaysPerEra; // [0, 146096]
            const Int64 yearOfEra = (dayOfEra - dayOfEra / 1460 + dayOfEra / 36524 - dayOfEra / 146096) / 365; // [0, 399]
            const Int64 dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100); // [0, 365]
            const Int64 monthIndex = (5 * dayOfYear + 2) / 153; // [0, 11]

            const Int64 day = dayOfYear - (153 * monthIndex + 2) / 5 + 1; // [1, 31]
            const Int64 month = monthIndex + (monthIndex < 10 ? 3 : -9); // [1, 12]
            const Int64 year = yearOfEra + era * 400 + (month <= 2);

            assert(year >= 0);

            return { static_cast<Uint64>(year), static_cast<Uint32>(month), static_cast<Uint32>(day) };
        }
    }

    SystemTime SystemTime::Now()
    {
        return FromInstant(Instant::Now());
    }

    SystemTime SystemTime::FromInstant(const Instant& instant)
    {
        const Uint64 seconds = instant.GetSeconds();
        const Uint64 secondOfDay = seconds % SecondsPerDay;

        const CivilDate date = CivilFromDays(static_cast<Int64>(seconds / SecondsPerDay));

        return { date.Year,
                 date.Month,
                 date.Day,
                 static_cast<Uint32>(secondOfDay / SecondsPerHour),
                 static_cast<Uint32>(secondOfDay % SecondsPerHour / SecondsPerMinute),
                 static_cast<Uint32>(secondOfDay % SecondsPerMinute),
                 instant.GetNanoseconds() };
    }

    SystemTime SystemTime::Utc(Uint64 year, Uint32 month, Uint32 day, Uint32 hour, Uint32 minute, Uint32 second, Uint32 nanosecond)
    {
        return { year, month, day, hour, minute, second, nanosecond };
    }

    SystemTime::SystemTime() :
        SystemTime(1970, 1, 1, 0, 0, 0, 0)
    {
        /// Nothing
    }

    Instant SystemTime::ToInstant() const
    {
        const Int64 days = DaysFromCivil(static_cast<Int64>(mYear), mMonth, mDay);

        /// An Instant counts from the epoch and is unsigned, so it cannot describe
        /// anything earlier than it
        assert(days >= 0);

        const Uint64 seconds = static_cast<Uint64>(days) * SecondsPerDay + mHour * SecondsPerHour + mMinute * SecondsPerMinute + mSecond;

        return Instant::EpochSeconds(seconds, mNanosecond);
    }

    Uint64 SystemTime::GetYear() const
    {
        return mYear;
    }

    Uint32 SystemTime::GetMonth() const
    {
        return mMonth;
    }

    Uint32 SystemTime::GetDay() const
    {
        return mDay;
    }

    Uint32 SystemTime::GetHour() const
    {
        return mHour;
    }

    Uint32 SystemTime::GetMinute() const
    {
        return mMinute;
    }

    Uint32 SystemTime::GetSecond() const
    {
        return mSecond;
    }

    Uint32 SystemTime::GetNanosecond() const
    {
        return mNanosecond;
    }

    SystemTime::SystemTime(Uint64 year, Uint32 month, Uint32 day, Uint32 hour, Uint32 minute, Uint32 second, Uint32 nanosecond) :
        mYear(year),
        mMonth(month),
        mDay(day),
        mHour(hour),
        mMinute(minute),
        mSecond(second),
        mNanosecond(nanosecond)
    {
        assert(month >= 1 && month <= 12);
        assert(day >= 1);
        assert(hour < 24);
        assert(minute < 60);
        assert(second < 60);
        assert(nanosecond < 1'000'000'000);
    }
}
