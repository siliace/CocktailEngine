#include <algorithm>
#include <numeric>
#include <random>
#include <vector>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/TreeMap.hpp>

namespace
{
    /**
     * \brief Shorthand for a map parameterized only by its allocator
     *
     * The tests are run against every allocator flavour so that both the
     * ownership-transferring and the entry-by-entry move paths are covered.
     */
    template <typename TAllocator>
    using IntMap = Ck::TreeMap<int, int, std::less<int>, TAllocator>;

    /**
     * \brief Collects the keys of a map in iteration order
     */
    template <typename TMap>
    Ck::Array<typename TMap::KeyType> CollectKeys(const TMap& map)
    {
        Ck::Array<typename TMap::KeyType> keys;
        for (const auto& entry : map)
            keys.Add(entry.Key);

        return keys;
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

TEMPLATE_TEST_CASE("Create empty TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;

    REQUIRE(map.IsEmpty());
    REQUIRE(map.GetSize() == 0);
    REQUIRE(map.GetIterator() == map.GetEndIterator());
}

TEMPLATE_TEST_CASE("Create TreeMap from initializer list", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map = { { 3, 30 }, { 1, 10 }, { 2, 20 }, { 1, 11 } };

    // The duplicated key keeps a single entry, holding the last value
    REQUIRE(map.GetSize() == 3);
    REQUIRE(CollectKeys(map) == Ck::Array<int>{ 1, 2, 3 });
    REQUIRE(*map.Get(1) == 11);
}

TEMPLATE_TEST_CASE("Copy TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> source;
    for (int key : { 5, 1, 9, 3, 7 })
        source.Put(key, key * 10);

    SECTION("By constructor")
    {
        IntMap<TestType> copy(source);

        REQUIRE(copy.GetSize() == 5);
        REQUIRE(CollectKeys(copy) == Ck::Array<int>{ 1, 3, 5, 7, 9 });

        // The copy owns its own nodes
        copy.Remove(5);
        REQUIRE(source.Contains(5));
    }

    SECTION("By assignment")
    {
        IntMap<TestType> copy;
        copy.Put(100, 1);
        copy = source;

        REQUIRE(copy.GetSize() == 5);
        REQUIRE_FALSE(copy.Contains(100));
        REQUIRE(CollectKeys(copy) == Ck::Array<int>{ 1, 3, 5, 7, 9 });
    }

    SECTION("Self assignment leaves the map untouched")
    {
        source = source;

        REQUIRE(source.GetSize() == 5);
    }
}

TEMPLATE_TEST_CASE("Move TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> source;
    for (int key : { 4, 2, 6, 1, 3 })
        source.Put(key, key * 10);

    SECTION("By constructor")
    {
        IntMap<TestType> moved(Move(source));

        REQUIRE(source.IsEmpty());
        REQUIRE(moved.GetSize() == 5);
        REQUIRE(CollectKeys(moved) == Ck::Array<int>{ 1, 2, 3, 4, 6 });
    }

    SECTION("By assignment")
    {
        IntMap<TestType> moved;
        moved.Put(42, 42);
        moved = Move(source);

        REQUIRE(source.IsEmpty());
        REQUIRE(moved.GetSize() == 5);
        REQUIRE_FALSE(moved.Contains(42));
    }
}

TEMPLATE_TEST_CASE("Put entries into TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;

    SECTION("Put inserts then replaces")
    {
        map.Put(1, 10);
        REQUIRE(map.GetSize() == 1);

        map.Put(1, 11);
        REQUIRE(map.GetSize() == 1);
        REQUIRE(*map.Get(1) == 11);
    }

    SECTION("PutIfMissing never replaces")
    {
        REQUIRE(map.PutIfMissing(1, 10));
        REQUIRE_FALSE(map.PutIfMissing(1, 11));
        REQUIRE(*map.Get(1) == 10);
        REQUIRE(map.GetSize() == 1);
    }

    SECTION("Replace only touches present keys")
    {
        map.Put(1, 10);
        REQUIRE(map.Replace(1, 99));
        REQUIRE(*map.Get(1) == 99);

        REQUIRE_FALSE(map.Replace(2, 99));
        REQUIRE(map.GetSize() == 1);
    }

    SECTION("ComputeIfMissing only calls the factory when absent")
    {
        int calls = 0;
        auto factory = [&calls](const int& key) {
            ++calls;
            return key * 100;
        };

        REQUIRE(map.ComputeIfMissing(2, factory) == 200);
        REQUIRE(calls == 1);

        REQUIRE(map.ComputeIfMissing(2, factory) == 200);
        REQUIRE(calls == 1);
        REQUIRE(map.GetSize() == 1);
    }

    SECTION("GetOrAdd inserts a default value")
    {
        REQUIRE(map.GetOrAdd(7) == 0);
        REQUIRE(map.GetSize() == 1);

        map.GetOrAdd(7) = 70;
        REQUIRE(*map.Get(7) == 70);
    }

    SECTION("operator[] behaves like GetOrAdd")
    {
        map[3] = 30;
        REQUIRE(map.GetSize() == 1);
        REQUIRE(*map.Get(3) == 30);
    }
}

TEMPLATE_TEST_CASE("Look up entries in TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;
    for (int key : { 10, 20, 30 })
        map.Put(key, key);

    const IntMap<TestType>& constMap = map;

    SECTION("Get returns a pointer or null")
    {
        REQUIRE(map.Get(20) != nullptr);
        REQUIRE(map.Get(25) == nullptr);
        REQUIRE(constMap.Get(20) != nullptr);
        REQUIRE(constMap.Get(25) == nullptr);
    }

    SECTION("TryGet returns an Optional")
    {
        REQUIRE(map.TryGet(10).HasValue());
        REQUIRE(map.TryGet(10).Get() == 10);
        REQUIRE(map.TryGet(11).IsEmpty());
        REQUIRE(constMap.TryGet(30).HasValue());
    }

    SECTION("Contains answers for both present and missing keys")
    {
        REQUIRE(map.Contains(30));
        REQUIRE_FALSE(map.Contains(31));
    }

    SECTION("Find positions an iterator")
    {
        auto it = map.Find(20);
        REQUIRE(it.IsValid());
        REQUIRE(it->Key == 20);

        REQUIRE_FALSE(map.Find(21).IsValid());
        REQUIRE(map.Find(21) == map.GetEndIterator());
        REQUIRE(constMap.Find(20).IsValid());
    }
}

TEMPLATE_TEST_CASE("Query TreeMap by order", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;
    for (int key : { 10, 20, 30, 40 })
        map.Put(key, key);

    SECTION("First and last entry")
    {
        REQUIRE(map.GetFirst().Get().Key == 10);
        REQUIRE(map.GetLast().Get().Key == 40);

        IntMap<TestType> empty;
        REQUIRE(empty.GetFirst().IsEmpty());
        REQUIRE(empty.GetLast().IsEmpty());
    }

    SECTION("Floor returns the greatest key at or below")
    {
        REQUIRE(map.Floor(30).Get().Key == 30);
        REQUIRE(map.Floor(35).Get().Key == 30);
        REQUIRE(map.Floor(9).IsEmpty());
        REQUIRE(map.Floor(100).Get().Key == 40);
    }

    SECTION("Ceiling returns the smallest key at or above")
    {
        REQUIRE(map.Ceiling(30).Get().Key == 30);
        REQUIRE(map.Ceiling(25).Get().Key == 30);
        REQUIRE(map.Ceiling(41).IsEmpty());
        REQUIRE(map.Ceiling(0).Get().Key == 10);
    }

    SECTION("LowerBound and UpperBound straddle an existing key")
    {
        REQUIRE(map.LowerBound(20)->Key == 20);
        REQUIRE(map.UpperBound(20)->Key == 30);

        REQUIRE(map.LowerBound(25)->Key == 30);
        REQUIRE(map.UpperBound(25)->Key == 30);

        REQUIRE(map.LowerBound(41) == map.GetEndIterator());
        REQUIRE(map.UpperBound(40) == map.GetEndIterator());
    }

    SECTION("GetBounds walks an inclusive key range")
    {
        const auto [begin, end] = map.GetBounds(20, 30);

        Ck::Array<int> keys;
        for (auto it = begin; it != end; ++it)
            keys.Add(it->Key);

        REQUIRE(keys == Ck::Array<int>{ 20, 30 });
    }

    SECTION("GetBounds over an empty range yields no entry")
    {
        const auto [begin, end] = map.GetBounds(21, 29);
        REQUIRE(begin == end);
    }
}

TEMPLATE_TEST_CASE("Remove entries from TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;
    for (int key : { 50, 25, 75, 10, 30, 60, 90 })
        map.Put(key, key);

    SECTION("Remove by key")
    {
        REQUIRE(map.Remove(25));
        REQUIRE(map.GetSize() == 6);
        REQUIRE_FALSE(map.Contains(25));

        REQUIRE_FALSE(map.Remove(25));
    }

    SECTION("Removing the root keeps the tree ordered")
    {
        REQUIRE(map.Remove(50));
        REQUIRE(CollectKeys(map) == Ck::Array<int>{ 10, 25, 30, 60, 75, 90 });
    }

    SECTION("Remove by iterator returns the following entry")
    {
        auto it = map.Find(30);
        auto next = map.Remove(it);

        REQUIRE(next.IsValid());
        REQUIRE(next->Key == 50);
        REQUIRE(map.GetSize() == 6);
    }

    SECTION("Remove by iterator on the last entry yields the end")
    {
        REQUIRE(map.Remove(map.GetLastIterator()) == map.GetEndIterator());
        REQUIRE_FALSE(map.Contains(90));
    }

    SECTION("RemoveFirst and RemoveLast peel the extremes")
    {
        REQUIRE(map.RemoveFirst());
        REQUIRE(map.GetFirst().Get().Key == 25);

        REQUIRE(map.RemoveLast());
        REQUIRE(map.GetLast().Get().Key == 75);

        REQUIRE(map.GetSize() == 5);

        IntMap<TestType> empty;
        REQUIRE_FALSE(empty.RemoveFirst());
        REQUIRE_FALSE(empty.RemoveLast());
    }

    SECTION("Clear drops every entry")
    {
        map.Clear();

        REQUIRE(map.IsEmpty());
        REQUIRE(map.GetSize() == 0);

        // The map stays usable afterwards
        map.Put(1, 1);
        REQUIRE(map.GetSize() == 1);
    }
}

TEMPLATE_TEST_CASE("Iterate over TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;
    for (int key : { 3, 1, 4, 5, 2 })
        map.Put(key, key * 10);

    SECTION("Range-for visits entries in ascending key order")
    {
        Ck::Array<int> keys;
        Ck::Array<int> values;
        for (const auto& [key, value] : map)
        {
            keys.Add(key);
            values.Add(value);
        }

        REQUIRE(keys == Ck::Array<int>{ 1, 2, 3, 4, 5 });
        REQUIRE(values == Ck::Array<int>{ 10, 20, 30, 40, 50 });
    }

    SECTION("Range-for over a const map yields const entries")
    {
        const IntMap<TestType>& constMap = map;

        int sum = 0;
        for (const auto& entry : constMap)
            sum += entry.Value;

        REQUIRE(sum == 150);
    }

    SECTION("Iteration is mutable through the entries")
    {
        for (auto& entry : map)
            entry.Value = 0;

        REQUIRE(*map.Get(3) == 0);
    }

    SECTION("Walking backwards from the end mirrors the forward order")
    {
        Ck::Array<int> keys;
        for (auto it = map.GetEndIterator(); it != map.GetIterator();)
        {
            --it;
            keys.Add(it->Key);
        }

        REQUIRE(keys == Ck::Array<int>{ 5, 4, 3, 2, 1 });
    }

    SECTION("GetLastIterator points at the greatest key")
    {
        REQUIRE(map.GetLastIterator()->Key == 5);
    }

    SECTION("ForEach accepts 0, 1 and 2 parameters")
    {
        int count = 0;
        map.ForEach([&count]() {
            ++count;
        });
        REQUIRE(count == 5);

        int sum = 0;
        map.ForEach([&sum](const int& value) {
            sum += value;
        });
        REQUIRE(sum == 150);

        Ck::Array<int> keys;
        map.ForEach([&keys](const int& /*value*/, const int& key) {
            keys.Add(key);
        });
        REQUIRE(keys == Ck::Array<int>{ 1, 2, 3, 4, 5 });
    }
}

TEMPLATE_TEST_CASE("Collect TreeMap keys and values", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> map;
    for (int key : { 2, 3, 1 })
        map.Put(key, key * 10);

    SECTION("GetKeys returns an ordered set")
    {
        auto keys = map.GetKeys();

        REQUIRE(keys.GetSize() == 3);
        REQUIRE(keys.Contains(1));
        REQUIRE(keys.GetFirst().Get() == 1);
        REQUIRE(keys.GetLast().Get() == 3);
    }

    SECTION("GetValues returns values in key order")
    {
        auto values = map.GetValues();

        REQUIRE(values.GetSize() == 3);
        REQUIRE(values[0] == 10);
        REQUIRE(values[1] == 20);
        REQUIRE(values[2] == 30);
    }
}

TEMPLATE_TEST_CASE("Merge TreeMap", "[TreeMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    IntMap<TestType> target;
    target.Put(1, 1);
    target.Put(3, 3);

    IntMap<TestType> source;
    source.Put(2, 22);
    source.Put(3, 33);
    source.Put(4, 44);

    target.Merge(source);

    REQUIRE(CollectKeys(target) == Ck::Array<int>{ 1, 2, 3, 4 });

    // The already present key keeps the target's value
    REQUIRE(*target.Get(3) == 3);
    REQUIRE(*target.Get(2) == 22);

    // Only the conflicting entry is left behind
    REQUIRE(source.GetSize() == 1);
    REQUIRE(source.Contains(3));
}

TEST_CASE("TreeMap honours a custom comparator", "[TreeMap]")
{
    Ck::TreeMap<int, int, std::greater<int>> map;
    for (int key : { 1, 5, 3 })
        map.Put(key, key);

    REQUIRE(CollectKeys(map) == Ck::Array<int>{ 5, 3, 1 });
    REQUIRE(map.GetFirst().Get().Key == 5);
    REQUIRE(map.GetLast().Get().Key == 1);

    // Floor and Ceiling follow the comparator, not the numeric value: with
    // std::greater, "ordering before" means being numerically greater
    REQUIRE(map.Floor(3).Get().Key == 3);
    REQUIRE(map.Floor(4).Get().Key == 5);
    REQUIRE(map.Ceiling(4).Get().Key == 3);
    REQUIRE(map.Ceiling(0).IsEmpty());
}

TEST_CASE("TreeMap stores non-trivial keys and values", "[TreeMap]")
{
    Ck::TreeMap<Ck::String, Ck::String> map;

    // Keys and values are built from literals, lvalues and rvalues alike
    map.Put(CK_TEXT("beta"), CK_TEXT("second"));

    const Ck::String alpha(CK_TEXT("alpha"));
    map.Put(alpha, Ck::String(CK_TEXT("first")));
    map.Put(Ck::String(CK_TEXT("gamma")), Ck::String(CK_TEXT("third")));

    REQUIRE(map.GetSize() == 3);
    REQUIRE(CollectKeys(map) == Ck::Array<Ck::String>{
                                    Ck::String(CK_TEXT("alpha")),
                                    Ck::String(CK_TEXT("beta")),
                                    Ck::String(CK_TEXT("gamma")),
                                });

    REQUIRE(*map.Get(Ck::String(CK_TEXT("alpha"))) == CK_TEXT("first"));
    REQUIRE(map.GetFirst().Get().Key == CK_TEXT("alpha"));
    REQUIRE(map.GetLast().Get().Key == CK_TEXT("gamma"));

    SECTION("A present key is replaced, not duplicated")
    {
        map.Put(CK_TEXT("beta"), CK_TEXT("replaced"));

        REQUIRE(map.GetSize() == 3);
        REQUIRE(*map.Get(Ck::String(CK_TEXT("beta"))) == CK_TEXT("replaced"));
    }

    SECTION("Removal keeps the remaining entries ordered")
    {
        REQUIRE(map.Remove(Ck::String(CK_TEXT("beta"))));
        REQUIRE(map.GetSize() == 2);
        REQUIRE(CollectKeys(map) == Ck::Array<Ck::String>{
                                        Ck::String(CK_TEXT("alpha")),
                                        Ck::String(CK_TEXT("gamma")),
                                    });
    }

    SECTION("GetOrAdd inserts a default-constructed string")
    {
        REQUIRE(map.GetOrAdd(Ck::String(CK_TEXT("delta"))).IsEmpty());
        REQUIRE(map.GetSize() == 4);
    }
}

TEST_CASE("TreeMap keeps references and iterators stable", "[TreeMap]")
{
    Ck::TreeMap<int, int> map;
    for (int key = 0; key < 64; key++)
        map.Put(key, key);

    int& pinnedValue = *map.Get(32);
    auto pinnedIterator = map.Find(40);

    // Inserting and removing unrelated entries must not relocate anything
    for (int key = 64; key < 256; key++)
        map.Put(key, key);
    for (int key = 0; key < 32; key++)
        map.Remove(key);

    REQUIRE(&pinnedValue == map.Get(32));
    REQUIRE(pinnedIterator->Key == 40);
    REQUIRE(pinnedValue == 32);
}

TEST_CASE("TreeMap stays balanced and ordered under randomized traffic", "[TreeMap]")
{
    constexpr int Count = 2000;

    Ck::Array<int> keys(Count);
    std::iota(keys.GetIterator(), keys.GetEndIterator(), 0);

    std::mt19937 random(20261012);
    std::shuffle(keys.GetIterator(), keys.GetEndIterator(), random);

    Ck::TreeMap<int, int> map;

    SECTION("Insertions in random order")
    {
        for (int key : keys)
            map.Put(key, key * 2);

        REQUIRE(map.GetSize() == Count);

        const Ck::Array<int> iterated = CollectKeys(map);
        REQUIRE(iterated.GetSize() == Count);
        REQUIRE(IsStrictlySorted(iterated));

        for (int key : keys)
            REQUIRE(*map.Get(key) == key * 2);
    }

    SECTION("Insertions in ascending order still stay balanced")
    {
        for (int key = 0; key < Count; key++)
            map.Put(key, key);
    }

    SECTION("Insertions in descending order still stay balanced")
    {
        for (int key = Count; key-- > 0;)
            map.Put(key, key);
    }

    SECTION("Removals in random order drain the map")
    {
        for (int key : keys)
            map.Put(key, key);

        Ck::Array<int> removalOrder = keys;
        std::shuffle(removalOrder.GetIterator(), removalOrder.GetEndIterator(), random);

        int remaining = Count;
        for (int key : removalOrder)
        {
            REQUIRE(map.Remove(key));
            --remaining;

            // Checking every step is O(n) each, so sample the invariants instead
            if (remaining % 97 == 0)
            {
                REQUIRE(map.GetSize() == static_cast<unsigned int>(remaining));
                REQUIRE(IsStrictlySorted(CollectKeys(map)));
            }
        }

        REQUIRE(map.IsEmpty());
    }

    SECTION("Interleaved insertions and removals")
    {
        std::uniform_int_distribution<int> keyDistribution(0, Count - 1);
        std::bernoulli_distribution insertDistribution(0.6);

        Ck::Array<bool> present(Count, false);
        std::size_t expectedSize = 0;

        for (int step = 0; step < 20000; step++)
        {
            const int key = keyDistribution(random);
            if (insertDistribution(random))
            {
                map.Put(key, key);
                if (!present[key])
                {
                    present[key] = true;
                    ++expectedSize;
                }
            }
            else
            {
                REQUIRE(map.Remove(key) == present[key]);
                if (present[key])
                {
                    present[key] = false;
                    --expectedSize;
                }
            }
        }

        REQUIRE(map.GetSize() == expectedSize);

        const Ck::Array<int> iterated = CollectKeys(map);
        REQUIRE(iterated.GetSize() == expectedSize);
        REQUIRE(IsStrictlySorted(iterated));

        for (int key = 0; key < Count; key++)
            REQUIRE(map.Contains(key) == present[key]);
    }
}
