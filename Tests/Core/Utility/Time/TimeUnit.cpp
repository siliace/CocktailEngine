#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

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
