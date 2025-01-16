#include <cassert>

#include <Cocktail/Core/Utility/Time/Duration.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>
#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
	Instant Instant::EpochMilliseconds(Uint64 milliseconds)
	{
		return {
			TimeUnit::Milliseconds().ConvertTo(milliseconds, TimeUnit::Seconds()),
			0
		};
	}

	Instant Instant::EpochSeconds(Uint64 seconds, Uint64 nanoseconds)
	{
		return {seconds, nanoseconds};
	}

	Instant::Instant() :
		Instant(0, 0)
	{
		/// Nothing
	}

	Instant Instant::After(const Duration& offset) const
	{
		const bool isNanoseconds = offset.GetUnit().IsSmaller(TimeUnit::Seconds());
		Uint64 seconds = mSeconds;
		Uint64 nanoseconds = mNanoseconds;

		if (isNanoseconds)
		{
			nanoseconds += offset.As(TimeUnit::Nanoseconds()).GetCount();
		}
		else
		{
			seconds += offset.As(TimeUnit::Seconds()).GetCount();
		}

		return EpochSeconds(seconds, nanoseconds);
	}

	bool Instant::IsAfter(const Instant& other) const
	{
		if (mSeconds > other.mSeconds)
			return true;

		return mNanoseconds > other.mNanoseconds;
	}

	Instant Instant::Before(const Duration& offset) const
	{
		const bool isNanoseconds = offset.GetUnit().IsSmaller(TimeUnit::Seconds());
		Uint64 seconds = mSeconds;
		Uint64 nanoseconds = mNanoseconds;

		if (isNanoseconds)
		{
			Uint64 nanosecondsOffset = offset.As(TimeUnit::Nanoseconds()).GetCount();
			assert(nanoseconds > nanosecondsOffset);
			nanoseconds -= nanosecondsOffset;
		}
		else
		{
			Uint64 secondsOffset = offset.As(TimeUnit::Seconds()).GetCount();
			assert(seconds > secondsOffset);
			seconds -= offset.As(TimeUnit::Seconds()).GetCount();
		}

		return EpochSeconds(seconds, nanoseconds);
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

	Uint64 Instant::GetNanoseconds() const
	{
		return mNanoseconds;
	}

	Instant::Instant(Uint64 seconds, Uint64 nanoseconds) :
		mSeconds(seconds),
		mNanoseconds(nanoseconds)
	{
		/// Nothing
	}
}
