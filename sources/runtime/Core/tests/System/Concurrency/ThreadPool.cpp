#include <atomic>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Exception.hpp>
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
        throw RuntimeException(CK_TEXT("expected failure"));
    });
    Future<int> succeeded = pool.Submit([] {
        return 42;
    });

    REQUIRE_THROWS_AS(failed.GetValue(), RuntimeException);
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

TEST_CASE("Flushing an idle thread pool returns without blocking", "[ThreadPool]")
{
    ThreadPool pool(3);

    REQUIRE_NOTHROW(pool.Flush());
    REQUIRE_NOTHROW(pool.Flush());
}

TEST_CASE("Flushing a thread pool waits for every pending task", "[ThreadPool]")
{
    constexpr unsigned int TaskCount = 64;
    constexpr unsigned int Rounds = 16;

    ThreadPool pool(4);

    for (unsigned int round = 0; round < Rounds; ++round)
    {
        std::atomic<unsigned int> completed = 0;

        for (unsigned int i = 0; i < TaskCount; ++i)
        {
            pool.Execute([&completed] {
                ++completed;
            });
        }

        pool.Flush();

        REQUIRE(completed == TaskCount);
    }
}

TEST_CASE("Flushing a thread pool waits for a task already running", "[ThreadPool]")
{
    ThreadPool pool(4);
    std::atomic<bool> completed = false;

    pool.Execute([&completed] {
        Thread::SleepFor(Duration::Milliseconds(50));
        completed = true;
    });

    pool.Flush();

    REQUIRE(completed);
}
TEST_CASE("Flushing a thread pool waits for submitted work", "[ThreadPool]")
{
    constexpr unsigned int TaskCount = 32;

    ThreadPool pool(4);
    std::atomic<unsigned int> completed = 0;
    Array<Future<unsigned int>> futures;
    futures.Reserve(TaskCount);

    for (unsigned int i = 0; i < TaskCount; ++i)
    {
        futures.Emplace(pool.Submit([&completed, i] {
            ++completed;
            return i;
        }));
    }

    pool.Flush();

    REQUIRE(completed == TaskCount);

    for (unsigned int i = 0; i < TaskCount; ++i)
        REQUIRE(futures[i].GetValue() == i);
}

TEST_CASE("Flushing a thread pool returns once a failed task is accounted for", "[ThreadPool]")
{
    ThreadPool pool(2);

    Future<int> failed = pool.Submit([]() -> int {
        throw RuntimeException(CK_TEXT("expected failure"));
    });

    pool.Flush();

    REQUIRE_THROWS_AS(failed.GetValue(), RuntimeException);
}

TEST_CASE("Several threads can flush a thread pool at once", "[ThreadPool]")
{
    constexpr unsigned int TaskCount = 32;
    constexpr unsigned int FlusherCount = 4;

    ThreadPool pool(4);
    ThreadPool flushers(FlusherCount);
    std::atomic<unsigned int> completed = 0;

    for (unsigned int i = 0; i < TaskCount; ++i)
    {
        pool.Execute([&completed] {
            Thread::SleepFor(Duration::Milliseconds(1));
            ++completed;
        });
    }

    /// Every flusher has to see the pool fully drained, not just the one the last
    /// worker happened to wake.
    Array<Future<unsigned int>> observed;
    observed.Reserve(FlusherCount);

    for (unsigned int i = 0; i < FlusherCount; ++i)
    {
        observed.Emplace(flushers.Submit([&pool, &completed] {
            pool.Flush();
            return completed.load();
        }));
    }

    for (unsigned int i = 0; i < FlusherCount; ++i)
        REQUIRE(observed[i].GetValue() == TaskCount);
}

TEST_CASE("A thread pool accepts work again after being flushed", "[ThreadPool]")
{
    ThreadPool pool(2);
    std::atomic<unsigned int> completed = 0;

    pool.Execute([&completed] {
        ++completed;
    });
    pool.Flush();
    REQUIRE(completed == 1);

    pool.Execute([&completed] {
        ++completed;
    });
    pool.Flush();
    REQUIRE(completed == 2);
}

TEST_CASE("A thread pool refuses to be flushed by one of its own workers", "[ThreadPool]")
{
    ThreadPool pool(2);

    /// Reported rather than deadlocked: the worker would be waiting for the task it
    /// is itself running.
    Future<bool> refused = pool.Submit([&pool] {
        try
        {
            pool.Flush();
        }
        catch (const LogicException&)
        {
            return true;
        }

        return false;
    });

    REQUIRE(refused.GetValue());
}
