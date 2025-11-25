#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP

#include <Cocktail/Core/System/Joystick/JoystickManager.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class JoystickManager : public Ck::JoystickManager
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr unsigned int MaxJoystickCount = 16;

		/**
		 * \brief 
		 */
		JoystickManager();

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetDeviceCount() const override;

		/**
		 * \brief  
		 * \param index 
		 * \return 
		 */
		Joystick* GetDevice(unsigned index) const override;

	private:

		UniquePtr<Joystick> mJoysticks[MaxJoystickCount];
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP
