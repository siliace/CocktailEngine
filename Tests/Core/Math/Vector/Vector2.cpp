#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Vector/Vector2.hpp>

TEST_CASE("Normalize a vector", "[Vector2]")
{
	Ck::Vector2<float> vector(2.f, 0.f);
	Ck::Vector2<float> normalized = Ck::Vector2<float>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(normalized.GetLength() == 1.f);
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(normalized.X() == 1.f);
		REQUIRE(normalized.Y() == 0.f);
	}
}

TEST_CASE("Compute the dot product of two vectors", "[Vector2]")
{
	Ck::Vector2<float> lhs(1.f, 1.f);
	Ck::Vector2<float> rhs(0.f, 1.f);

	REQUIRE(Ck::Vector2<float>::DotProduct(lhs, rhs) == 1.f);
}

TEST_CASE("Create a default Vector2", "[Vector2]")
{
	Ck::Vector2<float> vector;

	REQUIRE(vector.X() == 0.f);
	REQUIRE(vector.Y() == 0.f);
}

TEST_CASE("Create a Vector2 by specifying its components", "[Vector2]")
{
	Ck::Vector2<float> vector(2.5f, -7.f);

	REQUIRE(vector.X() == 2.5f);
	REQUIRE(vector.Y() == -7.f);
}

TEST_CASE("The length of an unit vector is 1", "[Vector2]")
{
	Ck::Vector2<float> vector(1.f, 0.f);

	SECTION("Its length should be 1")
	{
		REQUIRE(vector.GetLength() == 1.f);
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector2<float> normalized = Ck::Vector2<float>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}