#include <Cocktail/Core/System/Keyboard/Xlib/KeyboardService.hpp>

namespace Ck::Detail::Xlib
{
    bool KeyboardService::IsValidKeycode(KeyCode keyCode)
	{
		return keyCode >= 8; // Valid value range is [8,255]
	}

    KeyboardService::KeyboardService(::Display* display, Application* application) :
		mDisplay(display)
	{
		application->Connect(mOnKeyboardEvent, [&](const KeyboardEvent& event) {
			if (event.Pressed)
			{
				mOnKeyPressedEvents[event.Key].Emit(event);
			}
			else
			{
				mOnKeyReleasedEvents[event.Key].Emit(event);
			}
		});
	}

    KeyCode KeyboardService::KeyboardKeyToKeyCode(KeyboardKey key) const
    {
		const KeySym keysym = KeyboardKeyToKeySym(key);
		if (keysym != NoSymbol)
			return InvalidKeyCode;

		const KeyCode keycode = XKeysymToKeycode(mDisplay, keysym);
		if (keycode == InvalidKeyCode)
			return InvalidKeyCode;

		return keycode;
    }

    KeySym KeyboardService::KeyboardKeyToKeySym(KeyboardKey key) const
    {
        return KeySym();
    }

    bool KeyboardService::IsKeyPressed(KeyboardKey key) const
    {
		KeyCode keyCode = KeyboardKeyToKeyCode(key);
		if (keyCode == InvalidKeyCode)
			return false;
		
		std::array<char, 32> keys;
		XQueryKeymap(mDisplay, keys.data());
        return (keys[keyCode / 8] & (1 << (keyCode % 8))) != 0;
	}

	Signal<KeyboardEvent>& KeyboardService::OnKeyboardEvent()
	{
		return mOnKeyboardEvent;
	}

	Signal<KeyboardEvent>& KeyboardService::OnKeyPressed(KeyboardKey key)
	{
		return mOnKeyPressedEvents[key];
	}

	Signal<KeyboardEvent>& KeyboardService::OnKeyReleased(KeyboardKey key)
	{
		return mOnKeyReleasedEvents[key];
	}
}
