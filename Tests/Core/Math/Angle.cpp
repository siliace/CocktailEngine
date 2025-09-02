#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Angle.hpp>

TEMPLATE_TEST_CASE("Angle conversion between degrees and radians", "[Angle]", float, double)
{
	TestType degrees = 180;
	TestType radians = Ck::Constants<TestType>::Pi;

	SECTION("Radian to degree")
	{
		Ck::Angle<TestType> angle = Ck::Angle<TestType>::Degree(degrees);
		REQUIRE(Ck::NearlyEqual(angle.AsRadian(), radians));
	}

	SECTION("Degree to radian")
	{
		Ck::Angle<TestType> angle = Ck::Angle<TestType>::Radian(radians);
		REQUIRE(Ck::NearlyEqual(angle.AsDegree(), degrees));
	}
}

TEMPLATE_TEST_CASE("Angle equality", "[Angle]", float, double)
{
	SECTION("Equality")
	{
		REQUIRE(Ck::Angle<TestType>::Degree(180) == Ck::Angle<TestType>::Degree(180));
	}

	SECTION("Inequality")
	{
		REQUIRE(Ck::Angle<TestType>::Degree(180) != Ck::Angle<TestType>::Degree(360));
	}
}

TEMPLATE_TEST_CASE("Angle addition", "[Angle]", float, double)
{
	SECTION("With the same unit")
	{
		Ck::Angle<TestType> lhs = Ck::Angle<TestType>::Degree(180);
		Ck::Angle<TestType> rhs = Ck::Angle<TestType>::Degree(180);

		Ck::Angle<TestType> sum = lhs + rhs;
		REQUIRE(Ck::NearlyEqual(sum.AsDegree(), static_cast<TestType>(360)));
	}

	SECTION("Not with the same unit")
	{
		Ck::Angle<TestType> lhs = Ck::Angle<TestType>::Radian(Ck::Constants<TestType>::Pi);
		Ck::Angle<TestType> rhs = Ck::Angle<TestType>::Degree(180);

		Ck::Angle<TestType> sum = lhs + rhs;
		REQUIRE(Ck::NearlyEqual(sum.AsDegree(), static_cast<TestType>(360)));
	}
}

TEMPLATE_TEST_CASE("Angle normalization", "[Angle]", float, double)
{
	Ck::Angle<TestType> angle = Ck::Angle<TestType>::Degree(
		3 * 180
	);
	Ck::Angle<TestType> normalized = Ck::Angle<TestType>::Radian(
		Ck::Constants<TestType>::Pi
	);

	REQUIRE(angle.Normalized() == normalized);
}