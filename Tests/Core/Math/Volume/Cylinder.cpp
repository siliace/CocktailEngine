#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Volume/Cylinder.hpp>

TEST_CASE("Create a cylinder from a center, an half extent and a radius", "[Cylinder]")
{
	Ck::Cylinder<float> cylinder(1.f, 1.f, Ck::Vector3<float>::Zero());

	SECTION("Its radius should be 1")
	{
		REQUIRE(Ck::NearlyEqual(cylinder.GetRadius(), 1.f));
	}

	SECTION("Its half extent should be 1")
	{
		REQUIRE(Ck::NearlyEqual(cylinder.GetHalfExtent(), 1.f));
	}


	SECTION("Its center should be [0, 0, 0]")
	{
		REQUIRE(cylinder.GetCenter() == Ck::Vector3<float>::Zero());
	}
}

TEST_CASE("Extend a cylinder to encompass a point", "[Cylinder]")
{
	Ck::Cylinder<float> cylinder(1.f, 1.f, Ck::Vector3<float>::Zero());

	cylinder.Extend(Ck::Vector3<float>(3.f, 3.f, 0.f));

	SECTION("Its radius changed")
	{
		REQUIRE(Ck::NearlyEqual(cylinder.GetRadius(), 3.f));
	}

	SECTION("Its half extent changed")
	{
		REQUIRE(Ck::NearlyEqual(cylinder.GetHalfExtent(), 3.f));
	}
}