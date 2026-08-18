#ifndef COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKEVENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKEVENT_HPP

#include <CocktailEngine/Core/Flags.hpp>
#include <CocktailEngine/Core/System/Joystick/JoystickAxis.hpp>
#include <CocktailEngine/Core/System/Joystick/JoystickPovPosition.hpp>

namespace Ck
{
	class Joystick;

	/**
	 * \brief Base struct for Joystick events
	 */
	struct JoystickEvent
	{
		/**
		 * \brief The Joystick involved by the event
		 */
		Joystick* Source = nullptr;
	};

	/**
	 * \brief Event fired when a button of a joystick is pressed or released
	 */
	struct JoystickButtonEvent : JoystickEvent
	{
		/**
		 * \brief Specifies the button involved by the event
		 */
		unsigned int Button = 0;

		/**
		 * \brief Specifies whether the \p Button has been pressed or released
		 */
		bool Pressed = false;
	};

	/**
	 * \brief Event fired when an axis of a joystick is moved
	 */
	struct JoystickAxisEvent : JoystickEvent
	{
		/**
		 * \brief The new position of the axis
		 */
		unsigned int Position = 0;

		/**
		 * \brief The axis moved
		 */
		JoystickAxis Axis = JoystickAxis::X;
	};

	/**
	 * \brief Event fired when the POV hat of a joystick is moved
	 */
	struct JoystickPovEvent : JoystickEvent
	{
		/**
		 * \brief Specifies the new position of the POV hat
		 */
		Flags<JoystickPovPosition> Position;
	};
}

#endif // COCKTAILENGINE_CORE_SYSTEM_JOYSTICK_JOYSTICKEVENT_HPP
