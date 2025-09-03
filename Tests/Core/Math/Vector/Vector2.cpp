#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>

TEMPLATE_TEST_CASE("Normalize a vector", "[Vector2]", float, double)
{
	Ck::Vector2<TestType> vector(2, 0);
	Ck::Vector2<TestType> normalized = Ck::Vector2<TestType>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(Ck::NearlyEqual(normalized.GetLength(), 1));
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(Ck::NearlyEqual(normalized.X(), 1));
		REQUIRE(Ck::NearlyEqual(normalized.Y(), 0));
	}
}

TEMPLATE_TEST_CASE("Compute the dot product of two vectors", "[Vector2]", float, double)
{
	Ck::Vector2<TestType> lhs(1, 1);
	Ck::Vector2<TestType> rhs(0, 1);

	REQUIRE(Ck::NearlyEqual(Ck::Vector2<TestType>::DotProduct(lhs, rhs), 1));
}

TEMPLATE_TEST_CASE("Create a default Vector2", "[Vector2]", float, double)
{
	Ck::Vector2<TestType> vector;

	REQUIRE(Ck::NearlyEqual(vector.X(), 0));
	REQUIRE(Ck::NearlyEqual(vector.Y(), 0));
}

TEMPLATE_TEST_CASE("Create a Vector2 by specifying its components", "[Vector2]", float, double)
{
	Ck::Vector2<TestType> vector(2.5f, -7);

	REQUIRE(Ck::NearlyEqual(vector.X(), 2.5f));
	REQUIRE(Ck::NearlyEqual(vector.Y(), -7));
}

TEMPLATE_TEST_CASE("The length of an unit vector is 1", "[Vector2]", float, double)
{
	Ck::Vector2<TestType> vector(1, 0);

	SECTION("Its length should be 1")
	{
		REQUIRE(Ck::NearlyEqual(vector.GetLength(), 1));
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector2<TestType> normalized = Ck::Vector2<TestType>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}