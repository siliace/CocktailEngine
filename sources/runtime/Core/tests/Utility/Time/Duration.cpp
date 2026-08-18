#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <CocktailEngine/Core/Utility/Time/Instant.hpp>

TEST_CASE("Addition of two durations", "[Duration]")
{
    SECTION("With the same time unit")
    {
        const Ck::Duration lhs = Ck::Duration::Seconds(6);
        const Ck::Duration rhs = Ck::Duration::Seconds(9);
        const Ck::Duration sum = lhs + rhs;

        REQUIRE(sum == Ck::Duration::Seconds(15));
        REQUIRE(sum.GetUnit() == Ck::TimeUnit::Seconds());
    }

    SECTION("With differents time units")
    {
        const Ck::Duration lhs = Ck::Duration::Minutes(6);
        const Ck::Duration rhs = Ck::Duration::Microseconds(156);
        const Ck::Duration sum = lhs + rhs;

        REQUIRE(sum.GetCount() == 6 * 60000000 + 156);
        REQUIRE(sum.GetUnit() == Ck::TimeUnit::Microseconds());
    }
}

TEST_CASE("Convert a duration to another time unit", "[Duration]")
{
    Ck::Duration seconds = Ck::Duration::Seconds(60);

    SECTION("Convert a duration to a smaller time unit")
    {
        Ck::Duration milliseconds = Ck::Duration::Milliseconds(60000);
        REQUIRE(seconds.As(Ck::TimeUnit::Milliseconds()).GetCount() == milliseconds.GetCount());
    }

    SECTION("Convert a duration to a bigger time unit")
    {
        Ck::Duration minutes = Ck::Duration::Minutes(1);
        REQUIRE(seconds.As(Ck::TimeUnit::Minutes()).GetCount() == minutes.GetCount());
    }
}

TEST_CASE("Compute the absolute difference between two durations", "[Duration]")
{
    SECTION("With the same time unit")
    {
        Ck::Duration difference = Ck::Duration::Between(Ck::Duration::Seconds(5), Ck::Duration::Seconds(8));

        REQUIRE(difference.GetUnit() == Ck::TimeUnit::Seconds());
        REQUIRE(difference == Ck::Duration::Seconds(3));
    }

    SECTION("With differents time units")
    {
        Ck::Duration difference = Ck::Duration::Between(Ck::Duration::Milliseconds(100), Ck::Duration::Seconds(3));

        REQUIRE(difference.GetUnit() == Ck::TimeUnit::Milliseconds());
        REQUIRE(difference.GetCount() == 2900);
    }
}

TEST_CASE("Compute the absolute difference between two instants", "[Duration]")
{
    SECTION("With the same second")
    {
        Ck::Instant lhs = Ck::Instant::EpochSeconds(0, 16'000'000);
        Ck::Instant rhs = Ck::Instant::EpochSeconds(0, 32'000'000);

        REQUIRE(Ck::Duration::Between(lhs, rhs) == Ck::Duration::Milliseconds(16));

        std::swap(lhs, rhs);

        REQUIRE(Ck::Duration::Between(lhs, rhs) == Ck::Duration::Milliseconds(16));
    }

    SECTION("With a different second")
    {
        Ck::Instant lhs = Ck::Instant::EpochSeconds(11, 500'000);
        Ck::Instant rhs = Ck::Instant::EpochSeconds(10, 999'500'000);

        REQUIRE(Ck::Duration::Between(lhs, rhs) == Ck::Duration::Milliseconds(1));

        std::swap(lhs, rhs);

        REQUIRE(Ck::Duration::Between(lhs, rhs) == Ck::Duration::Milliseconds(1));
    }
}

TEST_CASE("Durations expressed in differently written units compare correctly", "[Duration]")
{
    // A one second quantum spelled in nanoseconds, which used to be picked as the
    // comparison unit and truncated both operands down to the same count
    const Ck::Duration oneSecond(1, Ck::TimeUnit::Nanoseconds(1'000'000'000));
    const Ck::Duration oneAndAHalfSecond = Ck::Duration::Milliseconds(1'500);

    SECTION("They are not equal")
    {
        REQUIRE(oneSecond.GetCount(Ck::TimeUnit::Milliseconds()) == 1'000);
        REQUIRE(oneAndAHalfSecond.GetCount(Ck::TimeUnit::Milliseconds()) == 1'500);

        REQUIRE(oneSecond != oneAndAHalfSecond);
    }

    SECTION("They are ordered on their actual length")
    {
        REQUIRE(oneSecond < oneAndAHalfSecond);
        REQUIRE(oneAndAHalfSecond > oneSecond);

        REQUIRE_FALSE(oneAndAHalfSecond < oneSecond);
        REQUIRE_FALSE(oneSecond > oneAndAHalfSecond);
    }

    SECTION("Equivalent lengths written differently are equal")
    {
        REQUIRE(oneSecond == Ck::Duration::Milliseconds(1'000));
        REQUIRE(oneSecond == Ck::Duration::Seconds(1));
    }
}

TEST_CASE("An infinite duration stays infinite", "[Duration]")
{
    SECTION("When something is added to it")
    {
        REQUIRE((Ck::Duration::Infinite() + Ck::Duration::Nanoseconds(1)).IsInfinite());
        REQUIRE((Ck::Duration::Infinite() + Ck::Duration::Days(1)).IsInfinite());
        REQUIRE((Ck::Duration::Nanoseconds(1) + Ck::Duration::Infinite()).IsInfinite());
    }

    SECTION("When another duration is taken away from it")
    {
        REQUIRE(Ck::Duration::Between(Ck::Duration::Infinite(), Ck::Duration::Seconds(5)).IsInfinite());
        REQUIRE(Ck::Duration::Between(Ck::Duration::Seconds(5), Ck::Duration::Infinite()).IsInfinite());
    }

    SECTION("And compares greater than any finite duration")
    {
        REQUIRE(Ck::Duration::Infinite() > Ck::Duration::Days(365));
    }
}
