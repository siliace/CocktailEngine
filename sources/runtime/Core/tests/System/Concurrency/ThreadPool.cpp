#include <catch2/catch_all.hpp>

#include <atomic>

#include <CocktailEngine/Core/System/Concurrency/ThreadPool.hpp>

using namespace Ck;

TEST_CASE("A thread pool executes fire-and-forget and submitted work", "[ThreadPool]")
{
    ThreadPool pool(1);
    std::atomic<unsigned int> executeCalls = 0;

    pool.Execute([&] {
        ++executeCalls;
    });

    Future<int> result = pool.Submit([](int lhs, int rhs) {
        return lhs + rhs;
    }, 19, 23);

    REQUIRE(result.GetValue() == 42);
    REQUIRE(executeCalls == 1);
}

TEST_CASE("A thread pool owns move-only task arguments", "[ThreadPool]")
{
    ThreadPool pool(1);

    Future<int> result = pool.Submit([](UniquePtr<int> value) {
        return *value;
    }, MakeUnique<int>(42));

    REQUIRE(result.GetValue() == 42);
}

TEST_CASE("A thread pool reports task failures and keeps its worker alive", "[ThreadPool]")
{
    ThreadPool pool(1);

    Future<int> failed = pool.Submit([]() -> int {
        throw std::runtime_error("expected failure");
    });
    Future<int> succeeded = pool.Submit([] {
        return 42;
    });

    REQUIRE_THROWS_AS(failed.GetValue(), std::runtime_error);
    REQUIRE(succeeded.GetValue() == 42);
}

TEST_CASE("A thread pool runs every submitted task once", "[ThreadPool]")
{
    constexpr unsigned int TaskCount = 64;

    ThreadPool pool(3);
    std::atomic<unsigned int> executionCount = 0;
    Array<Future<unsigned int>> futures;
    futures.Reserve(TaskCount);

    for (unsigned int i = 0; i < TaskCount; ++i)
    {
        futures.Emplace(pool.Submit([&executionCount, i] {
            ++executionCount;
            return i * i;
        }));
    }

    for (unsigned int i = 0; i < TaskCount; ++i)
        REQUIRE(futures[i].GetValue() == i * i);

    REQUIRE(executionCount == TaskCount);
}

TEST_CASE("Destroying an idle thread pool releases every worker", "[ThreadPool]")
{
    REQUIRE_NOTHROW([] {
        ThreadPool pool(3);
    }());
}
