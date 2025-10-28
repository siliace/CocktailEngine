#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/ByteArray.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

TEST_CASE("Create a ByteArray", "[ByteArray]")
{
	SECTION("with default constructor")
	{
		Ck::ByteArray byteArray;

		REQUIRE(byteArray.GetSize() == 0);
	}

	SECTION("from a memory area")
	{
		const char* data = "Hello World";
		const std::size_t length = std::strlen(data);

		Ck::ByteArray byteArray(data, length);

		REQUIRE(byteArray.GetSize() == length);
	}

	SECTION("by copying an existing one")
	{
		const char* data = "Hello World";
		const std::size_t length = std::strlen(data);

		Ck::ByteArray byteArray1(data, length);
		Ck::ByteArray byteArray2 = byteArray1;

		REQUIRE(byteArray2.GetSize() == length);
	}

	SECTION("by copying an moving one")
	{
		const char* data = "Hello World";
		const std::size_t length = std::strlen(data);

		Ck::ByteArray byteArray1(data, length);
		Ck::ByteArray byteArray2 = std::move(byteArray1);

		REQUIRE(byteArray1.GetSize() == 0);
		REQUIRE(byteArray2.GetSize() == length);
	}
}