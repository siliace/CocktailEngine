#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Memory/SharedPtr.hpp>

TEST_CASE("Construct a SharedPtr", "[SharedPtr]")
{
    SECTION("Empty")
    {
        Ck::SharedPtr<int> shared;
        REQUIRE(shared.Get() == nullptr);
    }

    SECTION("From a remote pointer")
    {
        Ck::SharedPtr<int> shared(new int(42));
        REQUIRE(*shared.Get() == 42);
    }

    SECTION("From MakeShared")
    {
        Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
        REQUIRE(*shared.Get() == 42);
    }
}

TEST_CASE("Construct SharedPtr from UniquePtr", "[SharedPtr]")
{
    Ck::UniquePtr<int> unique(new int(42));

    Ck::SharedPtr<int> shared(std::move(unique));

    REQUIRE(unique.Get() == nullptr);
    REQUIRE(shared);
    REQUIRE(*shared == 42);
    REQUIRE(shared.UseCount() == 1);
}

TEST_CASE("Copy a SharedPtr", "[SharedPtr]")
{
    Ck::SharedPtr<int> shared1(new int(42));

    SECTION("By construction")
    {
        Ck::SharedPtr<int> shared2(shared1);
        REQUIRE(shared1.Get() == shared2.Get());
    }

    SECTION("By assignation")
    {
        Ck::SharedPtr<int> shared2 = shared1;
        REQUIRE(shared1.Get() == shared2.Get());
    }

    SECTION("Self assignation")
    {
        Ck::SharedPtr<int> shared(new int(42));
        shared = shared;

        REQUIRE(shared);
        REQUIRE(shared.UseCount() == 1);
        REQUIRE(*shared == 42);
    }
}

TEST_CASE("Move a SharedPtr", "[SharedPtr]")
{
    Ck::SharedPtr<int> shared1(new int(42));

    SECTION("By construction")
    {
        Ck::SharedPtr<int> shared2(std::move(shared1));

        REQUIRE(shared1.Get() == nullptr);
        REQUIRE_FALSE(shared2.Get() == nullptr);
    }

    SECTION("By assignation")
    {
        Ck::SharedPtr<int> shared2 = std::move(shared1);

        REQUIRE(shared1.Get() == nullptr);
        REQUIRE_FALSE(shared2.Get() == nullptr);
    }

    SECTION("Self assignation")
    {
        Ck::SharedPtr<int> shared(new int(42));
        shared = std::move(shared);

        REQUIRE(shared);
        REQUIRE(shared.UseCount() == 1);
        REQUIRE(*shared == 42);
    }
}

TEST_CASE("SharedPtr use count", "[SharedPtr]")
{
    Ck::SharedPtr<int> shared1(new int(42));
    REQUIRE(shared1.UseCount() == 1);

    {
        Ck::SharedPtr<int> shared2(shared1);
        REQUIRE(shared1.UseCount() == 2);
        REQUIRE(shared2.UseCount() == 2);

        {
            Ck::SharedPtr<int> shared3 = shared2;
            REQUIRE(shared1.UseCount() == 3);
        }

        REQUIRE(shared1.UseCount() == 2);
    }

    REQUIRE(shared1.UseCount() == 1);
}

TEST_CASE("Reset a SharedPtr", "[SharedPtr]")
{
    Ck::SharedPtr<int> shared(new int(42));

    REQUIRE(shared);
    REQUIRE(shared.UseCount() == 1);

    shared.Reset(nullptr);

    REQUIRE_FALSE(shared);
    REQUIRE(shared.Get() == nullptr);
    REQUIRE(shared.UseCount() == 0);
}

TEST_CASE("Compare SharedPtr", "[SharedPtr]")
{
    Ck::SharedPtr<int> shared1(new int(42));
    Ck::SharedPtr<int> shared2(shared1);
    Ck::SharedPtr<int> shared3(new int(42));

    REQUIRE(shared1 == shared2);
    REQUIRE(shared1 != shared3);

    REQUIRE(shared1 != nullptr);

    shared3 = nullptr;
    REQUIRE(shared3 == nullptr);
}

struct Counter
{
    static inline int destroyed = 0;

    ~Counter()
    {
        ++destroyed;
    }
};

TEST_CASE("SharedPtr destroys object once", "[SharedPtr]")
{
    Counter::destroyed = 0;

    {
        Ck::SharedPtr<Counter> p1 = Ck::MakeShared<Counter>();

        {
            Ck::SharedPtr<Counter> p2 = p1;
            Ck::SharedPtr<Counter> p3 = p2;
        }

        REQUIRE(Counter::destroyed == 0);
    }

    REQUIRE(Counter::destroyed == 1);
}

TEST_CASE("StaticCast SharedPtr", "[SharedPtr]")
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Derived : Base
    {
        int value = 42;
    };

    Ck::SharedPtr<Derived> derived = Ck::MakeShared<Derived>();

    Ck::SharedPtr<Base> base = derived.StaticCast<Base>();

    REQUIRE(derived.UseCount() == 2);
    REQUIRE(base.UseCount() == 2);
    REQUIRE(base.Get() == static_cast<Base*>(derived.Get()));
}