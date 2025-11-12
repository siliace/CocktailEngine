#include <catch2/catch_all.hpp>

#include <Cocktail/Core/String.hpp>

#include "Cocktail/Core/Exception.hpp"

TEST_CASE("Create a string", "[String]") {
	SECTION("Empty")
	{
		Ck::String string;

		REQUIRE(string.IsEmpty());
		REQUIRE(string.GetLength() == 0);
	}

	SECTION("From a null terminated c-string")
	{
		Ck::String string(CK_TEXT("Hello World"));

		REQUIRE_FALSE(string.IsEmpty());
		REQUIRE(string.GetLength() == 11);
	}

	SECTION("From a c-string and a length")
	{
		Ck::String string(CK_TEXT("Hello World"), 5);

		REQUIRE_FALSE(string.IsEmpty());
		REQUIRE(string.GetLength() == 5);
	}

	SECTION("By copying another one")
	{
		Ck::String string(CK_TEXT("Hello World"), 5);
		Ck::String other(string);

		REQUIRE_FALSE(string.IsEmpty());
		REQUIRE(string.GetLength() == 5);

		REQUIRE_FALSE(other.IsEmpty());
		REQUIRE(other.GetLength() == 5);
	}

	SECTION("By moving another one")
	{
		Ck::String string(CK_TEXT("Hello World"), 5);
		Ck::String other(std::move(string));

		REQUIRE(string.IsEmpty());

		REQUIRE_FALSE(other.IsEmpty());
		REQUIRE(other.GetLength() == 5);
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

TEST_CASE("Get a char", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"));

	SECTION("With a good index")
	{
		REQUIRE(string.At(4) == CK_CHAR('o'));
	}

	SECTION("With a bad index")
	{
		REQUIRE_THROWS_AS(string.At(448451), Ck::ContainerOutOfRange);
	}
}

TEST_CASE("Try to get a char", "[String]")
{
	Ck::String string(CK_TEXT("Hello World"));

	SECTION("With a good index")
	{
		Ck::Optional<Ck::TextChar&> character = string.TryAt(4);

		REQUIRE_FALSE(character.IsEmpty());
		REQUIRE(character.Get() == CK_CHAR('o'));
	}

	SECTION("With a bad index")
	{
		Ck::Optional<Ck::TextChar&> character = string.TryAt(448451);

		REQUIRE(character.IsEmpty());
	}
}

TEST_CASE("Get an UTF codepoint", "[String]")
{
	Ck::String string(CK_TEXT("My name is Gérard"));

	SECTION("With a good index")
	{
		Ck::Optional<Ck::Utf32Char> codepoint = string.TryCodepointAt(12);

		REQUIRE_FALSE(codepoint.IsEmpty());
		REQUIRE(codepoint.Get() == 0x000000E9);
	}

	SECTION("With a bad index")
	{
		Ck::Optional<Ck::Utf32Char> codepoint = string.TryCodepointAt(448451);

		REQUIRE(codepoint.IsEmpty());
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

TEST_CASE("Get a substring", "[String]")
{
	Ck::String string = CK_TEXT("Hello World");

	SECTION("From an index to a string")
	{
		REQUIRE(string.SubString(6) == CK_TEXT("World"));
	}

	SECTION("From an index with a length to a string")
	{
		REQUIRE(string.SubString(6, 3) == CK_TEXT("Wor"));
	}

	SECTION("From an index to a string view")
	{
		REQUIRE(string.SubStringView(6) == CK_TEXT("World"));
	}

	SECTION("From an index with a length to a string view")
	{
		REQUIRE(string.SubStringView(6, 3) == CK_TEXT("Wor"));
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

TEST_CASE("Hash a string view", "[String]")
{
	Ck::StringView left = CK_TEXT("Hello");
	Ck::StringView right = CK_TEXT("World");

	REQUIRE_FALSE(std::hash<Ck::StringView>()(left) == std::hash<Ck::StringView>()(right));
}