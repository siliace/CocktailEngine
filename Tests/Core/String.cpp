#include <catch2/catch_all.hpp>

#include <Cocktail/Core/String.hpp>

TEST_CASE("Get a substring from a string", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"));

	REQUIRE(string.Substring(6) == CK_TEXT("World"));
	REQUIRE(string.Substring(0, 5) == CK_TEXT("Hello"));
}

TEST_CASE("Append a string to another one", "[String]")
{
	Ck::String string(CK_TEXT("Hello"));
	string.Append(Ck::InPlace, CK_TEXT(" World"));

	REQUIRE(string == CK_TEXT("Hello World"));
}

TEST_CASE("Test if a string stats with a prefix", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"));

	SECTION("With a good prefix")
	{
		REQUIRE(string.StartsWith(CK_TEXT("Hello")));
	}

	SECTION("With a good prefix with bad case")
	{
		REQUIRE_FALSE(string.StartsWith(CK_TEXT("HeLlO"), true));
	}

	SECTION("With a good prefix ignoring the case")
	{
		REQUIRE(string.StartsWith(CK_TEXT("HeLlO"), false));
	}

	SECTION("With a bad prefix")
	{
		REQUIRE_FALSE(string.StartsWith(CK_TEXT("World")));
	}
}

TEST_CASE("Test if a string ends with a suffix", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"));

	SECTION("With a good suffix")
	{
		REQUIRE(string.EndsWith(CK_TEXT("World"), true));
		REQUIRE(string.EndsWith(CK_TEXT("WoRlD"), false));
	}

	SECTION("With a bad suffix")
	{
		REQUIRE_FALSE(string.EndsWith(CK_TEXT("Hello")));
	}
}

TEST_CASE("Compare strings", "[String]")
{
	Ck::String string(CK_TEXT("mnop"));

	SECTION("with the same other string")
	{
		REQUIRE(string.Compare(CK_TEXT("mnop"), true) == 0);
		REQUIRE(string.Compare(CK_TEXT("mnop"), false) == 0);
	}

	SECTION("with another string lexically superior")
	{
		REQUIRE(string.Compare(CK_TEXT("wxyz"), true) == -1);
		REQUIRE(string.Compare(CK_TEXT("wXyZ"), false) == -1);
	}

	SECTION("with another string lexically inferior")
	{
		REQUIRE(string.Compare(CK_TEXT("abcd"), true) == 1);
		REQUIRE(string.Compare(CK_TEXT("aBcD"), false) == 1);
	}
}

TEST_CASE("Split string", "[String]")
{
	Ck::String string(CK_TEXT("Hello world what a nice day"));
	Ck::Array<Ck::String> tokens = string.Split(' ');

	REQUIRE(tokens.GetSize() == 6);
	REQUIRE(tokens[0] == CK_TEXT("Hello"));
	REQUIRE(tokens[1] == CK_TEXT("world"));
	REQUIRE(tokens[2] == CK_TEXT("what"));
	REQUIRE(tokens[3] == CK_TEXT("a"));
	REQUIRE(tokens[4] == CK_TEXT("nice"));
	REQUIRE(tokens[5] == CK_TEXT("day"));
}