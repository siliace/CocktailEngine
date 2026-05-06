#include <catch2/catch_all.hpp>

#include <Cocktail/Core/HashMap.hpp>

TEST_CASE("Put elements", "[HashMap]")
{
    Ck::HashMap<int, int> hashMap;

    SECTION("Put twice the same key")
    {
        hashMap.Put(1, 2);
        hashMap.Put(1, 4);

        REQUIRE(hashMap.GetSize() == 1);
    }

    SECTION("Put two different keys")
    {
        hashMap.Put(1, 2);
        hashMap.Put(2, 4);

        REQUIRE(hashMap.GetSize() == 2);
    }

    SECTION("Put element only if key is missing")
    {
        REQUIRE(hashMap.PutIfMissing(1, 2));
        REQUIRE(hashMap.GetSize() == 1);

        REQUIRE_FALSE(hashMap.PutIfMissing(1, 2));
        REQUIRE(hashMap.GetSize() == 1);
    }
}

TEST_CASE("Replace elements", "[HashMap]")
{
    Ck::HashMap<int, int> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);

    SECTION("with an existing key")
    {
        REQUIRE(hashMap.Replace(1, 3));
    }

    SECTION("with an invalid key")
    {
        REQUIRE_FALSE(hashMap.Replace(3, 3));
    }
}

TEST_CASE("Remove elements", "[HashMap]")
{
    Ck::HashMap<int, int> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);

    SECTION("With an existing key")
    {
        REQUIRE(hashMap.Remove(1));
        REQUIRE(hashMap.GetSize() == 1);
    }
}

TEST_CASE("Get elements", "[HashMap]")
{
    Ck::HashMap<int, int> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);
    hashMap.Put(3, 6);

    SECTION("with a missing key")
    {
        REQUIRE(hashMap.TryGet(4).IsEmpty());
    }

    SECTION("with a valid key")
    {
        REQUIRE(hashMap.TryGet(2).Get() == 4);
    }
}

TEST_CASE("Get all values", "[HashMap]")
{
    Ck::HashMap<int, int> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);
    hashMap.Put(3, 6);

    Ck::Array<int> values = hashMap.GetValues();
    REQUIRE(values.GetSize() == 3);
    REQUIRE(values.Contains(2));
    REQUIRE(values.Contains(4));
    REQUIRE(values.Contains(6));
}
