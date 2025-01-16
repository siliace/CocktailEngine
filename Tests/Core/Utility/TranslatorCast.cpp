#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/TranslatorCast.hpp>

TEST_CASE("Translation cast between a string and a scalar", "[TranslatorCast]")
{
	const int i = 42;
	const std::string str = "42";

	SECTION("String to scalar")
	{
		REQUIRE(Ck::TranslatorCast<int>(str) == i);
	}

	SECTION("Scalar to string")
	{
		REQUIRE(Ck::TranslatorCast<std::string>(i) == str);
	}
}

TEST_CASE("Translation cast between a string and a floating point", "[TranslatorCast]")
{
	const double i = 42.569;
	const std::string str = "42.569";

	SECTION("String to floating")
	{
		REQUIRE(Ck::NearlyEqual(Ck::TranslatorCast<double>(str), i));
	}

	SECTION("Floating to string")
	{
		/// Fix precision issues ? :(
		REQUIRE(Ck::TranslatorCast<std::string>(i).substr(0, str.length()) == str);
	}
}