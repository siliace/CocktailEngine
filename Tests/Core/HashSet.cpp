#include <catch2/catch_all.hpp>

#include <Cocktail/Core/HashSet.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>

TEMPLATE_TEST_CASE("Create empty HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set;

    REQUIRE(set.IsEmpty());
    REQUIRE(set.GetSize() == 0);
}

TEMPLATE_TEST_CASE("Create HashSet from initializer list", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set = { 1, 2, 3, 2, 1 };

    // Duplicates must be silently dropped
    REQUIRE(set.GetSize() == 3);
    REQUIRE(set.Contains(1));
    REQUIRE(set.Contains(2));
    REQUIRE(set.Contains(3));
}

TEMPLATE_TEST_CASE("Copy HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set1;
    set1.Add(10);
    set1.Add(20);
    set1.Add(30);

    SECTION("By constructor")
    {
        Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set2(set1);
        REQUIRE(set2.GetSize() == 3);
        REQUIRE(set2.Contains(10));
        REQUIRE(set2.Contains(20));
        REQUIRE(set2.Contains(30));
    }

    SECTION("By assignment")
    {
        Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set2;
        set2 = set1;
        REQUIRE(set2.GetSize() == 3);
        REQUIRE(set2.Contains(10));
    }
}

TEMPLATE_TEST_CASE("Move HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set1;
    set1.Add(1);
    set1.Add(2);
    set1.Add(3);

    SECTION("By constructor")
    {
        Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set2(std::move(set1));
        REQUIRE(set1.IsEmpty());
        REQUIRE(set2.GetSize() == 3);
    }

    SECTION("By assignment")
    {
        Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set2 = std::move(set1);
        REQUIRE(set1.IsEmpty());
        REQUIRE(set2.GetSize() == 3);
    }
}

TEMPLATE_TEST_CASE("Add elements to HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set;

    SECTION("Adding a new element returns true")
    {
        REQUIRE(set.Add(42));
        REQUIRE(set.GetSize() == 1);
    }

    SECTION("Adding a duplicate returns false")
    {
        set.Add(42);
        REQUIRE_FALSE(set.Add(42));
        REQUIRE(set.GetSize() == 1);
    }

    SECTION("Adding distinct elements grows the size")
    {
        set.Add(1);
        set.Add(2);
        set.Add(3);
        REQUIRE(set.GetSize() == 3);
    }
}

TEMPLATE_TEST_CASE("Emplace element into HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<std::string, std::hash<std::string>, std::equal_to<std::string>, TestType> set;

    REQUIRE(set.Emplace("hello"));
    REQUIRE(set.GetSize() == 1);

    REQUIRE_FALSE(set.Emplace("hello"));
    REQUIRE(set.GetSize() == 1);

    REQUIRE(set.Emplace("world"));
    REQUIRE(set.GetSize() == 2);
}

TEMPLATE_TEST_CASE("Contains in HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set;
    set.Add(1);
    set.Add(2);
    set.Add(3);

    REQUIRE(set.Contains(1));
    REQUIRE(set.Contains(2));
    REQUIRE(set.Contains(3));
    REQUIRE_FALSE(set.Contains(99));
}

TEMPLATE_TEST_CASE("Remove element from HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set;
    set.Add(1);
    set.Add(2);
    set.Add(3);

    SECTION("Remove an existing element")
    {
        REQUIRE(set.Remove(2));
        REQUIRE(set.GetSize() == 2);
        REQUIRE_FALSE(set.Contains(2));
    }

    SECTION("Remove a missing element")
    {
        REQUIRE_FALSE(set.Remove(99));
        REQUIRE(set.GetSize() == 3);
    }
}

TEMPLATE_TEST_CASE("Clear HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set = { 1, 2, 3, 4, 5 };

    set.Clear();

    REQUIRE(set.IsEmpty());
    REQUIRE(set.GetSize() == 0);
}

TEMPLATE_TEST_CASE("Reserve HashSet", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set;
    set.Reserve(64);

    // After reserve the set should handle at least 64 elements without rehashing
    for (int i = 0; i < 48; ++i)
        set.Add(i);

    REQUIRE(set.GetSize() == 48);
    REQUIRE(set.GetBucketCount() >= 64);
}

TEMPLATE_TEST_CASE("Iterate over HashSet with range-for", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set = { 1, 2, 3, 4, 5 };

    int count = 0;
    int sum   = 0;
    for (int v : set)
    {
        ++count;
        sum += v;
    }

    REQUIRE(count == 5);
    REQUIRE(sum == 15);
}

TEMPLATE_TEST_CASE("Iterate over HashSet with ForEach", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> set = { 10, 20, 30 };

    int sum = 0;
    set.ForEach([&sum](int v) { sum += v; });

    REQUIRE(sum == 60);
}

TEMPLATE_TEST_CASE("HashSet Union", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> a = { 1, 2, 3 };
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> b = { 3, 4, 5 };

    auto result = a.Union(b);

    REQUIRE(result.GetSize() == 5);
    REQUIRE(result.Contains(1));
    REQUIRE(result.Contains(2));
    REQUIRE(result.Contains(3));
    REQUIRE(result.Contains(4));
    REQUIRE(result.Contains(5));
}

TEMPLATE_TEST_CASE("HashSet Intersection", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> a = { 1, 2, 3, 4 };
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> b = { 3, 4, 5, 6 };

    auto result = a.Intersection(b);

    REQUIRE(result.GetSize() == 2);
    REQUIRE(result.Contains(3));
    REQUIRE(result.Contains(4));
}

TEMPLATE_TEST_CASE("HashSet Difference", "[HashSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> a = { 1, 2, 3, 4 };
    Ck::HashSet<int, std::hash<int>, std::equal_to<int>, TestType> b = { 3, 4, 5 };

    auto result = a.Difference(b);

    REQUIRE(result.GetSize() == 2);
    REQUIRE(result.Contains(1));
    REQUIRE(result.Contains(2));
    REQUIRE_FALSE(result.Contains(3));
    REQUIRE_FALSE(result.Contains(4));
}
