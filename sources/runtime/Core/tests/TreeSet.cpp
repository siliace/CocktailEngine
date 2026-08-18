#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/TreeSet.hpp>

namespace
{
    /**
     * \brief Shorthand for a set parameterized only by its allocator
     */
    template <typename TAllocator>
    using IntSet = Ck::TreeSet<int, std::less<int>, TAllocator>;

    /**
     * \brief Collects the elements of a set in iteration order
     */
    template <typename TSet>
    Ck::Array<typename TSet::ElementType> Collect(const TSet& set)
    {
        Ck::Array<typename TSet::ElementType> elements;
        for (const auto& element : set)
            elements.Add(element);

        return elements;
    }

    /**
     * \brief Tells whether a range is strictly increasing
     */
    template <typename T>
    bool IsStrictlySorted(const Ck::Array<T>& values)
    {
        return std::adjacent_find(values.GetIterator(), values.GetEndIterator(), std::greater_equal<T>()) == values.GetEndIterator();
    }
}

TEMPLATE_TEST_CASE("Create empty TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set;

    REQUIRE(set.IsEmpty());
    REQUIRE(set.GetSize() == 0);
    REQUIRE(set.GetIterator() == set.GetEndIterator());
}

TEMPLATE_TEST_CASE("Create TreeSet from initializer list", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 3, 1, 2, 1, 3 };

    // Duplicates must be silently dropped
    REQUIRE(set.GetSize() == 3);
    REQUIRE(Collect(set) == Ck::Array<int>{ 1, 2, 3 });
}

TEMPLATE_TEST_CASE("Copy TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> source = { 30, 10, 20 };

    SECTION("By constructor")
    {
        IntSet<TestType> copy(source);

        REQUIRE(Collect(copy) == Ck::Array<int>{ 10, 20, 30 });

        copy.Remove(20);
        REQUIRE(source.Contains(20));
    }

    SECTION("By assignment")
    {
        IntSet<TestType> copy = { 99 };
        copy = source;

        REQUIRE(Collect(copy) == Ck::Array<int>{ 10, 20, 30 });
        REQUIRE_FALSE(copy.Contains(99));
    }
}

TEMPLATE_TEST_CASE("Move TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> source = { 1, 2, 3 };

    SECTION("By constructor")
    {
        IntSet<TestType> moved(Move(source));

        REQUIRE(source.IsEmpty());
        REQUIRE(Collect(moved) == Ck::Array<int>{ 1, 2, 3 });
    }

    SECTION("By assignment")
    {
        IntSet<TestType> moved = { 42 };
        moved = Move(source);

        REQUIRE(source.IsEmpty());
        REQUIRE(Collect(moved) == Ck::Array<int>{ 1, 2, 3 });
    }
}

TEMPLATE_TEST_CASE("Add elements to TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set;

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

TEMPLATE_TEST_CASE("Emplace element into TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::TreeSet<Ck::String, std::less<Ck::String>, TestType> set;

    REQUIRE(set.Emplace(CK_TEXT("hello")));
    REQUIRE(set.GetSize() == 1);

    REQUIRE_FALSE(set.Emplace(CK_TEXT("hello")));
    REQUIRE(set.GetSize() == 1);

    REQUIRE(set.Emplace(CK_TEXT("world")));
    REQUIRE(set.GetSize() == 2);

    REQUIRE(Collect(set) == Ck::Array<Ck::String>{ Ck::String(CK_TEXT("hello")), Ck::String(CK_TEXT("world")) });
}

TEST_CASE("TreeSet orders strings lexicographically", "[TreeSet]")
{
    Ck::TreeSet<Ck::String> set = {
        Ck::String(CK_TEXT("pear")),
        Ck::String(CK_TEXT("apple")),
        Ck::String(CK_TEXT("app")),
        Ck::String(CK_TEXT("banana")),
    };

    // A prefix orders before the longer string it is a prefix of
    REQUIRE(Collect(set) == Ck::Array<Ck::String>{
                                Ck::String(CK_TEXT("app")),
                                Ck::String(CK_TEXT("apple")),
                                Ck::String(CK_TEXT("banana")),
                                Ck::String(CK_TEXT("pear")),
                            });

    REQUIRE(set.GetFirst().Get() == CK_TEXT("app"));
    REQUIRE(set.GetLast().Get() == CK_TEXT("pear"));
    REQUIRE(set.Ceiling(Ck::String(CK_TEXT("b"))).Get() == CK_TEXT("banana"));
    REQUIRE(set.Floor(Ck::String(CK_TEXT("c"))).Get() == CK_TEXT("banana"));

    REQUIRE(set.Remove(Ck::String(CK_TEXT("apple"))));
    REQUIRE_FALSE(set.Contains(Ck::String(CK_TEXT("apple"))));
    REQUIRE(set.Contains(Ck::String(CK_TEXT("app"))));
}

TEMPLATE_TEST_CASE("Contains in TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 1, 2, 3 };

    REQUIRE(set.Contains(1));
    REQUIRE(set.Contains(2));
    REQUIRE(set.Contains(3));
    REQUIRE_FALSE(set.Contains(99));
}

TEMPLATE_TEST_CASE("Query TreeSet by order", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 10, 20, 30, 40 };

    SECTION("First and last element")
    {
        REQUIRE(set.GetFirst().Get() == 10);
        REQUIRE(set.GetLast().Get() == 40);

        IntSet<TestType> empty;
        REQUIRE(empty.GetFirst().IsEmpty());
        REQUIRE(empty.GetLast().IsEmpty());
    }

    SECTION("Floor returns the greatest element at or below")
    {
        REQUIRE(set.Floor(30).Get() == 30);
        REQUIRE(set.Floor(35).Get() == 30);
        REQUIRE(set.Floor(9).IsEmpty());
    }

    SECTION("Ceiling returns the smallest element at or above")
    {
        REQUIRE(set.Ceiling(30).Get() == 30);
        REQUIRE(set.Ceiling(25).Get() == 30);
        REQUIRE(set.Ceiling(41).IsEmpty());
    }

    SECTION("LowerBound and UpperBound straddle an existing element")
    {
        REQUIRE(*set.LowerBound(20) == 20);
        REQUIRE(*set.UpperBound(20) == 30);
        REQUIRE(set.UpperBound(40) == set.GetEndIterator());
    }

    SECTION("GetBounds walks an inclusive range")
    {
        const auto [begin, end] = set.GetBounds(20, 30);

        Ck::Array<int> elements;
        for (auto it = begin; it != end; ++it)
            elements.Add(*it);

        REQUIRE(elements == Ck::Array<int>{ 20, 30 });
    }
}

TEMPLATE_TEST_CASE("Remove elements from TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 50, 25, 75, 10, 30, 60, 90 };

    SECTION("Remove an existing element")
    {
        REQUIRE(set.Remove(25));
        REQUIRE(set.GetSize() == 6);
        REQUIRE_FALSE(set.Contains(25));
    }

    SECTION("Remove a missing element")
    {
        REQUIRE_FALSE(set.Remove(99));
        REQUIRE(set.GetSize() == 7);
    }

    SECTION("Remove by iterator returns the following element")
    {
        auto next = set.Remove(set.LowerBound(30));

        REQUIRE(next.IsValid());
        REQUIRE(*next == 50);
    }

    SECTION("RemoveFirst and RemoveLast peel the extremes")
    {
        REQUIRE(set.RemoveFirst());
        REQUIRE(set.GetFirst().Get() == 25);

        REQUIRE(set.RemoveLast());
        REQUIRE(set.GetLast().Get() == 75);

        IntSet<TestType> empty;
        REQUIRE_FALSE(empty.RemoveFirst());
        REQUIRE_FALSE(empty.RemoveLast());
    }
}

TEMPLATE_TEST_CASE("Clear TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 1, 2, 3, 4, 5 };

    set.Clear();

    REQUIRE(set.IsEmpty());
    REQUIRE(set.GetSize() == 0);

    set.Add(7);
    REQUIRE(Collect(set) == Ck::Array<int>{ 7 });
}

TEMPLATE_TEST_CASE("Iterate over TreeSet", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> set = { 3, 1, 5, 2, 4 };

    SECTION("Range-for visits elements in ascending order")
    {
        Ck::Array<int> elements;
        int sum = 0;
        for (int element : set)
        {
            elements.Add(element);
            sum += element;
        }

        REQUIRE(elements == Ck::Array<int>{ 1, 2, 3, 4, 5 });
        REQUIRE(sum == 15);
    }

    SECTION("Range-for over a const set")
    {
        const IntSet<TestType>& constSet = set;

        REQUIRE(Collect(constSet) == Ck::Array<int>{ 1, 2, 3, 4, 5 });
    }

    SECTION("Walking backwards from the end mirrors the forward order")
    {
        Ck::Array<int> elements;
        for (auto it = set.GetEndIterator(); it != set.GetIterator();)
        {
            --it;
            elements.Add(*it);
        }

        REQUIRE(elements == Ck::Array<int>{ 5, 4, 3, 2, 1 });
    }

    SECTION("GetLastIterator points at the greatest element")
    {
        REQUIRE(*set.GetLastIterator() == 5);
    }

    SECTION("ForEach visits every element")
    {
        int sum = 0;
        set.ForEach([&sum](int element) {
            sum += element;
        });

        REQUIRE(sum == 15);
    }
}

TEMPLATE_TEST_CASE("TreeSet Union", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> a = { 1, 2, 3 };
    IntSet<TestType> b = { 3, 4, 5 };

    auto result = a.Union(b);

    REQUIRE(Collect(result) == Ck::Array<int>{ 1, 2, 3, 4, 5 });
}

TEMPLATE_TEST_CASE("TreeSet Intersection", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> a = { 1, 2, 3, 4 };
    IntSet<TestType> b = { 3, 4, 5, 6 };

    auto result = a.Intersection(b);

    REQUIRE(Collect(result) == Ck::Array<int>{ 3, 4 });
}

TEMPLATE_TEST_CASE("TreeSet Difference", "[TreeSet]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntSet<TestType> a = { 1, 2, 3, 4 };
    IntSet<TestType> b = { 3, 4, 5 };

    auto result = a.Difference(b);

    REQUIRE(Collect(result) == Ck::Array<int>{ 1, 2 });
}

TEST_CASE("TreeSet honours a custom comparator", "[TreeSet]")
{
    Ck::TreeSet<int, std::greater<int>> set = { 1, 5, 3 };

    REQUIRE(Collect(set) == Ck::Array<int>{ 5, 3, 1 });
    REQUIRE(set.GetFirst().Get() == 5);
    REQUIRE(set.GetLast().Get() == 1);

    // Floor and Ceiling follow the comparator, not the numeric value: with
    // std::greater, "ordering before" means being numerically greater
    REQUIRE(set.Floor(4).Get() == 5);
    REQUIRE(set.Ceiling(4).Get() == 3);
    REQUIRE(set.Ceiling(0).IsEmpty());
}

TEST_CASE("TreeSet stays balanced and ordered under randomized traffic", "[TreeSet]")
{
    constexpr int Count = 2000;

    Ck::Array<int> elements(Count);
    std::iota(elements.GetIterator(), elements.GetEndIterator(), 0);

    std::mt19937 random(20261012);
    std::shuffle(elements.GetIterator(), elements.GetEndIterator(), random);

    Ck::TreeSet<int> set;

    SECTION("Insertions in random order")
    {
        for (int element : elements)
            REQUIRE(set.Add(element));

        REQUIRE(set.GetSize() == Count);

        const Ck::Array<int> iterated = Collect(set);
        REQUIRE(iterated.GetSize() == Count);
        REQUIRE(IsStrictlySorted(iterated));
    }

    SECTION("Removals in random order drain the set")
    {
        for (int element : elements)
            set.Add(element);

        Ck::Array<int> removalOrder = elements;
        std::shuffle(removalOrder.GetIterator(), removalOrder.GetEndIterator(), random);

        int remaining = Count;
        for (int element : removalOrder)
        {
            REQUIRE(set.Remove(element));
            --remaining;

            if (remaining % 97 == 0)
            {
                REQUIRE(set.GetSize() == static_cast<unsigned int>(remaining));
                REQUIRE(IsStrictlySorted(Collect(set)));
            }
        }

        REQUIRE(set.IsEmpty());
    }

    SECTION("Peeling the extremes drains the set")
    {
        for (int element : elements)
            set.Add(element);

        for (int step = 0; step < Count / 2; step++)
        {
            REQUIRE(set.RemoveFirst());
            REQUIRE(set.RemoveLast());
        }

        REQUIRE(set.IsEmpty());
    }
}
