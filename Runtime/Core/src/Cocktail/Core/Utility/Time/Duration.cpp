#include <Cocktail/Core/Utility/Time/Duration.hpp>
#include <Cocktail/Core/Utility/Time/Instant.hpp>

namespace Ck
{
	Duration Duration::Infinite()
	{
		return { std::numeric_limits<Uint64>::max(), TimeUnit::Seconds() };
	}

	Duration Duration::Nanoseconds(Uint64 count)
	{
		return { count, TimeUnit::Nanoseconds() };
	}

	Duration Duration::Microseconds(Uint64 count)
	{
		return { count, TimeUnit::Microseconds() };
	}

	Duration Duration::Milliseconds(Uint64 count)
	{
		return { count, TimeUnit::Milliseconds() };
	}

	Duration Duration::Seconds(Uint64 count)
	{
		return { count, TimeUnit::Seconds() };
	}

	Duration Duration::Minutes(Uint64 count)
	{
		return { count, TimeUnit::Minutes() };
	}

	Duration Duration::Hours(Uint64 count)
	{
		return { count, TimeUnit::Hours() };
	}

	Duration Duration::Days(Uint64 count)
	{
		return { count, TimeUnit::Days() };
	}

	Duration Duration::Between(const Instant& lhs, const Instant& rhs)
	{
		Uint64 seconds = 0;
		Uint64 nanoseconds;
		if (lhs.GetSeconds() != rhs.GetSeconds())
		{
			seconds = AbsoluteDifference(lhs.GetSeconds(), rhs.GetSeconds()) - 1;
			if (lhs.IsBefore(rhs))
			{
				assert(lhs.GetSeconds() < rhs.GetSeconds());
				nanoseconds = rhs.GetNanoseconds() + (1'000'000'000 - lhs.GetNanoseconds());
			}
			else
			{
				assert(lhs.GetSeconds() > rhs.GetSeconds());
				nanoseconds = lhs.GetNanoseconds() + (1'000'000'000 - rhs.GetNanoseconds());
			}
		}
		else
		{
			nanoseconds = AbsoluteDifference(lhs.GetNanoseconds(), rhs.GetNanoseconds());
		}

		if (nanoseconds % 1'000'000'000 == 0)
		{
			Uint64 nanosecondsAsSeconds = TimeUnit::Seconds().ConvertFrom(nanoseconds, TimeUnit::Nanoseconds());
			return Seconds(seconds + nanosecondsAsSeconds);
		}

		if (nanoseconds % 1'000'000 == 0)
		{
			Uint64 secondsAsMilliseconds = TimeUnit::Milliseconds().ConvertFrom(seconds, TimeUnit::Seconds());
			Uint64 nanosecondsAsMilliseconds = TimeUnit::Milliseconds().ConvertFrom(nanoseconds, TimeUnit::Nanoseconds());
			return Milliseconds(secondsAsMilliseconds + nanosecondsAsMilliseconds);
		}

		if (nanoseconds % 1'000 == 0)
		{
			Uint64 secondsAsMicroseconds = TimeUnit::Microseconds().ConvertFrom(seconds, TimeUnit::Seconds());
			Uint64 nanosecondsAsMicroseconds = TimeUnit::Microseconds().ConvertFrom(nanoseconds, TimeUnit::Nanoseconds());
			return Microseconds(secondsAsMicroseconds + nanosecondsAsMicroseconds);
		}

		Uint64 secondsAsNanoseconds = TimeUnit::Nanoseconds().ConvertFrom(seconds, TimeUnit::Seconds());
		return Nanoseconds(secondsAsNanoseconds + nanoseconds);
	}

	Duration Duration::Between(const Duration& lhs, const Duration& rhs)
	{
		const TimeUnit& lhsTimeUnit = lhs.GetUnit();
		const TimeUnit& rhsTimeUnit = rhs.GetUnit();
		const TimeUnit& resultTimeUnit = lhsTimeUnit.IsSmaller(rhsTimeUnit) ? lhsTimeUnit : rhsTimeUnit;

		const Duration& max = std::max(lhs, rhs);
		const Duration& min = std::min(lhs, rhs);

		Uint64 resultCount = max.As(resultTimeUnit).GetCount();
		resultCount -= min.As(resultTimeUnit).GetCount();

		return Duration(resultCount, resultTimeUnit);
	}

	Duration::Duration() :
		Duration(0, TimeUnit::Seconds())
	{
		/// Nothing
	}

	Duration::Duration(Uint64 count, const TimeUnit& unit) :
		mCount(count),
		mUnit(unit)
	{
		/// Nothing
	}

	Duration Duration::As(const TimeUnit& unit) const
	{
		return { unit.ConvertFrom(mCount, mUnit), unit };
	}

	Uint64 Duration::GetCount() const
	{
		return mCount;
	}

	Uint64 Duration::GetCount(const TimeUnit& unit) const
	{
		return As(unit).GetCount();
	}

	const TimeUnit& Duration::GetUnit() const
	{
		return mUnit;
	}

	bool operator==(const Duration& lhs, const Duration& rhs)
	{
		const TimeUnit& lhsTimeUnit = lhs.GetUnit();
		const TimeUnit& rhsTimeUnit = rhs.GetUnit();
		const TimeUnit& smallerTimeUnit = lhsTimeUnit.IsSmaller(rhsTimeUnit) ? lhsTimeUnit : rhsTimeUnit;
		return lhs.As(smallerTimeUnit).GetCount() == rhs.As(smallerTimeUnit).GetCount();
	}

	bool operator!=(const Duration& lhs, const Duration& rhs)
	{
		return !(lhs == rhs);
	}

	Duration operator+(const Duration& lhs, const Duration& rhs)
	{
		Duration lhs1(lhs);
		return lhs1 += rhs;
	}

	Duration& operator+=(Duration& lhs, const Duration& rhs)
	{
		const TimeUnit& lhsTimeUnit = lhs.GetUnit();
		const TimeUnit& rhsTimeUnit = rhs.GetUnit();
		const TimeUnit& resultTimeUnit = lhsTimeUnit.IsSmaller(rhsTimeUnit) ? lhsTimeUnit : rhsTimeUnit;

		Uint64 resultCount = 0;
		resultCount += resultTimeUnit.ConvertFrom(lhs.GetCount(), lhsTimeUnit);
		resultCount += resultTimeUnit.ConvertFrom(rhs.GetCount(), rhsTimeUnit);

		lhs = Duration(resultCount, resultTimeUnit);
		return lhs;
;	}

	bool operator<(const Duration& lhs, const Duration& rhs)
	{
		const TimeUnit& lhsTimeUnit = lhs.GetUnit();
		const TimeUnit& rhsTimeUnit = rhs.GetUnit();
		const TimeUnit& smallerTimeUnit = lhsTimeUnit.IsSmaller(rhsTimeUnit) ? lhsTimeUnit : rhsTimeUnit;
		return lhs.As(smallerTimeUnit).GetCount() < rhs.As(smallerTimeUnit).GetCount();
	}

	bool operator<=(const Duration& lhs, const Duration& rhs)
	{
		return rhs >= lhs;
	}

	bool operator>(const Duration& lhs, const Duration& rhs)
	{
		return rhs < lhs;
	}

	bool operator>=(const Duration& lhs, const Duration& rhs)
	{
		return !(lhs < rhs);
	}
}
