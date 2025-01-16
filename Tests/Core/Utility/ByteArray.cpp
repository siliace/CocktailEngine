#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/ByteArray.hpp>

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

TEST_CASE("Append data to a ByteArray", "[ByteArray]")
{
	SECTION("initially empty")
	{
		Ck::ByteArray lhs;
		Ck::ByteArray rhs = Ck::ByteArray::FromString("Hello World");

		lhs.Append(rhs);

		REQUIRE(lhs.ToString() == "Hello World");
	}

	SECTION("initially not empty")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString("Hello");
		Ck::ByteArray rhs = Ck::ByteArray::FromString(" World");

		lhs.Append(rhs);

		REQUIRE(lhs.ToString() == "Hello World");
	}
}

TEST_CASE("Prepend data to a ByteArray", "[ByteArray]")
{
	SECTION("initially empty")
	{
		Ck::ByteArray lhs;
		Ck::ByteArray rhs = Ck::ByteArray::FromString("Hello World");

		lhs.Prepend(rhs);

		REQUIRE(lhs.ToString() == "Hello World");
	}

	SECTION("initially not empty")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString(" World");
		Ck::ByteArray rhs = Ck::ByteArray::FromString("Hello");

		lhs.Prepend(rhs);

		REQUIRE(lhs.ToString() == "Hello World");
	}
}

TEST_CASE("Insert data to a ByteArray", "[ByteArray]")
{
	SECTION("in the middle")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString("Helrld");
		Ck::ByteArray rhs = Ck::ByteArray::FromString("lo Wo");

		lhs.Insert(3, rhs);

		REQUIRE(lhs.ToString() == "Hello World");
	}
}

TEST_CASE("Get a slice of a ByteArray", "[ByteArray]")
{
	Ck::ByteArray lhs = Ck::ByteArray::FromString("Hello World");
	REQUIRE(lhs.Slice(3, 5) == Ck::ByteArray::FromString("lo Wo"));
}

TEST_CASE("Remove data to a ByteArray", "[ByteArray]")
{
	SECTION("at the beginning")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString("Hello World");

		lhs.Remove(0, 5);

		REQUIRE(lhs.ToString() == " World");
	}

	SECTION("in the middle")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString("Hello World");

		lhs.Remove(3, 5);

		REQUIRE(lhs.ToString() == "Helrld");
	}

	SECTION("at the end")
	{
		Ck::ByteArray lhs = Ck::ByteArray::FromString("Hello World");

		lhs.Remove(4, 6);

		REQUIRE(lhs.ToString() == "Hello");
	}
}

TEST_CASE("Convert a ByteArray to a string", "[ByteArray]")
{
	SECTION("To a char string")
	{
		const char* data = "Hello World";
		const std::size_t length = std::strlen(data);

		Ck::ByteArray byteArray(data, length);

		REQUIRE(byteArray.ToString() == data);
	}

	SECTION("To a wchar_t string")
	{
		const wchar_t* data = L"Hello World";
		const std::size_t length = std::wcslen(data);

		Ck::ByteArray byteArray(data, length);

		REQUIRE(byteArray.ToString<wchar_t>() == data);
	}
}