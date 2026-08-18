#include <cassert>
#include <tuple>

#include <CocktailEngine/Core/Utility/Time/TimeUnit.hpp>

namespace Ck
{
    namespace
    {
        Uint64 GreatestCommonDivisor(Uint64 lhs, Uint64 rhs)
        {
            while (rhs != 0)
            {
                const Uint64 remainder = lhs % rhs;
                lhs = rhs;
                rhs = remainder;
            }

            return lhs;
        }

        /**
         * \brief Computes count * numerator / denominator using integer arithmetic
         *
         * The fraction and the count are reduced before being multiplied, and the
         * product is split into a quotient and a remainder, so that intermediate
         * values stay as small as possible. Going through a floating point type
         * instead would lose precision as soon as the operands exceed the mantissa.
         *
         * \param count Value to scale
         * \param numerator Numerator of the scaling factor
         * \param denominator Denominator of the scaling factor
         *
         * \return The scaled count, truncated towards zero
         */
        Uint64 ScaleCount(Uint64 count, Uint64 numerator, Uint64 denominator)
        {
            assert(denominator != 0);

            Uint64 divisor = GreatestCommonDivisor(numerator, denominator);
            numerator /= divisor;
            denominator /= divisor;

            divisor = GreatestCommonDivisor(count, denominator);
            count /= divisor;
            denominator /= divisor;

            if (denominator == 1)
                return count * numerator;

            const Uint64 quotient = count / denominator;
            const Uint64 remainder = count % denominator;

            return quotient * numerator + remainder * numerator / denominator;
        }
    }

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

        /// Reduced on the way in, so that two units describing the same quantum hold
        /// the same pair and compare equal, and so that the cross products below stay
        /// as small as the quantum allows
        const Uint64 divisor = GreatestCommonDivisor(mRatioNumerator, mRatioDenominator);
        mRatioNumerator /= divisor;
        mRatioDenominator /= divisor;
    }

    bool TimeUnit::IsMultipleOf(const TimeUnit& other) const
    {
        if (other.mRatioNumerator == 0 || other.mRatioDenominator == 0)
            return false;

        const Uint64 lhs = mRatioNumerator * other.mRatioDenominator;
        const Uint64 rhs = mRatioDenominator * other.mRatioNumerator;

        return lhs % rhs == 0;
    }

    bool TimeUnit::IsBigger(const TimeUnit& other) const
    {
        return other.IsSmaller(*this);
    }

    bool TimeUnit::IsSmaller(const TimeUnit& other) const
    {
        /// Comparing the quanta means comparing mRatioNumerator / mRatioDenominator
        /// against the ratio of the other unit, which the cross product does without
        /// dividing. Looking at either component on its own says nothing: a unit can
        /// hold both a larger numerator and a larger denominator than another.
        return mRatioNumerator * other.mRatioDenominator < other.mRatioNumerator * mRatioDenominator;
    }

    Uint64 TimeUnit::ConvertTo(Uint64 count, const TimeUnit& destinationUnit) const
    {
        /// count * (mRatioNumerator / mRatioDenominator) seconds expressed in
        /// destinationUnit quanta, which is count scaled by the ratio of both units
        return ScaleCount(count, mRatioNumerator * destinationUnit.mRatioDenominator, mRatioDenominator * destinationUnit.mRatioNumerator);
    }

    Uint64 TimeUnit::ConvertFrom(Uint64 count, const TimeUnit& sourceUnit) const
    {
        return sourceUnit.ConvertTo(count, *this);
    }

    bool TimeUnit::operator==(const TimeUnit& rhs) const
    {
        return std::tie(mRatioNumerator, mRatioDenominator) == std::tie(rhs.mRatioNumerator, rhs.mRatioDenominator);
    }

    bool TimeUnit::operator!=(const TimeUnit& rhs) const
    {
        return !(*this == rhs);
    }
}
