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
		Centered = Bit(0),

		/**
		 * \brief 
		 */
		Right = Bit(1),

		/**
		 * \brief 
		 */
		Left = Bit(2),

		/**
		 * \brief 
		 */
		Up = Bit(3),

		/**
		 * \brief 
		 */
		Down = Bit(4)
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKPOVPOSITION_HPP
