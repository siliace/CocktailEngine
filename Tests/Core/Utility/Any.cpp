#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/Any.hpp>

TEST_CASE("Create an Any", "[Any]")
{
	SECTION("with default constructor")
	{
		Ck::Any any;

		REQUIRE(any.IsEmpty());
	}

	SECTION("by copying a value")
	{
		Ck::Any any(int{ 42 });

		REQUIRE_FALSE(any.IsEmpty());
	}

	SECTION("by construction in place an object")
	{
		Ck::Any any(Ck::InPlaceType<Ck::String>, CK_TEXT("Hello World"));

		REQUIRE_FALSE(any.IsEmpty());
	}
}

TEST_CASE("Cast an Any to its underlying value", "[Any]")
{
	Ck::Any any(int{ 42 });

	SECTION("Cast to the right type")
	{
		REQUIRE(any.Get<int>() == 42);
	}

	SECTION("Cast to a bad type")
	{
		REQUIRE_THROWS_AS(any.Get<double>() == 42.0, Ck::AnyCastException);
	}
}

TEST_CASE("Swap two Anys", "[Any]")
{
	Ck::Any lhs(int{ 42 });
	Ck::Any rhs;

	lhs.Swap(rhs);

	REQUIRE(lhs.IsEmpty());
	REQUIRE_FALSE(rhs.IsEmpty());

	REQUIRE_THROWS_AS(lhs.Get<int>() == 42, Ck::AnyCastException);
	REQUIRE(rhs.Get<int>() == 42);
}

TEST_CASE("Reset an Any", "[Any]")
{
	Ck::Any any(int{ 42 });

	REQUIRE_FALSE(any.IsEmpty());

	any.Reset();

	REQUIRE(any.IsEmpty());
}