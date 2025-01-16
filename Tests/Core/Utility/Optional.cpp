#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Utility/Optional.hpp>

TEST_CASE("Default optional is empty", "[Optional]")
{
	Ck::Optional<std::string> opt;
	REQUIRE(opt.IsEmpty());
}

TEST_CASE("Construct optional in-place", "[Optional]")
{
	Ck::Optional<std::string> opt = Ck::Optional<std::string>::Of(Ck::InPlace, "Hello world");
	REQUIRE_FALSE(opt.IsEmpty());
	REQUIRE(opt.Get() == "Hello world");
}

TEST_CASE("Move construct leave optional empty", "[Optional]")
{
	const std::string str = "foo";

	Ck::Optional<std::string> opt = Ck::Optional<std::string>::Of(str);
	Ck::Optional<std::string> opt2;

	REQUIRE_FALSE(opt.IsEmpty());
	REQUIRE(opt.Get() == str);
	REQUIRE(opt2.IsEmpty());

	opt2 = std::move(opt);

	REQUIRE(opt.IsEmpty());
	REQUIRE_FALSE(opt2.IsEmpty());
	REQUIRE(opt2.Get() == str);
}

TEST_CASE("Map of an optional", "[Optional]")
{
	Ck::Optional<std::string> opt;

	auto mapper = [](const std::string& str) {
		return str + str;
	};

	SECTION("Empty optional mapped to empty")
	{
		opt = opt.Map(mapper);

		REQUIRE(opt.IsEmpty());
	}

	SECTION("Not empty optional invoke mapper")
	{
		opt = Ck::Optional<std::string>::Of("foo");

		opt = opt.Map(mapper);

		REQUIRE_FALSE(opt.IsEmpty());
		REQUIRE(opt.Get() == "foofoo");
	}
}

TEST_CASE("Then of an optional", "[Optional]")
{
	Ck::Optional<std::string> opt;

	bool called = false;
	auto then = [&](const std::string&) {
		called = true;
	};

	SECTION("Empty optional mapped to empty")
	{
		opt.Then(then);

		REQUIRE_FALSE(called);
	}

	SECTION("Not empty optional invoke mapper")
	{
		opt = Ck::Optional<std::string>::Of("foo");
		opt.Then(then);

		REQUIRE(called);
	}
}

TEST_CASE("Get the value of an optional", "[Optional]")
{
	Ck::Optional<std::string> opt;
	
	SECTION("Empty optional gives an exception")
	{
		REQUIRE_THROWS_AS(opt.Get(), Ck::EmptyOptionalException);
	}

	SECTION("Not empty optional gives a value")
	{
		opt = Ck::Optional<std::string>::Of("foo");
		REQUIRE(opt.Get() == "foo");
	}
}

TEST_CASE("Get the value with fallback of an optional", "[Optional]")
{
	Ck::Optional<std::string> opt;

	SECTION("Empty optional gives the fallback")
	{
		REQUIRE(opt.GetOr("fallback") == "fallback");
	}

	SECTION("Not empty optional gives a value")
	{
		opt = Ck::Optional<std::string>::Of("foo");
		REQUIRE(opt.GetOr("fallback") == "foo");
	}
}