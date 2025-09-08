#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/TranslatorCast.hpp>

TEST_CASE("Parse an integer", "[String]")
{

}

TEST_CASE("Translation cast between a string and a scalar", "[TranslatorCast]")
{
	SECTION("String to scalar")
	{
		SECTION("With 0")
		{
			REQUIRE(Ck::TranslatorCast<int>("0") == 0);
		}

		SECTION("With a signed integer")
		{
			REQUIRE(Ck::TranslatorCast<int>("-10007") == -10007);
			REQUIRE(Ck::TranslatorCast<int>("-15013") == -15013);
			REQUIRE(Ck::TranslatorCast<int>("-20021") == -20021);
			REQUIRE(Ck::TranslatorCast<int>("-25031") == -25031);
			REQUIRE(Ck::TranslatorCast<int>("-30047") == -30047);

			REQUIRE(Ck::TranslatorCast<unsigned int>("35023") == 35023);
			REQUIRE(Ck::TranslatorCast<unsigned int>("40063") == 40063);
			REQUIRE(Ck::TranslatorCast<unsigned int>("45119") == 45119);
			REQUIRE(Ck::TranslatorCast<unsigned int>("50129") == 50129);
			REQUIRE(Ck::TranslatorCast<unsigned int>("55147") == 55147);
		}

		SECTION("With an unsigned integer")
		{
			REQUIRE(Ck::TranslatorCast<Ck::String>(100003u) == CK_TEXT("100003"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(105013u) == CK_TEXT("105013"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(110017u) == CK_TEXT("110017"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(115027u) == CK_TEXT("115027"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(120041u) == CK_TEXT("120041"));
		}
	}

	SECTION("Scalar to string")
	{
		SECTION("With 0")
		{
			REQUIRE(Ck::TranslatorCast<Ck::String>(0) == CK_TEXT("0"));
		}

		SECTION("With a signed integer")
		{
			REQUIRE(Ck::TranslatorCast<Ck::String>(-10007) == CK_TEXT("-10007"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(-15013) == CK_TEXT("-15013"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(-20021) == CK_TEXT("-20021"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(-25031) == CK_TEXT("-25031"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(-30047) == CK_TEXT("-30047"));

			REQUIRE(Ck::TranslatorCast<Ck::String>(35023) == CK_TEXT("35023"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(40063) == CK_TEXT("40063"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(45119) == CK_TEXT("45119"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(50129) == CK_TEXT("50129"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(55147) == CK_TEXT("55147"));
		}

		SECTION("With an unsigned integer")
		{
			REQUIRE(Ck::TranslatorCast<Ck::String>(100003u) == CK_TEXT("100003"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(105013u) == CK_TEXT("105013"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(110017u) == CK_TEXT("110017"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(115027u) == CK_TEXT("115027"));
			REQUIRE(Ck::TranslatorCast<Ck::String>(120041u) == CK_TEXT("120041"));
		}
	}
}