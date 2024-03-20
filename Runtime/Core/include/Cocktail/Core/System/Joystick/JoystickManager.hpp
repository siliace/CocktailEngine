#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKMANAGER_HPP

#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck
{
	class Joystick;

	/**
	 * \brief 
	 */
	class JoystickManager : public Implements<JoystickManager, Interface>
	{
	public:

		/**
		 * \brief Get the number of joystick plugged-in
		 * \return The number of joystick
		 */
		virtual unsigned int GetDeviceCount() const = 0;

		/**
		 * \brief Get a joystick from its index
		 * \param index The index of the joystick to get
		 * \return The joystick it its exists, nullptr otherwise
		 */
		virtual Joystick* GetDevice(unsigned int index) const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_JOYSTICKMANAGER_HPP
