#include <cassert>
#include <tuple>

#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
	TimeUnit TimeUnit::Nanoseconds(Uint64 repeat)
	{
		return { repeat, 1000000000 };
	}

	TimeUnit TimeUnit::Microseconds(Uint64 repeat)
	{
		return { repeat, 1000000 };
	}

	TimeUnit TimeUnit::Milliseconds(Uint64 repeat)
	{
		return { repeat, 1000 };
	}

	TimeUnit TimeUnit::Seconds(Uint64 repeat)
	{
		return { repeat, 1 };
	}

	TimeUnit TimeUnit::Minutes(Uint64 repeat)
	{
		return { 60 * repeat, 1 };
	}

	TimeUnit TimeUnit::Hours(Uint64 repeat)
	{
		return { 3'600 * repeat, 1 };
	}

	TimeUnit TimeUnit::Days(Uint64 repeat)
	{
		return { 86'400 * repeat, 1 };
	}

	TimeUnit::TimeUnit(Uint64 ratioNumerator, Uint64 ratioDenominator) :
		mRatioNumerator(ratioNumerator),
		mRatioDenominator(ratioDenominator)
	{
		assert(mRatioNumerator != 0);
		assert(mRatioDenominator != 0);
	}

	bool TimeUnit::IsBigger(const TimeUnit& other) const
	{
		if (mRatioDenominator < other.mRatioDenominator)
			return true;

		return mRatioNumerator > other.mRatioNumerator;
	}

	bool TimeUnit::IsSmaller(const TimeUnit& other) const
	{
		if (mRatioDenominator > other.mRatioDenominator)
			return true;

		return mRatioNumerator < other.mRatioNumerator;
	}

	Uint64 TimeUnit::ConvertTo(Uint64 count, const TimeUnit& destinationUnit) const
	{
		Uint64 seconds = count * mRatioNumerator;
		seconds /= mRatioDenominator;

		Uint64 result = seconds * destinationUnit.mRatioNumerator;
		return result / destinationUnit.mRatioDenominator;
	}

	Uint64 TimeUnit::ConvertFrom(Uint64 count, const TimeUnit& sourceUnit) const
	{
		if (*this == sourceUnit)
			return count;

		Uint64 seconds = count * sourceUnit.mRatioNumerator;
		seconds /= sourceUnit.mRatioDenominator;

		Uint64 result = seconds * mRatioDenominator;
		return result / mRatioNumerator;
	}

	bool TimeUnit::operator==(const TimeUnit& rhs) const
	{
		return std::tie(mRatioNumerator, mRatioDenominator) ==
			std::tie(rhs.mRatioNumerator, rhs.mRatioDenominator);
	}

	bool TimeUnit::operator!=(const TimeUnit& rhs) const
	{
		return !(*this == rhs);
	}
}
