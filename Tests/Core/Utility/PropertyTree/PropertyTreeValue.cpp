#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

TEST_CASE("Create a property tree value", "[PropertyTreeValue]")
{
	SECTION("With a string-able value")
	{
		Ck::Properties::ValueType value(42);

		REQUIRE(value.GetType() == Ck::Properties::NodeType::Type::Value);
		REQUIRE(value.As<unsigned int>() == static_cast<unsigned int>(42));
	}

	SECTION("With a raw string")
	{
		Ck::Properties::ValueType value(CK_TEXT("Hello World"));

		REQUIRE(value.GetType() == Ck::Properties::NodeType::Type::Value);
		REQUIRE(value.As<Ck::String>() == CK_TEXT("Hello World"));
	}
}