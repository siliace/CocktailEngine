#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/Utility/Time/Clock.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace
{
    Ck::Uint64 ElapsedMilliseconds(const Ck::Clock& clock)
    {
        return clock.GetElapsedDuration().GetCount(Ck::TimeUnit::Milliseconds());
    }
}

TEST_CASE("A stopped clock does not count the time it stays stopped", "[Clock]")
{
    Ck::Clock clock(Ck::TimeUnit::Milliseconds());
    clock.Start();

    Ck::Thread::SleepFor(Ck::Duration::Milliseconds(50));
    clock.Stop();

    const Ck::Uint64 atStop = ElapsedMilliseconds(clock);
    REQUIRE(atStop >= 40);

    SECTION("The elapsed duration does not move while stopped")
    {
        Ck::Thread::SleepFor(Ck::Duration::Milliseconds(100));

        REQUIRE(ElapsedMilliseconds(clock) == atStop);
    }

    SECTION("Resuming picks up where the clock was stopped")
    {
        Ck::Thread::SleepFor(Ck::Duration::Milliseconds(150));
        clock.Resume();
        Ck::Thread::SleepFor(Ck::Duration::Milliseconds(50));

        /// The 150 ms pause must not show up: only the two running stretches count,
        /// so the upper bound is well below the 250 ms of wall clock that went by
        const Ck::Uint64 afterResume = ElapsedMilliseconds(clock);
        REQUIRE(afterResume >= 80);
        REQUIRE(afterResume < 150);
    }
}

TEST_CASE("A clock reports whether it is running", "[Clock]")
{
    Ck::Clock clock(Ck::TimeUnit::Milliseconds());
    REQUIRE_FALSE(clock.IsStarted());

    clock.Start();
    REQUIRE(clock.IsStarted());

    clock.Stop();
    REQUIRE_FALSE(clock.IsStarted());

    clock.Resume();
    REQUIRE(clock.IsStarted());
}

TEST_CASE("A clock that was never started reports no elapsed time", "[Clock]")
{
    Ck::Clock clock(Ck::TimeUnit::Milliseconds());

    SECTION("Before anything is asked of it")
    {
        REQUIRE(ElapsedMilliseconds(clock) == 0);
    }

    SECTION("After being stopped without ever being started")
    {
        clock.Stop();

        REQUIRE(ElapsedMilliseconds(clock) == 0);
    }
}
