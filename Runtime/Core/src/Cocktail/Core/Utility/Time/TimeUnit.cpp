#include <cassert>
#include <tuple>

#include <Cocktail/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
	TimeUnit TimeUnit::Nanoseconds(Uint64 repeat)
	{
		return { repeat, 1'000'000'000 };
	}

	TimeUnit TimeUnit::Microseconds(Uint64 repeat)
	{
		return { repeat, 1'000'000 };
	}

	TimeUnit TimeUnit::Milliseconds(Uint64 repeat)
	{
		return { repeat, 1'000 };
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
		float seconds = static_cast<float>(count) * static_cast<float>(mRatioNumerator);
		seconds /= static_cast<float>(mRatioDenominator);

		float result = seconds * static_cast<float>(destinationUnit.mRatioNumerator);
		return result / static_cast<float>(destinationUnit.mRatioDenominator);
	}

	Uint64 TimeUnit::ConvertFrom(Uint64 count, const TimeUnit& sourceUnit) const
	{
		float seconds = static_cast<float>(count) * static_cast<float>(sourceUnit.mRatioNumerator);
		seconds /= static_cast<float>(sourceUnit.mRatioDenominator);

		float result = seconds * static_cast<float>(mRatioDenominator);
		return result / static_cast<float>(mRatioNumerator);
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
