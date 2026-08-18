#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Time/Duration.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

TEST_CASE("An instant keeps its nanoseconds component normalized", "[Instant]")
{
    SECTION("A nanoseconds component larger than a second is carried over")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 1'500'000'000);

        REQUIRE(instant.GetSeconds() == 11);
        REQUIRE(instant.GetNanoseconds() == 500'000'000);
    }

    SECTION("An offset crossing a second boundary is carried over")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 700'000'000).After(Ck::Duration::Milliseconds(500));

        REQUIRE(instant.GetSeconds() == 11);
        REQUIRE(instant.GetNanoseconds() == 200'000'000);
    }

    SECTION("An offset larger than the nanoseconds component range is carried over")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 0).After(Ck::Duration::Milliseconds(5'000));

        REQUIRE(instant.GetSeconds() == 15);
        REQUIRE(instant.GetNanoseconds() == 0);
    }

    SECTION("The current instant is normalized")
    {
        REQUIRE(Ck::Instant::Now().GetNanoseconds() < 1'000'000'000);
    }
}

TEST_CASE("Offsetting an instant backward borrows from the seconds component", "[Instant]")
{
    SECTION("Subtracting more nanoseconds than the component holds")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 200'000'000).Before(Ck::Duration::Milliseconds(500));

        REQUIRE(instant.GetSeconds() == 9);
        REQUIRE(instant.GetNanoseconds() == 700'000'000);
    }

    SECTION("Subtracting whole seconds")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 200'000'000).Before(Ck::Duration::Seconds(4));

        REQUIRE(instant.GetSeconds() == 6);
        REQUIRE(instant.GetNanoseconds() == 200'000'000);
    }

    SECTION("An offset spanning several seconds")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 200'000'000).Before(Ck::Duration::Milliseconds(2'500));

        REQUIRE(instant.GetSeconds() == 7);
        REQUIRE(instant.GetNanoseconds() == 700'000'000);
    }
}

TEST_CASE("Instants are ordered consistently", "[Instant]")
{
    SECTION("Across a second boundary")
    {
        const Ck::Instant earlier = Ck::Instant::EpochSeconds(10, 900'000'000);
        const Ck::Instant later = Ck::Instant::EpochSeconds(11, 100'000'000);

        REQUIRE(later.IsAfter(earlier));
        REQUIRE_FALSE(earlier.IsAfter(later));

        REQUIRE(earlier.IsBefore(later));
        REQUIRE_FALSE(later.IsBefore(earlier));
    }

    SECTION("Within the same second")
    {
        const Ck::Instant earlier = Ck::Instant::EpochSeconds(10, 100'000'000);
        const Ck::Instant later = Ck::Instant::EpochSeconds(10, 900'000'000);

        REQUIRE(later.IsAfter(earlier));
        REQUIRE_FALSE(earlier.IsAfter(later));

        REQUIRE(earlier.IsBefore(later));
        REQUIRE_FALSE(later.IsBefore(earlier));
    }

    SECTION("Two equal instants are neither before nor after one another")
    {
        const Ck::Instant instant = Ck::Instant::EpochSeconds(10, 500'000'000);
        const Ck::Instant same = Ck::Instant::EpochSeconds(10, 500'000'000);

        REQUIRE_FALSE(instant.IsAfter(same));
        REQUIRE_FALSE(instant.IsBefore(same));
    }
}

TEST_CASE("An instant can be built from milliseconds since the epoch", "[Instant]")
{
    SECTION("The sub-second part is kept")
    {
        const Ck::Instant instant = Ck::Instant::EpochMilliseconds(1'500);

        REQUIRE(instant.GetSeconds() == 1);
        REQUIRE(instant.GetNanoseconds() == 500'000'000);
    }

    SECTION("A realistic epoch timestamp is not degraded")
    {
        const Ck::Instant instant = Ck::Instant::EpochMilliseconds(1'790'000'000'123);

        REQUIRE(instant.GetSeconds() == 1'790'000'000);
        REQUIRE(instant.GetNanoseconds() == 123'000'000);
    }
}
