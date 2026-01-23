#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>

TEMPLATE_TEST_CASE("Add elements to the array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array;

    array.Add(2);
    REQUIRE(array.GetSize() == 1);
    REQUIRE(array.GetCapacity() >= 1);

    array.Add(4);
    REQUIRE(array.GetSize() == 2);
    REQUIRE(array.GetCapacity() >= 2);

    array.Add(6);
    REQUIRE(array.GetSize() == 3);
    REQUIRE(array.GetCapacity() >= 3);

    REQUIRE(array == Ck::Array<int, TestType>{ 2, 4, 6 });
}

TEMPLATE_TEST_CASE("Add elements in the middle of an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 1, 3, 5, 7 };

    array.AddAt(1, 2);
    REQUIRE(array == Ck::Array<int, TestType>{ 1, 2, 3, 5, 7 });

    array.AddAt(3, 4);
    REQUIRE(array == Ck::Array<int, TestType>{ 1, 2, 3, 4, 5, 7 });

    array.AddAt(5, 6);
    REQUIRE(array == Ck::Array<int, TestType>{ 1, 2, 3, 4, 5, 6, 7 });
}

TEMPLATE_TEST_CASE("Prepend elements at the begining of an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("Insert another array")
    {
        array.Prepend(Ck::Array<float, TestType>{ 0.f, 1.f, 2, 3, 4 });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }

    SECTION("Insert a initialize list")
    {
        array.Prepend({ 0, 1, 2, 3, 4 });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }
}

TEMPLATE_TEST_CASE("Insert elements in the middle of an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 10, 11, 12, 13, 14, 15 };

    SECTION("Insert another array")
    {
        array.Insert(4, Ck::Array<float, TestType>{ 4.f, 5.f, 6.f, 7.f, 8.f, 9.f });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }

    SECTION("Insert a initialize list")
    {
        array.Insert(4, { 4, 5, 6, 7, 8, 9 });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }
}

TEMPLATE_TEST_CASE("Append elements at the end of an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    SECTION("Insert another array")
    {
        array.Append(Ck::Array<float, TestType>{ 11.f, 12.f, 13.f, 14.f, 15.f });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }

    SECTION("Insert a initialize list")
    {
        array.Append({ 11, 12, 13, 14, 15 });
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }
}

TEMPLATE_TEST_CASE("Get an element from the array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("from a valid index")
    {
        REQUIRE(array.At(3) == 3);
    }

    SECTION("from an invalid index")
    {
        REQUIRE_THROWS(array.At(22));
    }
}

TEMPLATE_TEST_CASE("Try to get an element from the array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("from a valid index")
    {
        Ck::Optional<int&> value = array.TryAt(3);
        REQUIRE_FALSE(value.IsEmpty());
        REQUIRE(value.Get() == 3);
    }

    SECTION("from an invalid index")
    {
        REQUIRE(array.TryAt(22).IsEmpty());
    }
}

TEMPLATE_TEST_CASE("Test whether an array contains a value", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    REQUIRE(array.Contains(5));
    REQUIRE_FALSE(array.Contains(88));
}

TEMPLATE_TEST_CASE("Test whether an array contains a value matching a predicate", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>,
                   Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    REQUIRE(array.ContainsIf([](int value) {
        return value == 5;
        }));

    REQUIRE_FALSE(array.ContainsIf([](int value) {
        return value == 88;
        }));
}

TEMPLATE_TEST_CASE("Find the index of a value", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 3, 2, 1, 0 };

    SECTION("from the beginning")
    {
        Ck::Optional<typename TestType::SizeType> index = array.FindIndex(1);
        REQUIRE_FALSE(index.IsEmpty());
        REQUIRE(index.Get() == 1);
    }

    SECTION("from the end")
    {
        Ck::Optional<typename TestType::SizeType> index = array.FindLastIndex(1);
        REQUIRE_FALSE(index.IsEmpty());
        REQUIRE(index.Get() == 6);
    }

    SECTION("missing from the array")
    {
        Ck::Optional<typename TestType::SizeType> index = array.FindLastIndex(51);
        REQUIRE(index.IsEmpty());
    }
}

TEMPLATE_TEST_CASE("Test whether an array contains at last one value matching a predicate", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>,
                   Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    REQUIRE(array.AnyOf([](int value) {
        return value == 5;
        }));

    REQUIRE_FALSE(array.AnyOf([](int value) {
        return value == 88;
        }));
}

TEMPLATE_TEST_CASE("Test whether an array contains values matching a predicate", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>,
                   Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    REQUIRE(array.AllOf([](int value) {
        return value < 16;
        }));

    REQUIRE_FALSE(array.AllOf([](int value) {
        return value > 10;
        }));
}

TEMPLATE_TEST_CASE("Pop the first element of the array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array;

    SECTION("from an empty array")
    {
        REQUIRE_THROWS_AS(array.PopFirst(), Ck::ContainerEmpty);
    }

    SECTION("from a filled array")
    {
        array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

        int value = array.PopFirst();
        REQUIRE(value == 0);
        REQUIRE(array == Ck::Array<int, TestType>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    }
}

TEMPLATE_TEST_CASE("Pop the last element of the array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array;

    SECTION("from an empty array")
    {
        REQUIRE_THROWS_AS(array.PopLast(), Ck::ContainerEmpty);
    }

    SECTION("from a filled array")
    {
        array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

        int value = array.PopLast();
        REQUIRE(value == 15);
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 });
    }
}

TEMPLATE_TEST_CASE("Filter an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("To another array")
    {
        Ck::Array<int, TestType> results = array.Filter([](int i) {
            return i % 2 != 0;
        });

        REQUIRE(results.GetSize() == 8);
        REQUIRE(results == Ck::Array<int, TestType>{0, 2, 4, 6, 8, 10, 12, 14});
    }

    SECTION("In place")
    {
        array.FilterInPlace([](int i) {
            return i % 2 != 0;
        });

        REQUIRE(array.GetSize() == 8);
        REQUIRE(array == Ck::Array<int, TestType>{0, 2, 4, 6, 8, 10, 12, 14});
    }
}

TEMPLATE_TEST_CASE("Reduce an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 1, 2, 3, 4 };
    int sum = array.Reduce(0, [](int current, int value) {
        return current + value;
    });

    REQUIRE(sum == 10);
}

TEMPLATE_TEST_CASE("Reverse an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("To another array")
    {
        Ck::Array<int, TestType> reversed = array.Reverse();
        REQUIRE(reversed == Ck::Array<int, TestType>{ 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
    }

    SECTION("In place")
    {
        array.ReverseInPlace();
        REQUIRE(array == Ck::Array<int, TestType>{ 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 });
    }
}

TEMPLATE_TEST_CASE("Splice an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("To another array")
    {
        Ck::Array<int, TestType> sliced = array.Splice(3);

        REQUIRE(sliced.GetSize() == 3);
        REQUIRE(sliced == Ck::Array<int, TestType>{ 0, 1, 2 });
    }

    SECTION("In place")
    {
        array.SpliceInPlace(3);

        REQUIRE(array.GetSize() == 3);
        REQUIRE(array == Ck::Array<int, TestType>{ 0, 1, 2 });
    }
}

TEMPLATE_TEST_CASE("Splice an array with range", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("To another array")
    {
        Ck::Array<int, TestType> sliced = array.Splice(3, 5);

        REQUIRE(sliced.GetSize() == 11);
        REQUIRE(sliced == Ck::Array<int, TestType>{0, 1, 2, 8, 9, 10, 11, 12, 13, 14, 15});
    }

    SECTION("In place")
    {
        array.SpliceInPlace(3, 5);

        REQUIRE(array.GetSize() == 11);
        REQUIRE(array == Ck::Array<int, TestType>{0, 1, 2, 8, 9, 10, 11, 12, 13, 14, 15});
    }
}

TEMPLATE_TEST_CASE("Resize an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array;

    array.Resize(10);

    REQUIRE(array.GetSize() == 10);
    REQUIRE(array.GetCapacity() >= 10);

    SECTION("to a smaller capacity")
    {
        array.Resize(5);

        REQUIRE(array.GetSize() == 5);
    }

    SECTION("to a bigger capacity")
    {
        array.Resize(15);

        REQUIRE(array.GetSize() == 15);
        REQUIRE(array.GetCapacity() >= 15);
    }
}

TEMPLATE_TEST_CASE("Reserve an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> arr;
    arr.Reserve(10);

    REQUIRE(arr.GetSize() == 0);
    REQUIRE(arr.GetCapacity() >= 10);
}

TEMPLATE_TEST_CASE("Clear and Shrink an array", "[Array]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::Array<int, TestType> array = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

    SECTION("Clear")
    {
        array.Clear();

        REQUIRE(array.GetSize() == 0);
        REQUIRE(array.GetCapacity() > 0);
    }

    SECTION("Clear and Shrink")
    {
        array.Clear();
        array.Shrink();

        REQUIRE(array.GetSize() == 0);
        REQUIRE(array.GetCapacity() == 0);
    }
}

TEST_CASE("Iterate over elements of an Array", "[Array]")
{
    Ck::Array<int> list;

    list.Add(1);
    list.Add(2);
    list.Add(3);
    list.Add(4);
    list.Add(5);

    SECTION("In normal order")
    {
        int previous = 0;
        for (Ck::Array<int>::Iterator it = list.GetIterator(); it.IsValid(); it = it.Next())
        {
            REQUIRE(previous + 1 == *it);

            previous = *it;
        }
    }

    SECTION("In revered order")
    {
        int previous = 6;
        for (Ck::Array<int>::Iterator it = list.GetLastIterator(); it.IsValid(); it = it.Previous())
        {
            REQUIRE(previous - 1 == *it);

            previous = *it;
        }
    }
}
