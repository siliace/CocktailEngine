#include <catch2/catch_all.hpp>

#include <Cocktail/Core/Memory/SharedPtr.hpp>
#include <Cocktail/Core/Memory/WeakPtr.hpp>

TEST_CASE("Construct a WeakPtr", "[WeakPtr]")
{
    SECTION("Empty")
    {
        Ck::WeakPtr<int> weak;

        REQUIRE(weak.IsExpired());
        REQUIRE(weak.ToStrong() == nullptr);
    }

    SECTION("From nullptr")
    {
        Ck::WeakPtr<int> weak(nullptr);

        REQUIRE(weak.IsExpired());
        REQUIRE(weak.ToStrong() == nullptr);
    }

    SECTION("From SharedPtr")
    {
        Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
        Ck::WeakPtr<int> weak(shared);

        REQUIRE_FALSE(weak.IsExpired());

        auto strong = weak.ToStrong();
        REQUIRE(strong);
        REQUIRE(*strong == 42);
    }
}

TEST_CASE("Copy WeakPtr", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
    Ck::WeakPtr<int> weak1(shared);

    SECTION("By construction")
    {
        Ck::WeakPtr<int> weak2(weak1);

        REQUIRE(weak1.IsExpired() == weak2.IsExpired());
    }

    SECTION("By assignation")
    {
        Ck::WeakPtr<int> weak2;
        weak2 = weak1;

        REQUIRE(weak1.IsExpired() == weak2.IsExpired());
    }

    SECTION("Self assignation")
    {
        weak1 = weak1;

        REQUIRE_FALSE(weak1.IsExpired());
    }
}

TEST_CASE("Move WeakPtr", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
    Ck::WeakPtr<int> weak1(shared);

    SECTION("By construction")
    {
        Ck::WeakPtr<int> weak2(std::move(weak1));

        REQUIRE(weak1.IsExpired());
        REQUIRE_FALSE(weak2.IsExpired());
    }

    SECTION("By assignation")
    {
        Ck::WeakPtr<int> weak2;
        weak2 = std::move(weak1);

        REQUIRE(weak1.IsExpired());
        REQUIRE_FALSE(weak2.IsExpired());
    }
}

TEST_CASE("WeakPtr expired behavior", "[WeakPtr]")
{
    Ck::WeakPtr<int> weak;

    {
        Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
        weak = shared;

        REQUIRE_FALSE(weak.IsExpired());
        REQUIRE(weak.ToStrong());
    }

    REQUIRE(weak.IsExpired());
    REQUIRE(weak.ToStrong() == nullptr);
}

TEST_CASE("WeakPtr ToStrong", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
    Ck::WeakPtr<int> weak(shared);

    auto strong1 = weak.ToStrong();
    auto strong2 = weak.ToStrong();

    REQUIRE(strong1);
    REQUIRE(strong2);

    REQUIRE(*strong1 == 42);
    REQUIRE(*strong2 == 42);

    REQUIRE(strong1.UseCount() >= 1);
}

TEST_CASE("WeakPtr reset behavior", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);
    Ck::WeakPtr<int> weak(shared);

    weak = nullptr;

    REQUIRE(weak.IsExpired());
    REQUIRE(weak.ToStrong() == nullptr);
}

TEST_CASE("WeakPtr assignment from SharedPtr", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared1 = Ck::MakeShared<int>(42);
    Ck::SharedPtr<int> shared2 = Ck::MakeShared<int>(100);

    Ck::WeakPtr<int> weak(shared1);

    weak = shared2;

    auto strong = weak.ToStrong();

    REQUIRE(strong);
    REQUIRE(*strong == 100);
}

TEST_CASE("WeakPtr does not affect use count", "[WeakPtr]")
{
    Ck::SharedPtr<int> shared = Ck::MakeShared<int>(42);

    REQUIRE(shared.UseCount() == 1);

    {
        Ck::WeakPtr<int> weak(shared);
        REQUIRE(shared.UseCount() == 1);

        auto strong = weak.ToStrong();
        REQUIRE(shared.UseCount() >= 1);
    }

    REQUIRE(shared.UseCount() == 1);
}

struct WeakCounter
{
    static inline int destroyed = 0;

    ~WeakCounter()
    {
        ++destroyed;
    }
};

TEST_CASE("WeakPtr does not extend lifetime", "[WeakPtr]")
{
    WeakCounter::destroyed = 0;

    Ck::WeakPtr<WeakCounter> weak;

    {
        Ck::SharedPtr<WeakCounter> shared = Ck::MakeShared<WeakCounter>();
        weak = shared;

        REQUIRE_FALSE(weak.IsExpired());
    }

    REQUIRE(WeakCounter::destroyed == 1);
    REQUIRE(weak.IsExpired());
}