#include <atomic>

#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/Concurrency/Promise.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

using namespace Ck;

TEST_CASE("A future receives values published by its promise", "[Future][Promise]")
{
    Promise<int> promise;
    Future<int> future = promise.ToFuture();
    const int value = 42;

    REQUIRE_FALSE(future.IsSatisfied());
    promise.SetValue(value);

    REQUIRE(future.IsSatisfied());
    REQUIRE(future.GetValue() == value);
}

TEST_CASE("A promise accepts an rvalue exactly once", "[Future][Promise]")
{
    Promise<int> promise;
    Future<int> future = promise.ToFuture();

    REQUIRE_NOTHROW(promise.SetValue(42));
    REQUIRE_THROWS_AS(promise.SetValue(24), AlreadySatisfiedException);

    const int otherValue = 12;
    REQUIRE_THROWS_AS(promise.SetValue(otherValue), AlreadySatisfiedException);
    REQUIRE(future.GetValue() == 42);
}

TEST_CASE("Futures share a fulfilled state that outlives its promise", "[Future][Promise]")
{
    const Future<int> future = [] {
        Promise<int> promise;
        Future<int> result = promise.ToFuture();
        promise.SetValue(42);
        return result;
    }();
    const Future<int> copy = future;

    REQUIRE(future.IsSatisfied());
    REQUIRE(copy.IsSatisfied());
    REQUIRE(future.GetValue() == 42);
    REQUIRE(copy.GetValue() == 42);
}

TEST_CASE("A future blocks until its promise is fulfilled", "[Future][Promise]")
{
    Promise<int> promise;
    Future<int> future = promise.ToFuture();
    std::atomic<bool> started = false;
    std::atomic<bool> returned = false;
    int value = 0;

    UniquePtr<Runnable> consumer = MakeRunnable([&] {
        started = true;
        value = future.GetValue();
        returned = true;
    });
    UniquePtr<Thread> thread = Thread::Create(consumer.Get(), CK_TEXT("future-consumer"));

    while (!started)
        Thread::Yield();

    REQUIRE_FALSE(returned);

    promise.SetValue(42);
    thread->Join();

    REQUIRE(returned);
    REQUIRE(value == 42);
}

TEST_CASE("A future reports an abandoned promise", "[Future][Promise]")
{
    const Future<int> future = [] {
        Promise<int> promise;
        return promise.ToFuture();
    }();

    REQUIRE_FALSE(future.IsSatisfied());
    REQUIRE_THROWS_AS(future.GetValue(), BrokenPromiseException);
}
