#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Volume/Box.hpp>

TEST_CASE("Create a box from two points", "[Box]")
{
	Ck::Box<float> box(
		Ck::Vector3<float>::Unit(), -Ck::Vector3<float>::Unit()
	);

	SECTION("Its extents should be 2")
	{
		REQUIRE(box.GetExtents() == Ck::MakeExtent(2.f, 2.f, 2.f));
	}

	SECTION("Its center should be [0, 0, 0]")
	{
		REQUIRE(box.GetCenter() == Ck::Vector3<float>::Zero());
	}

	SECTION("Its diagonale is [2, 2, 2]")
	{
		REQUIRE(box.GetDiagonale() == Ck::Vector3<float>(2.f));
	}
}

TEST_CASE("Extend a box to encompass a point", "[Box]")
{
	Ck::Box<float> box(
		Ck::Vector3<float>::Unit(), -Ck::Vector3<float>::Unit()
	);

	box.Extend(Ck::Vector3<float>(3.f, 0.f, 0.f));

	SECTION("Its extents changed")
	{
		REQUIRE(box.GetExtents() == Ck::MakeExtent(4.f, 2.f, 2.f));
	}

	SECTION("Its center changed")
	{
		REQUIRE(box.GetCenter() == Ck::Vector3<float>(1.f, 0.f, 0.f));
	}

	SECTION("Its diagonale changed")
	{
		REQUIRE(box.GetDiagonale() == Ck::Vector3<float>(4.f, 2.f, 2.f));
	}
}