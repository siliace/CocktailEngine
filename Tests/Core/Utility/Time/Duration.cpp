#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include "Cocktail/Core/Utility/Time/Instant.hpp"

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
		Ck::Duration difference = Ck::Duration::Between(
			Ck::Duration::Seconds(5), Ck::Duration::Seconds(8)
		);

		REQUIRE(difference.GetUnit() == Ck::TimeUnit::Seconds());
		REQUIRE(difference == Ck::Duration::Seconds(3));
	}

	SECTION("With differents time units")
	{
		Ck::Duration difference = Ck::Duration::Between(
			Ck::Duration::Milliseconds(100), Ck::Duration::Seconds(3)
		);

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

