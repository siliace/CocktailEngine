#include <catch2/catch_all.hpp>

#include <Cocktail/Core/String.hpp>

TEST_CASE("Create a default string", "[String]")
{
	Ck::String string;

	REQUIRE(string.IsEmpty());
	REQUIRE(string.GetLength() == 0);
}

TEST_CASE("Create a string", "[String]")
{
	Ck::String string = CK_TEXT("Hello World");

	REQUIRE_FALSE(string.IsEmpty());
	REQUIRE(string.GetLength() == 11);
}

TEST_CASE("Create a string with a length", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"), 5);

	REQUIRE_FALSE(string.IsEmpty());
	REQUIRE(string.GetLength() == 5);
}

TEST_CASE("Prepend a string at the end of another one", "[String]")
{
	Ck::String string = CK_TEXT("World");

	SECTION("With a good string")
	{
		string.Prepend(CK_TEXT(" ")).Prepend(CK_TEXT("Hello"));
		REQUIRE(string == CK_TEXT("Hello World"));
	}

	SECTION("With an empty string")
	{
		string.Prepend(Ck::String::Empty);
		REQUIRE(string == CK_TEXT("World"));
	}

	SECTION("With itself")
	{
		string.Prepend(string);
		REQUIRE(string == CK_TEXT("WorldWorld"));
	}
}

TEST_CASE("Append a string at the end of another one", "[String]")
{
	Ck::String string = CK_TEXT("Hello");

	SECTION("With a good string")
	{
		string.Append(CK_TEXT(" ")).Append(CK_TEXT("World"));
		REQUIRE(string == CK_TEXT("Hello World"));
	}

	SECTION("With an empty string")
	{
		string.Append(Ck::String::Empty);
		REQUIRE(string == CK_TEXT("Hello"));
	}

	SECTION("With itself")
	{
		string.Append(string);
		REQUIRE(string == CK_TEXT("HelloHello"));
	}
}

TEST_CASE("Compare two strings", "[String]")
{
	Ck::String abcd = CK_TEXT("abcd");
	Ck::String wxyz = CK_TEXT("wxyz");

	Ck::String aBcD = CK_TEXT("aBcD");
	Ck::String wXyZ = CK_TEXT("wXyZ");

	SECTION("In case sensitive mode")
	{
		REQUIRE(abcd.Compare(abcd, true) == 0);
		REQUIRE(abcd.Compare(wxyz, true) == -1);
		REQUIRE(abcd.Compare(aBcD, true) == 1);
		REQUIRE(abcd.Compare(wXyZ, true) == -1);

		REQUIRE(wxyz.Compare(abcd, true) == 1);
		REQUIRE(wxyz.Compare(wxyz, true) == 0);
		REQUIRE(wxyz.Compare(aBcD, true) == 1);
		REQUIRE(wxyz.Compare(wXyZ, true) == 1);

		REQUIRE(aBcD.Compare(abcd, true) == -1);
		REQUIRE(aBcD.Compare(wxyz, true) == -1);
		REQUIRE(aBcD.Compare(aBcD, true) == 0);
		REQUIRE(aBcD.Compare(wXyZ, true) == -1);

		REQUIRE(wXyZ.Compare(abcd, true) == 1);
		REQUIRE(wXyZ.Compare(wxyz, true) == -1);
		REQUIRE(wXyZ.Compare(aBcD, true) == 1);
		REQUIRE(wXyZ.Compare(wXyZ, true) == 0);
	}

	SECTION("In non case sensitive mode")
	{
		REQUIRE(abcd.Compare(abcd, false) == 0);
		REQUIRE(abcd.Compare(wxyz, false) == -1);
		REQUIRE(abcd.Compare(aBcD, false) == 0);
		REQUIRE(abcd.Compare(wXyZ, false) == -1);

		REQUIRE(wxyz.Compare(abcd, false) == 1);
		REQUIRE(wxyz.Compare(wxyz, false) == 0);
		REQUIRE(wxyz.Compare(aBcD, false) == 1);
		REQUIRE(wxyz.Compare(wXyZ, false) == 0);

		REQUIRE(aBcD.Compare(abcd, false) == 0);
		REQUIRE(aBcD.Compare(wxyz, false) == -1);
		REQUIRE(aBcD.Compare(aBcD, false) == 0);
		REQUIRE(aBcD.Compare(wXyZ, false) == -1);

		REQUIRE(wXyZ.Compare(abcd, false) == 1);
		REQUIRE(wXyZ.Compare(wxyz, false) == 0);
		REQUIRE(wXyZ.Compare(aBcD, false) == 1);
		REQUIRE(wXyZ.Compare(wXyZ, false) == 0);
	}
}

TEST_CASE("Test if an string starts with another one", "[String]")
{
	Ck::String string = CK_TEXT("Hello World");

	SECTION("With a valid string")
	{
		REQUIRE(string.StartsWith(CK_TEXT("Hello")));
		REQUIRE_FALSE(string.StartsWith(CK_TEXT("World")));
	}

	SECTION("With an empty string")
	{
		REQUIRE(string.StartsWith(Ck::String::Empty));
	}

	SECTION("With a longer string")
	{
		REQUIRE_FALSE(string.StartsWith(CK_TEXT("Hello World, this is me")));
	}
}

TEST_CASE("Test if an string ends with another one", "[String]")
{
	Ck::String string = CK_TEXT("Hello World");

	SECTION("With a valid string")
	{
		REQUIRE(string.EndsWith(CK_TEXT("World")));
		REQUIRE_FALSE(string.EndsWith(CK_TEXT("Hello")));
	}

	SECTION("With an empty string")
	{
		REQUIRE(string.EndsWith(Ck::String::Empty));
	}

	SECTION("With a longer string")
	{
		REQUIRE_FALSE(string.EndsWith(CK_TEXT("Hello World, this is me")));
	}
}

TEST_CASE("Split a string", "[String]")
{
	Ck::String string = CK_TEXT("Hello world what a nice day");
	Ck::Array<Ck::String> tokens = string.Split(' ');

	REQUIRE(tokens.GetSize() == 6);
	REQUIRE(tokens[0] == CK_TEXT("Hello"));
	REQUIRE(tokens[1] == CK_TEXT("world"));
	REQUIRE(tokens[2] == CK_TEXT("what"));
	REQUIRE(tokens[3] == CK_TEXT("a"));
	REQUIRE(tokens[4] == CK_TEXT("nice"));
	REQUIRE(tokens[5] == CK_TEXT("day"));
}

TEST_CASE("Hash a string", "[String]")
{
	Ck::String left = CK_TEXT("Hello");
	Ck::String right = CK_TEXT("World");

	REQUIRE_FALSE(std::hash<Ck::String>()(left) == std::hash<Ck::String>()(right));
}