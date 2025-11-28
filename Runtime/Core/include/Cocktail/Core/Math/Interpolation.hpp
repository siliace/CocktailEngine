#ifndef COCKTAIL_CORE_MATH_INTERPOLATION_HPP
#define COCKTAIL_CORE_MATH_INTERPOLATION_HPP

#include <utility>

namespace Ck
{
    /**
     * \brief Perform a linear interpolation between two values
     *
     * \tparam T The type of value to interpole
     * \tparam U The type of the interpolation factor
     *
     * \param a The left value of the interpolation
     * \param b The right value of the interpolation
     * \param x The interpolation factor
     *
     * \return The interpolated value
     */
    template <typename T, typename U>
    T Lerp(const T& a, const T& b, U x)
    {
        return a * (1 - x) + b * x;
    }

    /**
     * \brief Perform a smooth interpolation between two values
    *
     * \tparam T The type of value to interpole
     * \tparam U The type of the interpolation factor
     *
     * \param a The left value of the interpolation
     * \param b The right value of the interpolation
     * \param x The interpolation factor
     *
     * \return The interpolated value
     */
    template <typename T, typename U>
    T Smoothstep(const T& a, const T& b, U x)
    {
        auto t = std::clamp((x - a) / (b - a), T(0), T(1));
        return t * t * (3 - 2 * t);
    }
}

#endif // COCKTAIL_CORE_MATH_INTERPOLATION_HPP