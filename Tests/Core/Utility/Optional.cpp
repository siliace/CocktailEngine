#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

TEST_CASE("Default optional is empty", "[Optional]")
{
	Ck::Optional<Ck::String> opt;
	REQUIRE(opt.IsEmpty());
}

TEST_CASE("Construct optional in-place", "[Optional]")
{
	Ck::Optional<Ck::String> opt = Ck::Optional<Ck::String>::Of(Ck::InPlace, CK_TEXT("Hello world"));
	REQUIRE_FALSE(opt.IsEmpty());
	REQUIRE(opt.Get() == CK_TEXT("Hello world"));
}

TEST_CASE("Construct optional from a reference", "[Optional]")
{
	Ck::String str = CK_TEXT("Hello world");
	Ck::Optional<Ck::String&> opt = Ck::Optional<Ck::String&>::Of(str);

	Ck::String& stringRef = opt.Get();
	REQUIRE(&str == &stringRef);
}

TEST_CASE("Move construct leave optional empty", "[Optional]")
{
	const Ck::String str = CK_TEXT("foo");

	Ck::Optional<Ck::String> opt = Ck::Optional<Ck::String>::Of(str);
	Ck::Optional<Ck::String> opt2;

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
	Ck::Optional<Ck::String> opt;

	auto mapper = [](const Ck::String& str) {
		return str + str;
	};

	SECTION("Empty optional mapped to empty")
	{
		opt = opt.Map(mapper);

		REQUIRE(opt.IsEmpty());
	}

	SECTION("Not empty optional invoke mapper")
	{
		opt = Ck::Optional<Ck::String>::Of(CK_TEXT("foo"));

		opt = opt.Map(mapper);

		REQUIRE_FALSE(opt.IsEmpty());
		REQUIRE(opt.Get() == CK_TEXT("foofoo"));
	}
}

TEST_CASE("Then of an optional", "[Optional]")
{
	Ck::Optional<Ck::String> opt;

	bool called = false;
	auto then = [&](const Ck::String&) {
		called = true;
	};

	SECTION("Empty optional mapped to empty")
	{
		opt.Then(then);

		REQUIRE_FALSE(called);
	}

	SECTION("Not empty optional invoke mapper")
	{
		opt = Ck::Optional<Ck::String>::Of(CK_TEXT("foo"));
		opt.Then(then);

		REQUIRE(called);
	}
}

TEST_CASE("Get the value of an optional", "[Optional]")
{
	Ck::Optional<Ck::String> opt;
	
	SECTION("Empty optional gives an exception")
	{
		REQUIRE_THROWS_AS(opt.Get(), Ck::EmptyOptionalException);
	}

	SECTION("Not empty optional gives a value")
	{
		opt = Ck::Optional<Ck::String>::Of(CK_TEXT("foo"));
		REQUIRE(opt.Get() == CK_TEXT("foo"));
	}
}

TEST_CASE("Get the value with fallback of an optional", "[Optional]")
{
	Ck::Optional<Ck::String> opt;

	SECTION("Empty optional gives the fallback")
	{
		REQUIRE(opt.GetOr(CK_TEXT("fallback")) == CK_TEXT("fallback"));
	}

	SECTION("Not empty optional gives a value")
	{
		opt = Ck::Optional<Ck::String>::Of(CK_TEXT("foo"));
		REQUIRE(opt.GetOr(CK_TEXT("fallback")) == CK_TEXT("foo"));
	}
}