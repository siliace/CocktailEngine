#include <Cocktail/Core/Utility/Time/Clock.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

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
		mStarted = true;
	}

	void Clock::Stop()
	{
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
}
