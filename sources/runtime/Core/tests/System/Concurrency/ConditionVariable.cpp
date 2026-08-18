#include <catch2/catch_all.hpp>

#include <atomic>

#include <CocktailEngine/Core/System/Concurrency/ConditionVariable.hpp>
#include <CocktailEngine/Core/System/Concurrency/LockGuard.hpp>
#include <CocktailEngine/Core/System/Concurrency/Mutex.hpp>
#include <CocktailEngine/Core/System/Concurrency/Runnable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>

using namespace Ck;

namespace
{
    /**
     * \brief Spins until adding \p offset to the current instant crosses a second
     *
     * The deadline a relative wait computes is an absolute instant, and the platform
     * primitives it is handed to describe it as a seconds and a nanoseconds component.
     * A deadline landing in the next second is what forces the nanoseconds component to
     * be carried over, and an un-normalized component is rejected outright rather than
     * waited on, so the case has to be reached deliberately instead of being left to
     * whichever point of a second the test happens to run at.
     *
     * \param offset Duration the wait under test is given
     */
    void AwaitDeadlineCrossingASecond(const Duration& offset)
    {
        const Uint64 offsetNanoseconds = offset.GetCount(TimeUnit::Nanoseconds());

        while (Instant::Now().GetNanoseconds() + offsetNanoseconds < 1'000'000'000)
            Thread::Yield();
    }
}

TEST_CASE("A condition variable reports a timeout rather than an error", "[ConditionVariable]")
{
    Mutex mutex;
    ConditionVariable conditionVariable;

    SECTION("When the deadline stays within the current second")
    {
        LockGuard guard(mutex);

        bool notified = true;
        REQUIRE_NOTHROW(notified = conditionVariable.WaitFor(mutex, Duration::Milliseconds(20)));
        REQUIRE_FALSE(notified);
    }

    SECTION("When the deadline crosses into the next second")
    {
        const Duration timeout = Duration::Milliseconds(20);
        AwaitDeadlineCrossingASecond(timeout);

        LockGuard guard(mutex);

        bool notified = true;
        REQUIRE_NOTHROW(notified = conditionVariable.WaitFor(mutex, timeout));
        REQUIRE_FALSE(notified);
    }

    SECTION("When the deadline has already passed")
    {
        LockGuard guard(mutex);

        REQUIRE_FALSE(conditionVariable.WaitUntil(mutex, Instant::Now()));
    }
}

TEST_CASE("A condition variable waits for the whole timeout", "[ConditionVariable]")
{
    Mutex mutex;
    ConditionVariable conditionVariable;

    LockGuard guard(mutex);

    const Instant start = Instant::Now();
    REQUIRE_FALSE(conditionVariable.WaitFor(mutex, Duration::Milliseconds(100)));
    const Duration elapsed = Duration::Between(start, Instant::Now());

    /// Slightly below the requested timeout, the point being to catch a wait that
    /// gives up at once rather than to measure the scheduler
    REQUIRE(elapsed >= Duration::Milliseconds(90));
}

TEST_CASE("A condition variable wakes a waiting thread", "[ConditionVariable]")
{
    Mutex mutex;
    ConditionVariable conditionVariable;
    bool ready = false;

    UniquePtr<Runnable> notifier = MakeRunnable([&] {
        Thread::SleepFor(Duration::Milliseconds(20));

        LockGuard guard(mutex);
        ready = true;
        conditionVariable.NotifyOne();
    });

    UniquePtr<Thread> thread = Thread::Create(notifier.Get(), CK_TEXT("cv-notifier"));

    {
        LockGuard guard(mutex);
        REQUIRE(conditionVariable.WaitFor(mutex, Duration::Seconds(5), [&] {
            return ready;
        }));
    }

    thread->Join();
}

TEST_CASE("An infinite timeout waits for a notification", "[ConditionVariable]")
{
    Mutex mutex;
    ConditionVariable conditionVariable;

    std::atomic<bool> returned = false;
    bool notified = false;

    UniquePtr<Runnable> waiter = MakeRunnable([&] {
        LockGuard guard(mutex);
        notified = conditionVariable.WaitFor(mutex, Duration::Infinite());
        returned = true;
    });

    UniquePtr<Thread> thread = Thread::Create(waiter.Get(), CK_TEXT("cv-waiter"));

    /// Notified repeatedly so that a notification sent before the waiter reached the
    /// wait, which a condition variable does not retain, cannot block it forever
    while (!returned)
    {
        {
            LockGuard guard(mutex);
            conditionVariable.NotifyAll();
        }

        Thread::SleepFor(Duration::Milliseconds(5));
    }

    thread->Join();

    /// An infinite timeout must block until notified, not report a timeout at once
    REQUIRE(notified);
}
