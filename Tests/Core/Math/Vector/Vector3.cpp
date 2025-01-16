#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Vector/Vector3.hpp>

TEST_CASE("Normalize a vector", "[Vector3]")
{
	Ck::Vector3<float> vector(2.f, 0.f, 0.f);
	Ck::Vector3<float> normalized = Ck::Vector3<float>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(normalized.GetLength() == 1.f);
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(normalized.X() == 1.f);
		REQUIRE(normalized.Y() == 0.f);
		REQUIRE(normalized.Z() == 0.f);
	}
}

TEST_CASE("Compute the dot product of two vectors", "[Vector3]")
{
	Ck::Vector3<float> lhs(1.f, 0.f, 1.f);
	Ck::Vector3<float> rhs(0.f, 1.f, 1.f);

	REQUIRE(Ck::Vector3<float>::DotProduct(lhs, rhs) == 1.f);
}

TEST_CASE("Compute the cross product of two vectors", "[Vector3]")
{
	Ck::Vector3<float> lhs = Ck::Vector3<float>::Right();
	Ck::Vector3<float> rhs = Ck::Vector3<float>::Forward();

	REQUIRE(Ck::Vector3<float>::CrossProduct(lhs, rhs) == Ck::Vector3<float>::Up());
}

TEST_CASE("Create a default Vector3", "[Vector3]")
{
	Ck::Vector3<float> vector;

	REQUIRE(vector.X() == 0.f);
	REQUIRE(vector.Y() == 0.f);
	REQUIRE(vector.Z() == 0.f);
}

TEST_CASE("Create a Vector3 by specifying its components", "[Vector3]")
{
	Ck::Vector3<float> vector(2.f, -7.f, 0.5f);

	REQUIRE(vector.X() == 2.f);
	REQUIRE(vector.Y() == -7.f);
	REQUIRE(vector.Z() == 0.5f);
}

TEST_CASE("The length of an unit vector is 1", "[Vector3]")
{
	Ck::Vector3<float> vector(1.f, 0.f, 0.f);

	SECTION("Its length should be 1")
	{
		REQUIRE(vector.GetLength() == 1.f);
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector3<float> normalized = Ck::Vector3<float>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}