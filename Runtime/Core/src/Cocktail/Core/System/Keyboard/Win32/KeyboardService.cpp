#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Keyboard/Win32/KeyboardService.hpp>

namespace Ck::Detail::Win32
{
	namespace 
	{
		int ToWin32Type(KeyboardKey key)
		{
			switch (key)
			{
			case KeyboardKey::Backspace: return VK_BACK;
			case KeyboardKey::Tabulation: return VK_TAB;
			case KeyboardKey::Clear: return VK_CLEAR;
			case KeyboardKey::Enter: return VK_RETURN;
			case KeyboardKey::Pause: return VK_PAUSE;
			case KeyboardKey::CapsLock: return VK_CAPITAL;
			case KeyboardKey::Escape: return VK_ESCAPE;
			case KeyboardKey::Space: return VK_SPACE;
			case KeyboardKey::Prior: return VK_PRIOR;
			case KeyboardKey::Next: return VK_NEXT;
			case KeyboardKey::End: return VK_END;
			case KeyboardKey::Home: return VK_HOME;
			case KeyboardKey::Left: return VK_LEFT;
			case KeyboardKey::Up: return VK_UP;
			case KeyboardKey::Right: return VK_RIGHT;
			case KeyboardKey::Down: return VK_DOWN;
			case KeyboardKey::Select: return VK_SELECT;
			case KeyboardKey::Print: return VK_PRINT;
			case KeyboardKey::Execute: return VK_EXECUTE;
			case KeyboardKey::ScreenShot: return VK_SNAPSHOT;
			case KeyboardKey::Insert: return VK_INSERT;
			case KeyboardKey::Delete: return VK_DELETE;
			case KeyboardKey::Help: return VK_HELP;
			case KeyboardKey::A: return 'A';
			case KeyboardKey::B: return 'B';
			case KeyboardKey::C: return 'C';
			case KeyboardKey::D: return 'D';
			case KeyboardKey::E: return 'E';
			case KeyboardKey::F: return 'F';
			case KeyboardKey::G: return 'G';
			case KeyboardKey::H: return 'H';
			case KeyboardKey::I: return 'I';
			case KeyboardKey::J: return 'J';
			case KeyboardKey::K: return 'K';
			case KeyboardKey::L: return 'L';
			case KeyboardKey::M: return 'M';
			case KeyboardKey::N: return 'N';
			case KeyboardKey::O: return 'O';
			case KeyboardKey::P: return 'P';
			case KeyboardKey::Q: return 'Q';
			case KeyboardKey::R: return 'R';
			case KeyboardKey::S: return 'S';
			case KeyboardKey::T: return 'T';
			case KeyboardKey::U: return 'U';
			case KeyboardKey::V: return 'V';
			case KeyboardKey::W: return 'W';
			case KeyboardKey::X: return 'X';
			case KeyboardKey::Y: return 'Y';
			case KeyboardKey::Z: return 'Z';
			case KeyboardKey::LeftSystem: return VK_LWIN;
			case KeyboardKey::RightSystem: return VK_RWIN;
			case KeyboardKey::App: return VK_APPS;
			case KeyboardKey::Sleep: return VK_SLEEP;
			case KeyboardKey::NumPad0: return VK_NUMPAD0;
			case KeyboardKey::NumPad1: return VK_NUMPAD1;
			case KeyboardKey::NumPad2: return VK_NUMPAD2;
			case KeyboardKey::NumPad3: return VK_NUMPAD3;
			case KeyboardKey::NumPad4: return VK_NUMPAD4;
			case KeyboardKey::NumPad5: return VK_NUMPAD5;
			case KeyboardKey::NumPad6: return VK_NUMPAD6;
			case KeyboardKey::NumPad7: return VK_NUMPAD7;
			case KeyboardKey::NumPad8: return VK_NUMPAD8;
			case KeyboardKey::NumPad9: return VK_NUMPAD9;
			case KeyboardKey::Multiply: return VK_MULTIPLY;
			case KeyboardKey::Add: return VK_ADD;
			case KeyboardKey::Separator: return VK_SEPARATOR;
			case KeyboardKey::Subtract: return VK_SUBTRACT;
			case KeyboardKey::Decimal: return VK_DECIMAL;
			case KeyboardKey::Divide: return VK_DIVIDE;
			case KeyboardKey::F1: return VK_F1;
			case KeyboardKey::F2: return VK_F2;
			case KeyboardKey::F3: return VK_F3;
			case KeyboardKey::F4: return VK_F4;
			case KeyboardKey::F5: return VK_F5;
			case KeyboardKey::F6: return VK_F6;
			case KeyboardKey::F7: return VK_F7;
			case KeyboardKey::F8: return VK_F8;
			case KeyboardKey::F9: return VK_F9;
			case KeyboardKey::F10: return VK_F10;
			case KeyboardKey::F11: return VK_F11;
			case KeyboardKey::F12: return VK_F12;
			case KeyboardKey::F13: return VK_F13;
			case KeyboardKey::F14: return VK_F14;
			case KeyboardKey::F15: return VK_F15;
			case KeyboardKey::F16: return VK_F16;
			case KeyboardKey::F17: return VK_F17;
			case KeyboardKey::F18: return VK_F18;
			case KeyboardKey::F19: return VK_F19;
			case KeyboardKey::F20: return VK_F20;
			case KeyboardKey::F21: return VK_F21;
			case KeyboardKey::F22: return VK_F22;
			case KeyboardKey::F23: return VK_F23;
			case KeyboardKey::F24: return VK_F24;
			case KeyboardKey::NumLock: return VK_NUMLOCK;
			case KeyboardKey::ScrollLock: return VK_SCROLL;
			case KeyboardKey::LeftShift: return VK_LSHIFT;
			case KeyboardKey::RightShift: return VK_RSHIFT;
			case KeyboardKey::LeftControl: return VK_LCONTROL;
			case KeyboardKey::RightControl: return VK_RCONTROL;
			case KeyboardKey::LeftAlt: return VK_LMENU;
			case KeyboardKey::RightAlt: return VK_RMENU;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	KeyboardService::KeyboardService(Application* application)
	{
		application->Connect(KeyboardService::OnKeyboardEvent(), [&](const KeyboardEvent& event) {
			if (event.Pressed)
			{
				OnKeyPressed(event.Key).Emit(event);
			}
			else
			{
				OnKeyReleased(event.Key).Emit(event);
			}
		});
	}

	bool KeyboardService::IsKeyPressed(KeyboardKey key) const
	{
		return GetAsyncKeyState(ToWin32Type(key)) & 1 << 15;
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
