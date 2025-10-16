#include <Cocktail/Core/System/Window/Win32/WindowEventDispatcher.hpp>

namespace Ck::Detail::Win32
{
	void WindowEventDispatcher::Dispatch(Window* window, UINT msg, WPARAM wParam, LPARAM lParam) const
	{
		switch (msg)
		{
		case WM_CLOSE:
			{
				WindowCloseEvent event = CreateWindowEvent<WindowCloseEvent>(window);

				window->OnCloseEvent().Emit(event);
			}
			break;

		case WM_PAINT:
			{
				WindowRedrawEvent event = CreateWindowEvent<WindowRedrawEvent>(window);

				window->OnRedrawEvent().Emit(event);
			}
			break;

		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			{
				WindowFocusEvent event = CreateWindowEvent<WindowFocusEvent>(window);
				event.HasFocus = msg == WM_SETFOCUS;

				window->OnFocusEvent().Emit(event);
			}
			break;

		case WM_MOVE:
			{
				WindowMovedEvent event = CreateWindowEvent<WindowMovedEvent>(window);
				event.Position.Width = (int)(short)LOWORD(lParam);
				event.Position.Height = (int)(short)HIWORD(lParam);

				window->OnMovedEvent().Emit(event);
			}
			break;

		case WM_SIZE:
			{
				WindowResizedEvent event = CreateWindowEvent<WindowResizedEvent>(window);

				switch (wParam)
				{
				case SIZE_MAXIMIZED:
					event.DisplayMode = WindowDisplayMode::Maximized;
					break;

				case SIZE_MINIMIZED:
					event.DisplayMode = WindowDisplayMode::Minimized;
					break;

				default:
					event.DisplayMode = WindowDisplayMode::Normal;
					break;
				}

				event.Size.Width = LOWORD(lParam);
				event.Size.Height = HIWORD(lParam);

				window->OnResizedEvent().Emit(event);
			}
			break;

		case WM_MOUSEMOVE:
			{
				WindowMouseMoveEvent event = CreateWindowMouseEvent<WindowMouseMoveEvent>(window, wParam, lParam);
				event.RelativeDisplacement = event.Position - mMousePosition;
				window->OnMouseMovedEvent().Emit(event);

				mMousePosition = event.Position;
			}
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			{
				WindowMouseButtonEvent event = CreateWindowMouseEvent<WindowMouseButtonEvent>(window, LOWORD(wParam), lParam);
				event.Type = GetMessageClickType(msg);
				event.Button = GetMessageMouseButton(msg);

				window->OnMouseButtonEvent().Emit(event);
			}
			break;

		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_XBUTTONDBLCLK:
			{
				WindowMouseButtonEvent event = CreateWindowMouseEvent<WindowMouseButtonEvent>(window, LOWORD(wParam), lParam);
				event.Type = GetMessageClickType(msg);
				event.Button = HIWORD(wParam) == XBUTTON1 ? MouseButton::Extra1 : MouseButton::Extra2;

				window->OnMouseButtonEvent().Emit(event);
			}
			break;


		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
			{
				WindowMouseWheelEvent event = CreateWindowMouseEvent<WindowMouseWheelEvent>(window, LOWORD(wParam), lParam);
				event.Delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
				event.Wheel = msg == WM_MOUSEWHEEL ? MouseWheel::Vertical : MouseWheel::Horizontal;

				window->OnMouseWheelEvent().Emit(event);
			}
			break;

		case WM_CHAR:
			{
				WindowTextEvent event;
				event.Window = window;
				event.Unicode = wParam;

				window->OnTextEvent().Emit(event);
			}
			break;

		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			{
				WORD virtualKey = LOWORD(wParam);
				WORD keyFlags = HIWORD(lParam);

				WORD scanCode = LOBYTE(keyFlags);
				if ((keyFlags & KF_EXTENDED) == KF_EXTENDED)
					scanCode = MAKEWORD(scanCode, 0xE0);

				if (virtualKey == VK_SHIFT || virtualKey == VK_CONTROL || virtualKey == VK_MENU)
					virtualKey = LOWORD(MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX));

				WindowKeyboardEvent event;
				event.Window = window;
				event.Key = GetMessageKey(virtualKey);
				event.Pressed = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
				event.Repeated = (lParam & Bit(30)) != 0;

				window->OnKeyboardEvent().Emit(event);
			}
			break;

		case WM_GETMINMAXINFO:
			{
				MINMAXINFO* minmaxinfo = reinterpret_cast<MINMAXINFO*>(lParam);

				Extent2D<unsigned int> minSize = window->GetMinSize();
				if (minSize.Width && minSize.Height)
				{
					minmaxinfo->ptMinTrackSize.x = minSize.Width;
					minmaxinfo->ptMinTrackSize.y = minSize.Height;
				}

				Extent2D<unsigned int> maxSize = window->GetMaxSize();
				if (minSize.Width && minSize.Height)
				{
					minmaxinfo->ptMaxTrackSize.x = maxSize.Width;
					minmaxinfo->ptMaxTrackSize.y = maxSize.Height;
				}
			}
			break;
		}
	}

	MouseButtonEvent::ClickType WindowEventDispatcher::GetMessageClickType(UINT msg)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_XBUTTONDOWN:
			return MouseButtonEvent::ClickType::Pressed;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_XBUTTONUP:
			return MouseButtonEvent::ClickType::Released;

		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
			return MouseButtonEvent::ClickType::Double;
		}

		COCKTAIL_UNREACHABLE();
	}

	KeyboardKey WindowEventDispatcher::GetMessageKey(WPARAM wParam)
	{
		switch (wParam)
		{
		case VK_BACK: return KeyboardKey::Backspace;
		case VK_TAB: return KeyboardKey::Tabulation;
		case VK_CLEAR: return KeyboardKey::Clear;
		case VK_RETURN: return KeyboardKey::Enter;
		case VK_PAUSE: return KeyboardKey::Pause;
		case VK_CAPITAL: return KeyboardKey::CapsLock;
		case VK_ESCAPE: return KeyboardKey::Escape;
		case VK_SPACE: return KeyboardKey::Space;
		case VK_PRIOR: return KeyboardKey::Prior;
		case VK_NEXT: return KeyboardKey::Next;
		case VK_END: return KeyboardKey::End;
		case VK_HOME: return KeyboardKey::Home;
		case VK_LEFT: return KeyboardKey::Left;
		case VK_UP: return KeyboardKey::Up;
		case VK_RIGHT: return KeyboardKey::Right;
		case VK_DOWN: return KeyboardKey::Down;
		case VK_SELECT: return KeyboardKey::Select;
		case VK_PRINT: return KeyboardKey::Print;
		case VK_EXECUTE: return KeyboardKey::Execute;
		case VK_SNAPSHOT: return KeyboardKey::ScreenShot;
		case VK_INSERT: return KeyboardKey::Insert;
		case VK_DELETE: return KeyboardKey::Delete;
		case VK_HELP: return KeyboardKey::Help;
		case 'A': return KeyboardKey::A;
		case 'B': return KeyboardKey::B;
		case 'C': return KeyboardKey::C;
		case 'D': return KeyboardKey::D;
		case 'E': return KeyboardKey::E;
		case 'F': return KeyboardKey::F;
		case 'G': return KeyboardKey::G;
		case 'H': return KeyboardKey::H;
		case 'I': return KeyboardKey::I;
		case 'J': return KeyboardKey::J;
		case 'K': return KeyboardKey::K;
		case 'L': return KeyboardKey::L;
		case 'M': return KeyboardKey::M;
		case 'N': return KeyboardKey::N;
		case 'O': return KeyboardKey::O;
		case 'P': return KeyboardKey::P;
		case 'Q': return KeyboardKey::Q;
		case 'R': return KeyboardKey::R;
		case 'S': return KeyboardKey::S;
		case 'T': return KeyboardKey::T;
		case 'U': return KeyboardKey::U;
		case 'V': return KeyboardKey::V;
		case 'W': return KeyboardKey::W;
		case 'X': return KeyboardKey::X;
		case 'Y': return KeyboardKey::Y;
		case 'Z': return KeyboardKey::Z;
		case VK_LWIN: return KeyboardKey::LeftSystem;
		case VK_RWIN: return KeyboardKey::RightSystem;
		case VK_APPS: return KeyboardKey::App;
		case VK_SLEEP: return KeyboardKey::Sleep;
		case VK_NUMPAD0: return KeyboardKey::NumPad0;
		case VK_NUMPAD1: return KeyboardKey::NumPad1;
		case VK_NUMPAD2: return KeyboardKey::NumPad2;
		case VK_NUMPAD3: return KeyboardKey::NumPad3;
		case VK_NUMPAD4: return KeyboardKey::NumPad4;
		case VK_NUMPAD5: return KeyboardKey::NumPad5;
		case VK_NUMPAD6: return KeyboardKey::NumPad6;
		case VK_NUMPAD7: return KeyboardKey::NumPad7;
		case VK_NUMPAD8: return KeyboardKey::NumPad8;
		case VK_NUMPAD9: return KeyboardKey::NumPad9;
		case VK_MULTIPLY: return KeyboardKey::Multiply;
		case VK_ADD: return KeyboardKey::Add;
		case VK_SEPARATOR: return KeyboardKey::Separator;
		case VK_SUBTRACT: return KeyboardKey::Subtract;
		case VK_DECIMAL: return KeyboardKey::Decimal;
		case VK_DIVIDE: return KeyboardKey::Divide;
		case VK_F1: return KeyboardKey::F1;
		case VK_F2: return KeyboardKey::F2;
		case VK_F3: return KeyboardKey::F3;
		case VK_F4: return KeyboardKey::F4;
		case VK_F5: return KeyboardKey::F5;
		case VK_F6: return KeyboardKey::F6;
		case VK_F7: return KeyboardKey::F7;
		case VK_F8: return KeyboardKey::F8;
		case VK_F9: return KeyboardKey::F9;
		case VK_F10: return KeyboardKey::F10;
		case VK_F11: return KeyboardKey::F11;
		case VK_F12: return KeyboardKey::F12;
		case VK_F13: return KeyboardKey::F13;
		case VK_F14: return KeyboardKey::F14;
		case VK_F15: return KeyboardKey::F15;
		case VK_F16: return KeyboardKey::F16;
		case VK_F17: return KeyboardKey::F17;
		case VK_F18: return KeyboardKey::F18;
		case VK_F19: return KeyboardKey::F19;
		case VK_F20: return KeyboardKey::F20;
		case VK_F21: return KeyboardKey::F21;
		case VK_F22: return KeyboardKey::F22;
		case VK_F23: return KeyboardKey::F23;
		case VK_F24: return KeyboardKey::F24;
		case VK_NUMLOCK: return KeyboardKey::NumLock;
		case VK_SCROLL: return KeyboardKey::ScrollLock;
		case VK_LSHIFT: return KeyboardKey::LeftShift;
		case VK_RSHIFT: return KeyboardKey::RightShift;
		case VK_LCONTROL: return KeyboardKey::LeftControl;
		case VK_RCONTROL: return KeyboardKey::RightControl;
		case VK_LMENU: return KeyboardKey::LeftAlt;
		case VK_RMENU: return KeyboardKey::RightAlt;
		}

		COCKTAIL_UNREACHABLE();
	}

	MouseButton WindowEventDispatcher::GetMessageMouseButton(UINT msg)
	{
		switch (msg)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
			return MouseButton::Left;

		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
			return MouseButton::Middle;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
			return MouseButton::Right;
		}

		COCKTAIL_UNREACHABLE();
	}
}
