#ifndef COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP
#define COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	enum class JoystickPovPosition
	{
		/**
		 * \brief 
		 */
		Right = Bit(0),

		/**
		 * \brief 
		 */
		Left = Bit(1),

		/**
		 * \brief 
		 */
		Up = Bit(2),

		/**
		 * \brief 
		 */
		Down = Bit(3)
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP
