#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP

#include <Cocktail/Core/Cocktail.hpp>

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

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP
