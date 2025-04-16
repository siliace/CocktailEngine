#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Volume/Box.hpp>
#include <Cocktail/Core/Math/Volume/Sphere.hpp>

TEST_CASE("Create a sphere from a point and a radius", "[Sphere]")
{
	Ck::Sphere<float> sphere(1.f, Ck::Vector3<float>::Zero());

	SECTION("Its radius should be 1")
	{
		REQUIRE(Ck::NearlyEqual(sphere.GetRadius(), 1.f));
	}

	SECTION("Its center should be [0, 0, 0]")
	{
		REQUIRE(sphere.GetCenter() == Ck::Vector3<float>::Zero());
	}
}

TEST_CASE("Create a sphere from two points", "[Sphere]")
{
	Ck::Sphere<float> sphere(
		Ck::Vector3<float>(-1.f, 0.f, 0.f), Ck::Vector3<float>(1.f, 0.f, 0.f)
	);

	SECTION("Its radius should be 2")
	{
		REQUIRE(Ck::NearlyEqual(sphere.GetRadius(), 1.f));
	}

	SECTION("Its center should be [0, 0, 0]")
	{
		REQUIRE(sphere.GetCenter() == Ck::Vector3<float>::Zero());
	}
}

TEST_CASE("Extend a sphere to encompass a point", "[Sphere]")
{
	Ck::Sphere<float> sphere(1.f, Ck::Vector3<float>::Zero());

	sphere.Extend(Ck::Vector3<float>(3.f, 0.f, 0.f));

	SECTION("Its radius changed")
	{
		REQUIRE(Ck::NearlyEqual(sphere.GetRadius(), 3.f));
	}
}

TEST_CASE("Test if a point is inside a sphere", "[Sphere]")
{
	Ck::Sphere<float> sphere(1.f, Ck::Vector3<float>::Zero());

	SECTION("with inside point")
	{
		Ck::Vector3<float> point(0.5f);
		REQUIRE(sphere.Contains(point));
	}

	SECTION("with outside point")
	{
		Ck::Vector3<float> point(1.5f);
		REQUIRE_FALSE(sphere.Contains(point));
	}
}

TEST_CASE("Test intersection between two spheres", "[Sphere]")
{
	Ck::Sphere<float> sphere(1.f, Ck::Vector3<float>::Zero());

	SECTION("With an inside sphere")
	{
		Ck::Sphere<float> other(0.5f, Ck::Vector3<float>::Zero());
		REQUIRE(sphere.Intersect(other) == Ck::Intersection::Inside);
	}

	SECTION("With an intersecting sphere")
	{
		Ck::Sphere<float> other(1.f, Ck::Vector3<float>(1.5f, 0.f, 0.f));
		REQUIRE(sphere.Intersect(other) == Ck::Intersection::Intersect);
	}

	SECTION("With an outside sphere")
	{
		Ck::Sphere<float> other(1.f, Ck::Vector3<float>(3.f, 0.f, 0.f));
		REQUIRE(sphere.Intersect(other) == Ck::Intersection::Outside);
	}
}

TEST_CASE("Test intersection between a sphere and a box", "[Sphere]")
{
	Ck::Sphere<float> sphere(1.f, Ck::Vector3<float>::Zero());

	SECTION("With an inside box")
	{
		Ck::Box<float> box(Ck::Vector3<float>(-0.5f), Ck::Vector3<float>(0.5f));
		REQUIRE(sphere.Intersect(box) == Ck::Intersection::Inside);
	}

	SECTION("With an intersecting box")
	{
		Ck::Box<float> box(Ck::Vector3<float>(0.f), Ck::Vector3<float>(2.f));
		REQUIRE(sphere.Intersect(box) == Ck::Intersection::Intersect);
	}

	SECTION("With an outside box")
	{
		Ck::Box<float> box(Ck::Vector3<float>(2.f), Ck::Vector3<float>(4.f));
		REQUIRE(sphere.Intersect(box) == Ck::Intersection::Outside);
	}
}