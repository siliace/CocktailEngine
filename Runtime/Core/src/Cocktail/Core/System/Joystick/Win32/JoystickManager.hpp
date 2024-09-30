#ifndef COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Joystick/JoystickManager.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class JoystickManager : public Inherit<JoystickManager, Object, Ck::JoystickManager>
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

		Ref<Joystick> mJoysticks[MaxJoystickCount];
	};
}

#endif // COCKTAIL_CORE_SYSTEM_JOYSTICK_WIN32_JOYSTICKMANAGER_HPP
