#include <catch2/catch_all.hpp>

#include <Cocktail/Core/System/FileSystem/URI.hpp>

TEST_CASE("Parse an URI", "[URI]")
{
	SECTION("Parse a path without scheme")
	{
		Ck::String path = CK_TEXT("/home/user/siliace");
		Ck::URI uri = Ck::URI::Parse(path);

		REQUIRE(uri.GetScheme() == Ck::String::Empty);
		REQUIRE(uri.GetPath() == Ck::Path::Parse(path));
	}

	SECTION("Parse a path with a file scheme")
	{
		Ck::String path = CK_TEXT("/home/user/siliace");
		Ck::URI uri = Ck::URI::Parse(CK_TEXT("file:///home/user/siliace"));

		REQUIRE(uri.GetScheme() == CK_TEXT("file"));
		REQUIRE(uri.GetPath().ToString() == path);
	}
}