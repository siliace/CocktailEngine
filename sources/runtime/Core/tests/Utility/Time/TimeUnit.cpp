#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Time/TimeUnit.hpp>

TEST_CASE("Convert amout of time between units", "[TimeUnit]")
{
    SECTION("Convert from another time unit")
    {
        REQUIRE(Ck::TimeUnit::Seconds().ConvertFrom(1, Ck::TimeUnit::Minutes()) == 60);
    }

    SECTION("Convert to another time unit")
    {
        REQUIRE(Ck::TimeUnit::Minutes().ConvertTo(1, Ck::TimeUnit::Seconds()) == 60);
    }
}

TEST_CASE("Compare two time unit multiplicity", "[TimeUnit]")
{
    SECTION("Compare two multiples time units")
    {
        Ck::TimeUnit frame = Ck::TimeUnit::Milliseconds(16); // 16 ms per frame
        Ck::TimeUnit tick250 = Ck::TimeUnit::Milliseconds(250); // 250 ms tick
        Ck::TimeUnit second = Ck::TimeUnit::Seconds();

        REQUIRE(frame.IsMultipleOf(Ck::TimeUnit::Milliseconds()));
        REQUIRE(second.IsMultipleOf(tick250));
    }

    SECTION("Compare two time units not multiples")
    {
        Ck::TimeUnit frame = Ck::TimeUnit::Milliseconds(16); // 16 ms per frame
        Ck::TimeUnit tick250 = Ck::TimeUnit::Milliseconds(250); // 250 ms tick
        Ck::TimeUnit second = Ck::TimeUnit::Seconds();

        REQUIRE_FALSE(tick250.IsMultipleOf(frame));
        REQUIRE_FALSE(frame.IsMultipleOf(second));
    }

    SECTION("Compare two time units equivalents")
    {
        Ck::TimeUnit hour = Ck::TimeUnit::Hours();
        Ck::TimeUnit min = Ck::TimeUnit::Minutes();

        REQUIRE(hour.IsMultipleOf(min));
        REQUIRE_FALSE(min.IsMultipleOf(hour));
    }
}

TEST_CASE("Compare two time units", "[TimeUnit]")
{
    SECTION("Compare two time units")
    {
        REQUIRE(Ck::TimeUnit::Seconds().IsBigger(Ck::TimeUnit::Milliseconds()));
        REQUIRE(Ck::TimeUnit::Milliseconds().IsSmaller(Ck::TimeUnit::Seconds()));
    }

    SECTION("Compare two time units equivalents")
    {
        REQUIRE(Ck::TimeUnit::Days() == Ck::TimeUnit::Hours(24));
        REQUIRE(Ck::TimeUnit::Seconds(3600) == Ck::TimeUnit::Hours());
        REQUIRE(Ck::TimeUnit::Seconds(60) == Ck::TimeUnit::Minutes());
    }
}

TEST_CASE("Time unit ordering compares quanta", "[TimeUnit]")
{
    SECTION("A unit holding both a larger numerator and denominator is ordered on its quantum")
    {
        // 5000 nanoseconds is 5 microseconds, so it is the larger quantum even
        // though its denominator is the larger one too
        const Ck::TimeUnit fiveMicroseconds = Ck::TimeUnit::Nanoseconds(5'000);
        const Ck::TimeUnit oneMicrosecond = Ck::TimeUnit::Microseconds();

        REQUIRE_FALSE(fiveMicroseconds.IsSmaller(oneMicrosecond));
        REQUIRE(fiveMicroseconds.IsBigger(oneMicrosecond));

        REQUIRE(oneMicrosecond.IsSmaller(fiveMicroseconds));
        REQUIRE_FALSE(oneMicrosecond.IsBigger(fiveMicroseconds));
    }

    SECTION("Ordering is antisymmetric")
    {
        const Ck::TimeUnit lhs = Ck::TimeUnit::Nanoseconds(5'000);
        const Ck::TimeUnit rhs = Ck::TimeUnit::Microseconds();

        const bool bothSmaller = lhs.IsSmaller(rhs) && rhs.IsSmaller(lhs);
        const bool bothBigger = lhs.IsBigger(rhs) && rhs.IsBigger(lhs);

        REQUIRE_FALSE(bothSmaller);
        REQUIRE_FALSE(bothBigger);
    }

    SECTION("Equivalent units are neither smaller nor bigger")
    {
        const Ck::TimeUnit lhs = Ck::TimeUnit::Milliseconds(1'000);
        const Ck::TimeUnit rhs = Ck::TimeUnit::Seconds();

        REQUIRE_FALSE(lhs.IsSmaller(rhs));
        REQUIRE_FALSE(lhs.IsBigger(rhs));
        REQUIRE_FALSE(rhs.IsSmaller(lhs));
        REQUIRE_FALSE(rhs.IsBigger(lhs));
    }

    SECTION("An arbitrary ratio is ordered on its quantum")
    {
        // 5/3 of a second against half a second
        const Ck::TimeUnit lhs(5, 3);
        const Ck::TimeUnit rhs(1, 2);

        REQUIRE(rhs.IsSmaller(lhs));
        REQUIRE_FALSE(lhs.IsSmaller(rhs));
    }
}

TEST_CASE("Equivalent time units written differently are equal", "[TimeUnit]")
{
    SECTION("A non reduced ratio equals its reduced form")
    {
        REQUIRE(Ck::TimeUnit::Milliseconds(1'000) == Ck::TimeUnit::Seconds());
        REQUIRE(Ck::TimeUnit::Nanoseconds(1'000'000'000) == Ck::TimeUnit::Seconds());
        REQUIRE(Ck::TimeUnit::Microseconds(60'000'000) == Ck::TimeUnit::Minutes());
    }

    SECTION("Distinct quanta stay different")
    {
        REQUIRE(Ck::TimeUnit::Milliseconds(1'000) != Ck::TimeUnit::Milliseconds(1'500));
    }

    SECTION("Conversions are unaffected by the way a unit was written")
    {
        REQUIRE(Ck::TimeUnit::Milliseconds(1'000).ConvertTo(3, Ck::TimeUnit::Milliseconds()) == 3'000);
        REQUIRE(Ck::TimeUnit::Nanoseconds(1'000'000'000).ConvertTo(3, Ck::TimeUnit::Seconds()) == 3);
    }
}
