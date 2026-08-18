#include <CocktailEngine/Core/Utility/Time/Clock.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

namespace Ck
{
    Clock::Clock(const TimeUnit& timeUnit) :
        mStarted(false),
        mTimeUnit(timeUnit)
    {
        /// Nothing
    }

    Clock::Clock(const Instant& start, const TimeUnit& timeUnit) :
        Clock(timeUnit)
    {
        Start(start);
    }

    void Clock::Start(const Instant& start)
    {
        mStarted = true;
        mStart = start;
    }

    void Clock::Resume()
    {
        if (mStarted)
            return;

        /// The start instant is pushed forward by however long the clock stayed
        /// stopped, so that the elapsed duration picks up where Stop() left it
        /// instead of counting the pause
        mStart = mStart.After(Duration::Between(mStop, Instant::Now()));
        mStarted = true;
    }

    void Clock::Stop()
    {
        /// Stopping an already stopped clock would move the instant Resume() measures
        /// the pause from, and shorten the interval the clock reports
        if (!mStarted)
            return;

        mStarted = false;
        mStop = Instant::Now();
    }

    bool Clock::IsStarted() const
    {
        return mStarted;
    }

    Duration Clock::GetElapsedDuration() const
    {
        return Duration::Between(mStarted ? Instant::Now() : mStop, mStart).As(mTimeUnit);
    }

    Instant Clock::GetStart() const
    {
        return mStart;
    }

    TimeUnit Clock::GetTimeUnit() const
    {
        return mTimeUnit;
    }
}
