#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp>

TEST_CASE("Create a property tree value", "[PropertyTreeValue]")
{
	SECTION("With a string-able value")
	{
		Ck::Properties::Value value(42);

		REQUIRE(value.GetType() == Ck::Properties::Node::Type::Value);
		REQUIRE(value.As<unsigned short>() == static_cast<unsigned short>(42));
	}

	SECTION("With a raw string")
	{
		Ck::Properties::Value value("Hello World");

		REQUIRE(value.GetType() == Ck::Properties::Node::Type::Value);
		REQUIRE(value.As<std::string>() == "Hello World");
	}
}