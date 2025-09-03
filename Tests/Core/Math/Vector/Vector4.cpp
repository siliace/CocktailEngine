#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Vector/Vector4.hpp>

TEMPLATE_TEST_CASE("Normalize a vector", "[Vector4]", float, double)
{
	Ck::Vector4<TestType> vector(2, 0, 0, 0);
	Ck::Vector4<TestType> normalized = Ck::Vector4<TestType>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(Ck::NearlyEqual(normalized.GetLength(), 1));
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(Ck::NearlyEqual(normalized.X(), 1));
		REQUIRE(Ck::NearlyEqual(normalized.Y(), 0));
		REQUIRE(Ck::NearlyEqual(normalized.Z(), 0));
		REQUIRE(Ck::NearlyEqual(normalized.Z(), 0));
	}
}

TEMPLATE_TEST_CASE("Compute the dot product of two vectors", "[Vector4]", float, double)
{
	Ck::Vector4<TestType> lhs(1, 0, 1, 0);
	Ck::Vector4<TestType> rhs(0, 1, 1, 0);

	REQUIRE(Ck::NearlyEqual(Ck::Vector4<TestType>::DotProduct(lhs, rhs), 1));
}

TEMPLATE_TEST_CASE("Create a default Vector4", "[Vector4]", float, double)
{
	Ck::Vector4<TestType> vector;

	REQUIRE(Ck::NearlyEqual(vector.X(), 0));
	REQUIRE(Ck::NearlyEqual(vector.Y(), 0));
	REQUIRE(Ck::NearlyEqual(vector.Z(), 0));
	REQUIRE(Ck::NearlyEqual(vector.W(), 0));
}

TEMPLATE_TEST_CASE("Create a Vector4 by specifying its components", "[Vector4]", float, double)
{
	Ck::Vector4<TestType> vector(2, -7, 0.5f, 0);

	REQUIRE(Ck::NearlyEqual(vector.X(), 2));
	REQUIRE(Ck::NearlyEqual(vector.Y(), -7));
	REQUIRE(Ck::NearlyEqual(vector.Z(), 0.5));
	REQUIRE(Ck::NearlyEqual(vector.W(), 0));
}

TEMPLATE_TEST_CASE("The length of an unit vector is 1", "[Vector4]", float, double)
{
	Ck::Vector4<TestType> vector(1, 0, 0, 0);

	SECTION("Its length should be 1")
	{
		REQUIRE(Ck::NearlyEqual(vector.GetLength(), 1));
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector4<TestType> normalized = Ck::Vector4<TestType>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}