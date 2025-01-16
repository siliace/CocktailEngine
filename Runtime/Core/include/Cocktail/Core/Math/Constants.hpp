#ifndef COCKTAIL_CORE_MATH_CONSTANTS_HPP
#define COCKTAIL_CORE_MATH_CONSTANTS_HPP

#include <cmath>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	struct Constants
	{
		/**
		 * \brief 
		 */
		static constexpr T Pi = static_cast<T>(3.141592654);
	};
}

#endif // COCKTAIL_CORE_MATH_CONSTANTS_HPP
