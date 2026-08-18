#include <cassert>

#include <CocktailEngine/Core/Utility/Time/Duration.hpp>
#include <CocktailEngine/Core/Utility/Time/Instant.hpp>
#include <CocktailEngine/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
    namespace
    {
        constexpr Uint64 NanosecondsPerSecond = 1'000'000'000;
    }

    Instant Instant::EpochMilliseconds(Uint64 milliseconds)
    {
        return { milliseconds / 1'000, (milliseconds % 1'000) * 1'000'000 };
    }

    Instant Instant::EpochSeconds(Uint64 seconds, Uint64 nanoseconds)
    {
        return { seconds, nanoseconds };
    }

    Instant::Instant() :
        Instant(0, 0)
    {
        /// Nothing
    }

    Instant Instant::After(const Duration& offset) const
    {
        Uint64 seconds = mSeconds;
        Uint64 nanoseconds = mNanoseconds;

        if (offset.GetUnit().IsSmaller(TimeUnit::Seconds()))
        {
            nanoseconds += offset.GetCount(TimeUnit::Nanoseconds());
        }
        else
        {
            seconds += offset.GetCount(TimeUnit::Seconds());
        }

        return EpochSeconds(seconds, nanoseconds);
    }

    bool Instant::IsAfter(const Instant& other) const
    {
        if (mSeconds == other.mSeconds)
            return mNanoseconds > other.mNanoseconds;

        return mSeconds > other.mSeconds;
    }

    Instant Instant::Before(const Duration& offset) const
    {
        Uint64 seconds = mSeconds;
        Uint64 nanoseconds = mNanoseconds;

        Uint64 offsetSeconds;
        Uint64 offsetNanoseconds = 0;

        if (offset.GetUnit().IsSmaller(TimeUnit::Seconds()))
        {
            const Uint64 offsetCount = offset.GetCount(TimeUnit::Nanoseconds());
            offsetSeconds = offsetCount / NanosecondsPerSecond;
            offsetNanoseconds = offsetCount % NanosecondsPerSecond;
        }
        else
        {
            offsetSeconds = offset.GetCount(TimeUnit::Seconds());
        }

        /// Borrow a second when the nanoseconds component would underflow
        if (nanoseconds < offsetNanoseconds)
        {
            assert(offsetSeconds < seconds);
            --seconds;
            nanoseconds += NanosecondsPerSecond;
        }

        assert(offsetSeconds <= seconds);

        return EpochSeconds(seconds - offsetSeconds, nanoseconds - offsetNanoseconds);
    }

    bool Instant::IsBefore(const Instant& other) const
    {
        if (mSeconds == other.mSeconds)
            return mNanoseconds < other.mNanoseconds;

        return mSeconds < other.mSeconds;
    }

    Uint64 Instant::GetSeconds() const
    {
        return mSeconds;
    }

    Uint32 Instant::GetNanoseconds() const
    {
        return mNanoseconds;
    }

    Instant::Instant(Uint64 seconds, Uint64 nanoseconds) :
        mSeconds(seconds + nanoseconds / NanosecondsPerSecond),
        mNanoseconds(static_cast<Uint32>(nanoseconds % NanosecondsPerSecond))
    {
        /// Nothing
    }
}
