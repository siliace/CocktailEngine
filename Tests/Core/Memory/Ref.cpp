#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Memory/WeakRef.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

class Foo : public Ck::Extends<Foo, Ck::Object>
{
public:

};

TEST_CASE("Create a null Ref", "[Ref]")
{
	REQUIRE(Ck::Ref<Foo>() == nullptr);
}

TEST_CASE("Create a Ref by copying another none", "[Ref]")
{
	Ck::Ref<Foo> ref = Foo::New();
	Ck::Ref<Foo> ref2 = ref;

	SECTION("References must be the same")
	{
		REQUIRE(ref2 != nullptr);
		REQUIRE(ref2 == ref);
		REQUIRE(ref2.GetRefCounter() == ref.GetRefCounter());
	}

	SECTION("Null one of the references must affect the RefCounter of the other one")
	{
		REQUIRE(ref2.GetRefCounter()->GetCounter() == 2);

		ref = nullptr;

		REQUIRE(ref2.GetRefCounter()->GetCounter() == 1);
	}
}

TEST_CASE("Creating a WeakRef from a Ref", "[Ref]")
{
	Ck::Ref<Foo> ref = Foo::New();

	SECTION("Only the weak counter should be incremented")
	{
		Ck::WeakRef<Foo> weakRef = ref;

		REQUIRE(weakRef.GetRefCounter()->GetCounter() == 1);
		REQUIRE(weakRef.GetRefCounter()->GetWeakCounter() == 2);
	}

	SECTION("When the Ref is deleted, the WeakRef should be expired")
	{
		Ck::WeakRef<Foo> weakRef = ref;
		ref = nullptr;

		REQUIRE(weakRef.IsExpired());
	}
}