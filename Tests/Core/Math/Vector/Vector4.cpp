#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Vector/Vector4.hpp>

TEST_CASE("Normalize a vector", "[Vector4]")
{
	Ck::Vector4<float> vector(2.f, 0.f, 0.f, 0.f);
	Ck::Vector4<float> normalized = Ck::Vector4<float>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(normalized.GetLength() == 1.f);
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(normalized.X() == 1.f);
		REQUIRE(normalized.Y() == 0.f);
		REQUIRE(normalized.Z() == 0.f);
		REQUIRE(normalized.Z() == 0.f);
	}
}

TEST_CASE("Compute the dot product of two vectors", "[Vector4]")
{
	Ck::Vector4<float> lhs(1.f, 0.f, 1.f, 0.f);
	Ck::Vector4<float> rhs(0.f, 1.f, 1.f, 0.f);

	REQUIRE(Ck::Vector4<float>::DotProduct(lhs, rhs) == 1.f);
}

TEST_CASE("Create a default Vector4", "[Vector4]")
{
	Ck::Vector4<float> vector;

	REQUIRE(vector.X() == 0.f);
	REQUIRE(vector.Y() == 0.f);
	REQUIRE(vector.Z() == 0.f);
}

TEST_CASE("Create a Vector4 by specifying its components", "[Vector4]")
{
	Ck::Vector4<float> vector(2.f, -7.f, 0.5f, 0.f);

	REQUIRE(vector.X() == 2.f);
	REQUIRE(vector.Y() == -7.f);
	REQUIRE(vector.Z() == 0.5f);
	REQUIRE(vector.W() == 0.f);
}

TEST_CASE("The length of an unit vector is 1", "[Vector4]")
{
	Ck::Vector4<float> vector(1.f, 0.f, 0.f, 0.f);

	SECTION("Its length should be 1")
	{
		REQUIRE(vector.GetLength() == 1.f);
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector4<float> normalized = Ck::Vector4<float>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}