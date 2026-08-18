#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Joystick/Win32/Joystick.hpp>
#include <CocktailEngine/Core/System/Joystick/Win32/JoystickManager.hpp>

namespace Ck::Detail::Win32
{
	JoystickManager::JoystickManager()
	{
		for (unsigned int i = 0; i < MaxJoystickCount; i++)
			mJoysticks[i] = MakeUnique<Joystick>(i);
	}

	unsigned int JoystickManager::GetDeviceCount() const
	{
		return joyGetNumDevs();
	}

	Joystick* JoystickManager::GetDevice(unsigned int index) const
	{
		assert(index < MaxJoystickCount);
		return mJoysticks[index].Get();
	}
}
