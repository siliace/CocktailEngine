#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/StaticArray.hpp>

TEST_CASE("Create an array", "[StaticArray]")
{
    SECTION("not empty")
    {
        Ck::StaticArray<int, 10> array;

        REQUIRE(array.GetSize() == 10);
        REQUIRE_FALSE(array.IsEmpty());
    }

    SECTION("empty")
    {
        Ck::StaticArray<int, 0> array;

        REQUIRE(array.GetSize() == 0);
        REQUIRE(array.IsEmpty());
    }
}

TEST_CASE("Fill an array", "[StaticArray]")
{
    Ck::StaticArray<int, 10> array;
    array.Fill(42);

    array.ForEach([](int i) {
        REQUIRE(i == 42);
    });
}

TEST_CASE("Access to an element of an array", "[StaticArray]")
{
    SECTION("not empty")
    {
        Ck::StaticArray<int, 10> array;
        for (int i = 0; i < array.GetSize(); i++)
            array[i] = i;

        SECTION("With a good index")
        {
            REQUIRE(array.At(3) == 3);
            REQUIRE(array.TryAt(3).Get() == 3);
        }

        SECTION("With a bad index")
        {
            REQUIRE_THROWS_AS(array.At(42), Ck::ContainerOutOfRange);
            REQUIRE(array.TryAt(42).IsEmpty());
        }

        SECTION("At the first element")
        {
            REQUIRE(array.First() == 0);
            REQUIRE(array.TryFirst().Get() == 0);
        }

        SECTION("At the last element")
        {
            REQUIRE(array.Last() == 9);
            REQUIRE(array.TryLast().Get() == 9);
        }
    }

    SECTION("empty")
    {
        Ck::StaticArray<int, 0> array;

        REQUIRE_THROWS_AS(array.At(0), Ck::ContainerEmpty);
        REQUIRE(array.TryAt(0).IsEmpty());

        REQUIRE_THROWS_AS(array.First(), Ck::ContainerEmpty);
        REQUIRE(array.TryFirst().IsEmpty());

        REQUIRE_THROWS_AS(array.Last(), Ck::ContainerEmpty);
        REQUIRE(array.TryLast().IsEmpty());
    }
}

TEST_CASE("Check whether an array contains a value", "[StaticArray]")
{
    Ck::StaticArray<int, 4> odds = { 1, 3, 5, 7 };
    Ck::StaticArray<int, 4> evens = { 2, 4, 6, 8 };

    SECTION("By value")
    {
        REQUIRE(odds.Contains(1));
        REQUIRE_FALSE(odds.Contains(2));

        REQUIRE_FALSE(evens.Contains(1));
        REQUIRE(evens.Contains(2));
    }

    SECTION("By predicate")
    {
        REQUIRE(odds.ContainsIf([](int i) {
            return i % 2 != 0;
        }));

        REQUIRE_FALSE(odds.ContainsIf([](int i) {
            return i % 2 == 0;
        }));

        REQUIRE_FALSE(evens.ContainsIf([](int i) {
            return i % 2 != 0;
        }));

        REQUIRE(evens.ContainsIf([](int i) {
            return i % 2 == 0;
        }));
    }
}

TEST_CASE("Check whether any element of an array matches a predicate", "[StaticArray]")
{
    Ck::StaticArray<int, 4> odds = { 1, 3, 5, 7 };
    Ck::StaticArray<int, 4> evens = { 2, 4, 6, 8 };

    REQUIRE(odds.AnyOf([](int i) {
        return i == 3;
    }));

    REQUIRE_FALSE(odds.AnyOf([](int i) {
        return i == 4;
    }));

    REQUIRE_FALSE(evens.AnyOf([](int i) {
        return i == 3;
    }));

    REQUIRE(evens.AnyOf([](int i) {
        return i == 4;
    }));
}

TEST_CASE("Check whether all elements of an array matches a predicate", "[StaticArray]")
{
    Ck::StaticArray<int, 4> odds = { 1, 3, 5, 7 };
    Ck::StaticArray<int, 4> evens = { 2, 4, 6, 8 };

    REQUIRE(odds.AllOf([](int i) {
        return i % 2 != 0;
    }));

    REQUIRE_FALSE(odds.AnyOf([](int i) {
        return i % 2 == 0;
    }));

    REQUIRE_FALSE(evens.AllOf([](int i) {
        return i % 2 != 0;
    }));

    REQUIRE(evens.AnyOf([](int i) {
        return i % 2 == 0;
    }));
}

TEST_CASE("Transform an array", "[StaticArray]")
{
    Ck::StaticArray<int, 4> array = { 1, 3, 5, 7 };
    array = array.Map([](int i) {
        return i * 2;
    });

    REQUIRE(array == Ck::StaticArray<int, 4>{ 2, 6, 10, 14 });
}

TEST_CASE("Reverse an array", "[StaticArray]")
{
    Ck::StaticArray<int, 4> array = { 1, 3, 5, 7 };
    REQUIRE(array.Reverse() == Ck::StaticArray<int, 4>{ 7, 5, 3, 1 });
}
