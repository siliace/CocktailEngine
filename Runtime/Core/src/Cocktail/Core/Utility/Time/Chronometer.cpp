#include <Cocktail/Core/Utility/Time/Chronometer.hpp>

namespace Ck
{
    Chronometer::Chronometer(const TimeUnit& timeUnit) :
        mClock(timeUnit)
    {
        OnBeginLap();
    }

    void Chronometer::Mark()
    {
        mCurrentLap.Time = mClock.GetElapsedDuration();
        mLaps.Add(mCurrentLap);

        OnBeginLap();
    }

    void Chronometer::Reset()
    {
        mLaps.Clear();

        OnBeginLap();
    }

    const Array<Chronometer::Lap>& Chronometer::GetLaps() const
    {
        return mLaps;
    }

    void Chronometer::OnBeginLap()
    {
        mClock.Start();

        mCurrentLap.Start = mClock.GetStart();
        mCurrentLap.Time = Duration(0, mClock.GetTimeUnit());
    }
}
