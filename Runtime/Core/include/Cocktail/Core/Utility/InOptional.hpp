#ifndef COCKTAIL_CORE_UTILITY_INOPTIONAL_HPP
#define COCKTAIL_CORE_UTILITY_INOPTIONAL_HPP

#include <optional>

namespace Ck
{
	/**
	 * \brief InOptional tag
	 */
	struct InOptionalTag{};

	/**
	 * \brief Helper constant for function overload wrap the return value into an optional
	 */
	static constexpr InOptionalTag InOptional;
}

#endif // COCKTAIL_CORE_UTILITY_INOPTIONAL_HPP
