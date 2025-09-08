#include <catch2/catch_all.hpp>

#include <Cocktail/Core/System/FileSystem/Path.hpp>

TEST_CASE("Parse a path", "[Path]")
{
	SECTION("With a relative path")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT("hello/world"));

		REQUIRE_FALSE(path.IsAbsolute());
		REQUIRE(path.GetRoot() == Ck::String::Empty);
		REQUIRE(path.ToString() == CK_TEXT("hello/world"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT("hello"));
		REQUIRE(path.GetParent().GetParent().ToString() == Ck::String::Empty);
	}
	SECTION("With a relative path starting with a '.'")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT("./hello/world"));

		REQUIRE_FALSE(path.IsAbsolute());
		REQUIRE(path.GetRoot() == Ck::String::Empty);
		REQUIRE(path.ToString() == CK_TEXT("./hello/world"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT("./hello"));
		REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT("."));
	}

	SECTION("With a relative Windows style path")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(..\..\Siliace\Documents)"));

		REQUIRE_FALSE(path.IsAbsolute());
		REQUIRE(path.GetRoot() == Ck::String::Empty);
		REQUIRE(path.ToString() == CK_TEXT(R"(..\..\Siliace\Documents)"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(..\..\Siliace)"));
		REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT(R"(..\..)"));
	}

	SECTION("With an absolute Windows style path")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(C:\Users\Siliace\Documents)"));

		REQUIRE(path.IsAbsolute());
		REQUIRE(path.GetRoot() == CK_TEXT(R"(C:\)"));
		REQUIRE(path.ToString() == CK_TEXT(R"(C:\Users\Siliace\Documents)"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(C:\Users\Siliace)"));
		REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT(R"(C:\Users)"));
	}

	SECTION("With an absolute UNC style path")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT(R"(\\Server\Share\Users\Siliace\Documents)"));

		REQUIRE(path.IsAbsolute());
		REQUIRE(path.GetRoot() == CK_TEXT(R"(\\Server\Share)"));
		REQUIRE(path.ToString() == CK_TEXT(R"(\\Server\Share\Users\Siliace\Documents)"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT(R"(\\Server\Share\Users\Siliace)"));
	}

	SECTION("With an absolute unix style path")
	{
		Ck::Path path = Ck::Path::Parse(CK_TEXT("/home/siliace/documents"));

		REQUIRE(path.IsAbsolute());
		REQUIRE(path.ToString() == CK_TEXT("/home/siliace/documents"));
		REQUIRE(path.GetParent().ToString() == CK_TEXT("/home/siliace"));
		REQUIRE(path.GetParent().GetParent().ToString() == CK_TEXT("/home"));
	}
}

TEST_CASE("Join two paths", "[Path]")
{
	Ck::Path absolute = Ck::Path::Parse(CK_TEXT("/home/siliace"), Ck::Path::Format::Generic);
	Ck::Path relative = Ck::Path::Parse(CK_TEXT("./documents/c++"), Ck::Path::Format::Generic);

	SECTION("A relative into an absolute")
	{
		REQUIRE(absolute.Join(relative).ToString() == CK_TEXT("/home/siliace/./documents/c++"));
	}

	SECTION("An absolute into an relative")
	{
		REQUIRE(relative.Join(absolute).ToString() == CK_TEXT("/home/siliace"));
	}
}