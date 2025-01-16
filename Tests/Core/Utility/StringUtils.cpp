#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/StringUtils.hpp>

TEST_CASE("String begins with", "[StringUtils]")
{
	REQUIRE(Ck::StringUtils::StartsWith("Hello World", "Hello"));
}

TEST_CASE("String ends with", "[StringUtils]")
{
	REQUIRE(Ck::StringUtils::EndsWith("Hello World", "World"));
}

TEST_CASE("Split string", "[StringUtils]")
{
	std::vector<std::string> tokens = Ck::StringUtils::Split("Hello world what a nice day", ' ');

	REQUIRE(tokens.size() == 6);
	REQUIRE(tokens[0] == "Hello");
	REQUIRE(tokens[1] == "world");
	REQUIRE(tokens[2] == "what");
	REQUIRE(tokens[3] == "a");
	REQUIRE(tokens[4] == "nice");
	REQUIRE(tokens[5] == "day");
}

TEST_CASE("Join string", "[StringUtils]")
{
	std::vector<std::string> tokens = {
	"Hello", "world", "what", "a", "nice", "day"
	};
	REQUIRE(Ck::StringUtils::Join(tokens.begin(), tokens.end(), " ") == "Hello world what a nice day");
}

TEST_CASE("Capitalize string", "[StringUtils]")
{
	REQUIRE(Ck::StringUtils::Capitalize("Hello world") == "HELLO WORLD");
}

TEST_CASE("Minusculize string", "[StringUtils]")
{
	REQUIRE(Ck::StringUtils::Minusculize("Hello world") == "hello world");
}