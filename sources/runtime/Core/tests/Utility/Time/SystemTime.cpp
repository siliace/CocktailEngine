#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Time/Duration.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>
#include <CocktailEngine/Core/Utility/Time/SystemTime.hpp>

namespace
{
    /**
     * \brief Returns the last day of february of a given year
     *
     * Walks back a day from the 1st of march, which is the definition of the end of
     * february whether the year is a leap one or not.
     */
    Ck::Uint32 LastDayOfFebruary(Ck::Uint64 year)
    {
        const Ck::Instant firstOfMarch = Ck::SystemTime::Utc(year, 3, 1).ToInstant();

        return Ck::SystemTime::FromInstant(firstOfMarch.Before(Ck::Duration::Days(1))).GetDay();
    }
}

TEST_CASE("A system time breaks an instant down into calendar fields", "[SystemTime]")
{
    SECTION("The epoch itself")
    {
        const Ck::SystemTime epoch = Ck::SystemTime::FromInstant(Ck::Instant());

        REQUIRE(epoch.GetYear() == 1970);
        REQUIRE(epoch.GetMonth() == 1);
        REQUIRE(epoch.GetDay() == 1);
        REQUIRE(epoch.GetHour() == 0);
        REQUIRE(epoch.GetMinute() == 0);
        REQUIRE(epoch.GetSecond() == 0);
        REQUIRE(epoch.GetNanosecond() == 0);
    }

    SECTION("A default constructed system time is the epoch too")
    {
        REQUIRE(Ck::SystemTime().ToInstant().GetSeconds() == 0);
    }

    SECTION("A known timestamp")
    {
        // 1790000000 is 2026-09-21T14:13:20Z
        const Ck::SystemTime stamp = Ck::SystemTime::FromInstant(Ck::Instant::EpochSeconds(1'790'000'000, 123'456'789));

        REQUIRE(stamp.GetYear() == 2026);
        REQUIRE(stamp.GetMonth() == 9);
        REQUIRE(stamp.GetDay() == 21);
        REQUIRE(stamp.GetHour() == 14);
        REQUIRE(stamp.GetMinute() == 13);
        REQUIRE(stamp.GetSecond() == 20);
        REQUIRE(stamp.GetNanosecond() == 123'456'789);
    }

    SECTION("The last representable second of year 9999")
    {
        const Ck::SystemTime stamp = Ck::SystemTime::FromInstant(Ck::Instant::EpochSeconds(253'402'300'799, 0));

        REQUIRE(stamp.GetYear() == 9999);
        REQUIRE(stamp.GetMonth() == 12);
        REQUIRE(stamp.GetDay() == 31);
        REQUIRE(stamp.GetHour() == 23);
        REQUIRE(stamp.GetMinute() == 59);
        REQUIRE(stamp.GetSecond() == 59);
    }

    SECTION("The last second before a day rolls over")
    {
        const Ck::SystemTime stamp = Ck::SystemTime::FromInstant(Ck::Instant::EpochSeconds(86'399, 0));

        REQUIRE(stamp.GetYear() == 1970);
        REQUIRE(stamp.GetMonth() == 1);
        REQUIRE(stamp.GetDay() == 1);
        REQUIRE(stamp.GetHour() == 23);
        REQUIRE(stamp.GetMinute() == 59);
        REQUIRE(stamp.GetSecond() == 59);
    }

    SECTION("The first second of the next day")
    {
        const Ck::SystemTime stamp = Ck::SystemTime::FromInstant(Ck::Instant::EpochSeconds(86'400, 0));

        REQUIRE(stamp.GetDay() == 2);
        REQUIRE(stamp.GetHour() == 0);
        REQUIRE(stamp.GetMinute() == 0);
        REQUIRE(stamp.GetSecond() == 0);
    }
}

TEST_CASE("A system time converts back to the instant it came from", "[SystemTime]")
{
    SECTION("Across a spread of timestamps")
    {
        const Ck::Uint64 timestamps[] = { 0, 1, 86'399, 86'400, 951'782'400, 1'709'164'800, 1'790'000'000, 4'107'542'400, 253'402'300'799 };

        for (Ck::Uint64 seconds : timestamps)
        {
            const Ck::Instant instant = Ck::Instant::EpochSeconds(seconds, 987'654'321);
            const Ck::Instant roundTrip = Ck::SystemTime::FromInstant(instant).ToInstant();

            REQUIRE(roundTrip.GetSeconds() == seconds);
            REQUIRE(roundTrip.GetNanoseconds() == 987'654'321);
        }
    }

    SECTION("Every second across a day boundary")
    {
        const Ck::Uint64 base = 1'790'000'000 / 86'400 * 86'400;

        for (Ck::Uint64 offset = 0; offset < 120; ++offset)
        {
            const Ck::Uint64 seconds = base - 60 + offset;
            const Ck::Instant instant = Ck::Instant::EpochSeconds(seconds, 0);

            REQUIRE(Ck::SystemTime::FromInstant(instant).ToInstant().GetSeconds() == seconds);
        }
    }

    SECTION("Explicit fields survive the conversion")
    {
        const Ck::SystemTime stamp = Ck::SystemTime::Utc(2026, 10, 2, 14, 35, 7, 250'000'000);
        const Ck::SystemTime roundTrip = Ck::SystemTime::FromInstant(stamp.ToInstant());

        REQUIRE(roundTrip.GetYear() == 2026);
        REQUIRE(roundTrip.GetMonth() == 10);
        REQUIRE(roundTrip.GetDay() == 2);
        REQUIRE(roundTrip.GetHour() == 14);
        REQUIRE(roundTrip.GetMinute() == 35);
        REQUIRE(roundTrip.GetSecond() == 7);
        REQUIRE(roundTrip.GetNanosecond() == 250'000'000);
    }
}

TEST_CASE("A system time follows the Gregorian leap year rules", "[SystemTime]")
{
    SECTION("A year divisible by four is a leap year")
    {
        REQUIRE(LastDayOfFebruary(2024) == 29);
    }

    SECTION("A year not divisible by four is not")
    {
        REQUIRE(LastDayOfFebruary(2023) == 28);
    }

    SECTION("A century divisible by four hundred is a leap year")
    {
        REQUIRE(LastDayOfFebruary(2000) == 29);
        REQUIRE(LastDayOfFebruary(2400) == 29);
    }

    SECTION("A century not divisible by four hundred is not")
    {
        REQUIRE(LastDayOfFebruary(2100) == 28);
        REQUIRE(LastDayOfFebruary(2200) == 28);
        REQUIRE(LastDayOfFebruary(2300) == 28);
    }

    SECTION("The leap day itself is reachable")
    {
        const Ck::SystemTime leapDay = Ck::SystemTime::FromInstant(Ck::SystemTime::Utc(2024, 2, 29).ToInstant());

        REQUIRE(leapDay.GetMonth() == 2);
        REQUIRE(leapDay.GetDay() == 29);
    }
}

TEST_CASE("A system time reads the same clock as an instant", "[SystemTime]")
{
    const Ck::Instant before = Ck::Instant::Now();
    const Ck::SystemTime now = Ck::SystemTime::Now();
    const Ck::Instant after = Ck::Instant::Now();

    const Ck::Instant asInstant = now.ToInstant();

    REQUIRE_FALSE(asInstant.IsBefore(before));
    REQUIRE_FALSE(asInstant.IsAfter(after));

    /// Sanity check that the fields are a plausible date rather than a zeroed one
    REQUIRE(now.GetYear() >= 2026);
    REQUIRE(now.GetMonth() >= 1);
    REQUIRE(now.GetMonth() <= 12);
    REQUIRE(now.GetDay() >= 1);
    REQUIRE(now.GetDay() <= 31);
    REQUIRE(now.GetHour() <= 23);
    REQUIRE(now.GetMinute() <= 59);
    REQUIRE(now.GetSecond() <= 59);
    REQUIRE(now.GetNanosecond() < 1'000'000'000);
}
