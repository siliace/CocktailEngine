#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Angle.hpp>

TEST_CASE("Angle conversion between degrees and radians", "[Angle]")
{
	float degrees = 180.f;
	float radians = Ck::Constants<float>::Pi;

	SECTION("Radian to degree")
	{
		Ck::Angle<float> angle = Ck::Angle<float>::Degree(degrees);
		REQUIRE(Ck::NearlyEqual(angle.AsRadian(), radians));
	}

	SECTION("Degree to radian")
	{
		Ck::Angle<float> angle = Ck::Angle<float>::Radian(radians);
		REQUIRE(Ck::NearlyEqual(angle.AsDegree(), degrees));
	}
}

TEST_CASE("Angle equality", "[Angle]")
{
	SECTION("Equality")
	{
		REQUIRE(Ck::Angle<float>::Degree(180.f) == Ck::Angle<float>::Degree(180.f));
	}

	SECTION("Inequality")
	{
		REQUIRE(Ck::Angle<float>::Degree(180.f) != Ck::Angle<float>::Degree(360.f));
	}
}

TEST_CASE("Angle addition", "[Angle]")
{
	SECTION("With the same unit")
	{
		Ck::Angle<float> lhs = Ck::Angle<float>::Degree(180.f);
		Ck::Angle<float> rhs = Ck::Angle<float>::Degree(180.f);

		Ck::Angle<float> sum = lhs + rhs;
		REQUIRE(Ck::NearlyEqual(sum.AsDegree(), 360.f));
	}

	SECTION("Not with the same unit")
	{
		Ck::Angle<float> lhs = Ck::Angle<float>::Radian(Ck::Constants<float>::Pi);
		Ck::Angle<float> rhs = Ck::Angle<float>::Degree(180.f);

		Ck::Angle<float> sum = lhs + rhs;
		REQUIRE(Ck::NearlyEqual(sum.AsDegree(), 360.f));
	}
}

TEST_CASE("Angle normalization", "[Angle]")
{
	Ck::Angle<float> angle = Ck::Angle<float>::Degree(
		3 * 180.f
	);
	Ck::Angle<float> normalized = Ck::Angle<float>::Radian(
		Ck::Constants<float>::Pi
	);

	REQUIRE(angle.Normalized() == normalized);
}