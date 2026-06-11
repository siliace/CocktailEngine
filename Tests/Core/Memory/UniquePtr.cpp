#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Memory/UniquePtr.hpp>

using namespace Ck;

TEST_CASE("Construct a UniquePtr", "[UniquePtr]")
{
    SECTION("Empty")
    {
        UniquePtr<int> ptr;

        REQUIRE(ptr.Get() == nullptr);
        REQUIRE_FALSE(ptr);
    }

    SECTION("From nullptr")
    {
        UniquePtr<int> ptr(nullptr);

        REQUIRE(ptr.Get() == nullptr);
        REQUIRE_FALSE(ptr);
    }

    SECTION("From raw pointer")
    {
        UniquePtr<int> ptr(new int(42));

        REQUIRE(ptr.Get() != nullptr);
        REQUIRE(*ptr == 42);
        REQUIRE(ptr.IsValid());
    }

    SECTION("From MakeUnique")
    {
        auto ptr = MakeUnique<int>(42);

        REQUIRE(ptr);
        REQUIRE(*ptr == 42);
    }
}

TEST_CASE("Move UniquePtr", "[UniquePtr]")
{
    UniquePtr<int> ptr1(new int(42));

    SECTION("By construction")
    {
        UniquePtr<int> ptr2(std::move(ptr1));

        REQUIRE(ptr1.Get() == nullptr);
        REQUIRE(ptr2);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("By assignment")
    {
        UniquePtr<int> ptr2;
        ptr2 = std::move(ptr1);

        REQUIRE(ptr1.Get() == nullptr);
        REQUIRE(ptr2);
        REQUIRE(*ptr2 == 42);
    }

    SECTION("Self move assignment")
    {
        UniquePtr<int> ptr(new int(42));

        ptr = std::move(ptr);

        REQUIRE(ptr);
        REQUIRE(*ptr == 42);
    }
}

TEST_CASE("Reset UniquePtr", "[UniquePtr]")
{
    UniquePtr<int> ptr(new int(42));

    REQUIRE(ptr);

    ptr.Reset();

    REQUIRE(ptr.Get() == nullptr);
    REQUIRE_FALSE(ptr);

    SECTION("Reset with new pointer")
    {
        ptr.Reset(new int(100));

        REQUIRE(ptr);
        REQUIRE(*ptr == 100);
    }
}

TEST_CASE("Release UniquePtr", "[UniquePtr]")
{
    UniquePtr<int> ptr(new int(42));

    int* raw = ptr.Release();

    REQUIRE(ptr.Get() == nullptr);
    REQUIRE(raw != nullptr);
    REQUIRE(*raw == 42);

    delete raw;
}

TEST_CASE("UniquePtr comparison", "[UniquePtr]")
{
    UniquePtr<int> ptr1(new int(42));
    UniquePtr<int> ptr2(std::move(ptr1));

    UniquePtr<int> ptr3(new int(42));

    REQUIRE(ptr2 == ptr2);
    REQUIRE(ptr2 != ptr3);

    REQUIRE(ptr2 != nullptr);

    ptr3.Reset();
    REQUIRE(ptr3 == nullptr);
}

TEST_CASE("UniquePtr arrow and dereference", "[UniquePtr]")
{
    struct Foo
    {
        int value = 42;
        int Get() const { return value; }
    };

    UniquePtr<Foo> ptr(new Foo());

    REQUIRE(ptr->value == 42);
    REQUIRE((*ptr).Get() == 42);
}

struct Counter
{
    static inline int destroyed = 0;

    ~Counter()
    {
        ++destroyed;
    }
};

TEST_CASE("UniquePtr destruction", "[UniquePtr]")
{
    Counter::destroyed = 0;

    {
        UniquePtr<Counter> ptr1(new Counter());
        {
            UniquePtr<Counter> ptr2(std::move(ptr1));
        }
        REQUIRE(Counter::destroyed == 1);
    }

    REQUIRE(Counter::destroyed == 1);
}

TEST_CASE("UniquePtr self assignment safety", "[UniquePtr]")
{
    UniquePtr<int> ptr(new int(42));

    ptr = std::move(ptr);

    REQUIRE(ptr);
    REQUIRE(*ptr == 42);
}

TEST_CASE("UniquePtr array", "[UniquePtr]")
{
    SECTION("MakeUnique array")
    {
        auto ptr = MakeUnique<int[]>(5);

        for (int i = 0; i < 5; i++)
            ptr[i] = i * 10;

        REQUIRE(ptr[0] == 0);
        REQUIRE(ptr[4] == 40);
    }

    SECTION("Move array UniquePtr")
    {
        UniquePtr<int[]> ptr1(new int[3]{1,2,3});

        UniquePtr<int[]> ptr2(std::move(ptr1));

        REQUIRE(ptr1.Get() == nullptr);
        REQUIRE(ptr2[1] == 2);
    }

    SECTION("Array reset")
    {
        UniquePtr<int[]> ptr(new int[3]{1,2,3});

        ptr.Reset();

        REQUIRE(ptr.Get() == nullptr);
    }
}
