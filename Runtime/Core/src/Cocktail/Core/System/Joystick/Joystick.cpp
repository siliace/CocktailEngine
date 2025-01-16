#include <Cocktail/Core/System/Joystick/Joystick.hpp>

namespace Ck
{
	unsigned int Joystick::GetDeviceCount()
	{
		return ResolveFacadeInstance()->GetDeviceCount();
	}

	Joystick* Joystick::GetDevice(unsigned int index)
	{
		return ResolveFacadeInstance()->GetDevice(index);
	}
}
