#include <catch2/catch_all.hpp>

#include <Cocktail/Core/HashMap.hpp>
#include <Cocktail/Core/Memory/Allocator/SizedLinearAllocator.hpp>

TEMPLATE_TEST_CASE("Create empty", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
    
    REQUIRE(hashMap.IsEmpty());
    REQUIRE(hashMap.GetSize() == 0);
}

TEMPLATE_TEST_CASE("Create by copy", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap1;
    hashMap1.Put(1, 2);
    hashMap1.Put(2, 4);
    hashMap1.Put(3, 6);
    
    SECTION("By constructor")
    {
        Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hastMap2(hashMap1);
    
        REQUIRE_FALSE(hastMap2.IsEmpty());
        REQUIRE(hastMap2.GetSize() == 3);

        typename Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType>::ConstIterator lhs = hashMap1.GetIterator();
        typename Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType>::ConstIterator rhs = hastMap2.GetIterator();
        while (lhs.IsValid() || rhs.IsValid())
        {
            REQUIRE(lhs.IsValid());
            REQUIRE(rhs.IsValid());
            REQUIRE(lhs.GetValue().Key == rhs.GetValue().Key);
            REQUIRE(lhs.GetValue().Value == rhs.GetValue().Value);
            
            lhs.Advance();
            rhs.Advance();
        }
    }
    
    SECTION("By assignation")
    {
        Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hastMap2 = hashMap1;
    
        REQUIRE_FALSE(hastMap2.IsEmpty());
        REQUIRE(hastMap2.GetSize() == 3);

        typename Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType>::ConstIterator lhs = hashMap1.GetIterator();
        typename Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType>::ConstIterator rhs = hastMap2.GetIterator();
        while (lhs.IsValid() || rhs.IsValid())
        {
            REQUIRE(lhs.IsValid());
            REQUIRE(rhs.IsValid());
            REQUIRE(lhs.GetValue().Key == rhs.GetValue().Key);
            REQUIRE(lhs.GetValue().Value == rhs.GetValue().Value);
            
            lhs.Advance();
            rhs.Advance();
        }
    }
}

TEMPLATE_TEST_CASE("Create by move", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap1;
    hashMap1.Put(1, 2);
    hashMap1.Put(2, 4);
    hashMap1.Put(3, 6);
    
    SECTION("By constructor")
    {
        Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hastMap2(std::move(hashMap1));
    
        REQUIRE(hashMap1.IsEmpty());
        REQUIRE_FALSE(hastMap2.IsEmpty());

        REQUIRE(hashMap1.GetSize() == 0);
        REQUIRE(hastMap2.GetSize() == 3);
    }
    
    SECTION("By assignation")
    {
        Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hastMap2 = std::move(hashMap1);

        REQUIRE(hashMap1.IsEmpty());
        REQUIRE_FALSE(hastMap2.IsEmpty());

        REQUIRE(hashMap1.GetSize() == 0);
        REQUIRE(hastMap2.GetSize() == 3);
    }
}

TEMPLATE_TEST_CASE("Put elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;

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

    SECTION("Put element by computing if key is missing")
    {
        int result = hashMap.ComputeIfMissing(1, [](int key) {
            return key + 1;
        });
        REQUIRE(result == 2);
        REQUIRE(hashMap.GetSize() == 1);

        result = hashMap.ComputeIfMissing(1, [](int key) {
            return key - 1;
        });
        REQUIRE(result == 2);
        REQUIRE(hashMap.GetSize() == 1);
    }
}

TEMPLATE_TEST_CASE("Replace elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
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

TEMPLATE_TEST_CASE("Get elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
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

TEMPLATE_TEST_CASE("Get or add default elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);
    hashMap.Put(3, 6);

    SECTION("with a missing key")
    {
        REQUIRE(hashMap.GetOrAdd(1) == 2);
        REQUIRE(hashMap.GetSize() == 3);
    }

    SECTION("with a valid key")
    {
        REQUIRE(hashMap.GetOrAdd(4) == 0);
        REQUIRE(hashMap.GetSize() == 4);
    }
}

TEMPLATE_TEST_CASE("Test containing elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);
    hashMap.Put(3, 6);

    SECTION("with a missing key")
    {
        REQUIRE(hashMap.Contains(1));
    }

    SECTION("with a valid key")
    {
        REQUIRE_FALSE(hashMap.Contains(4));
    }
}

TEMPLATE_TEST_CASE("Remove elements", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);

    SECTION("With an existing key")
    {
        REQUIRE(hashMap.Remove(1));
        REQUIRE(hashMap.GetSize() == 1);
    }
}

TEMPLATE_TEST_CASE("Get all values", "[HashMap]", Ck::HeapAllocator, Ck::LargeHeapAllocator, Ck::LinearAllocator<1024>, Ck::LargeLinearAllocator<1024>)
{
    Ck::HashMap<int, int, std::hash<int>, std::equal_to<int>, TestType> hashMap;
    hashMap.Put(1, 2);
    hashMap.Put(2, 4);
    hashMap.Put(3, 6);

    Ck::Array<int, TestType> values = hashMap.GetValues();
    REQUIRE(values.GetSize() == 3);
    REQUIRE(values.Contains(2));
    REQUIRE(values.Contains(4));
    REQUIRE(values.Contains(6));
}
