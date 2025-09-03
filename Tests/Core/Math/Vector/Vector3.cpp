#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Math/Vector/Vector3.hpp>

TEMPLATE_TEST_CASE("Normalize a vector", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> vector(2, 0, 0);
	Ck::Vector3<TestType> normalized = Ck::Vector3<TestType>::Normalize(vector);

	SECTION("Its length should be one")
	{
		REQUIRE(Ck::NearlyEqual(normalized.GetLength(), 1));
	}

	SECTION("Its components should all be clamped between 0 and 1")
	{
		REQUIRE(Ck::NearlyEqual(normalized.X(), 1));
		REQUIRE(Ck::NearlyEqual(normalized.Y(), 0));
		REQUIRE(Ck::NearlyEqual(normalized.Z(), 0));
	}
}

TEMPLATE_TEST_CASE("Compute the dot product of two vectors", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> lhs(1, 0, 1);
	Ck::Vector3<TestType> rhs(0, 1, 1);

	REQUIRE(Ck::NearlyEqual(Ck::Vector3<TestType>::DotProduct(lhs, rhs), 1));
}

TEMPLATE_TEST_CASE("Compute the cross product of two vectors", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> lhs = Ck::Vector3<TestType>::Right();
	Ck::Vector3<TestType> rhs = Ck::Vector3<TestType>::Forward();

	REQUIRE(Ck::Vector3<TestType>::CrossProduct(lhs, rhs) == Ck::Vector3<TestType>::Up());
}

TEMPLATE_TEST_CASE("Create a default Vector3", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> vector;

	REQUIRE(Ck::NearlyEqual(vector.X(), 0));
	REQUIRE(Ck::NearlyEqual(vector.Y(), 0));
	REQUIRE(Ck::NearlyEqual(vector.Z(), 0));
}

TEMPLATE_TEST_CASE("Create a Vector3 by specifying its components", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> vector(2, -7, 0.5);

	REQUIRE(Ck::NearlyEqual(vector.X(), 2));
	REQUIRE(Ck::NearlyEqual(vector.Y(), -7));
	REQUIRE(Ck::NearlyEqual(vector.Z(), 0.5));
}

TEMPLATE_TEST_CASE("The length of an unit vector is 1", "[Vector3]", float, double)
{
	Ck::Vector3<TestType> vector(1, 0, 0);

	SECTION("Its length should be 1")
	{
		REQUIRE(Ck::NearlyEqual(vector.GetLength(), 1));
	}

	SECTION("It should not change when normalized")
	{
		Ck::Vector3<TestType> normalized = Ck::Vector3<TestType>::Normalize(vector);
		REQUIRE(vector == normalized);
	}
}