#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKCAPABILITIES_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKCAPABILITIES_HPP

#include <Cocktail/Core/System/Joystick/JoystickAxis.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

namespace Ck
{
	/**
	 * \brief Structure describing axes and buttons supported by a joystick
	 */
	struct JoystickCapabilities
	{
		/**
		 * \brief Structure describing the range of values possible for a single axis
		 */
		struct AxisInfo
		{
			/**
			 * \brief 
			 */
			unsigned int Min = 0;

			/**
			 * \brief 
			 */
			unsigned int Max = 0;

			/**
			 * \brief 
			 */
			bool Supported = false;
		};

		/**
		 * \brief Specifies the number of buttons of the joystick
		 */
		unsigned int ButtonCount = 0;

		/**
		 * \brief Specifies the number of axes of the joystick
		 */
		unsigned int AxisCount = 0;

		/**
		 * \brief Specifies axes info of the joystick
		 */
		EnumMap<JoystickAxis, AxisInfo> Axes;

		/**
		 * \brief Specifies whether the joystick has a POV hat
		 */
		bool SupportHatPov = false;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKCAPABILITIES_HPP
