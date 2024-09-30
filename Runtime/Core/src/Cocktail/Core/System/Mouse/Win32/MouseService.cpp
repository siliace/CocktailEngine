#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Mouse/Win32/MouseService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		int ToWin32Type(MouseButton button)
		{
			switch (button)
			{
			case MouseButton::Left: return VK_LBUTTON;
			case MouseButton::Middle: return VK_MBUTTON;
			case MouseButton::Right: return VK_RBUTTON;
			case MouseButton::Extra1: return VK_XBUTTON1;
			case MouseButton::Extra2: return VK_XBUTTON2;
			}

			COCKTAIL_UNREACHABLE();
		}
	}

	bool MouseService::IsCursorVisible() const
	{
		CURSORINFO cursorInfo;
		if (GetCursorInfo(&cursorInfo) == FALSE)
			throw SystemError::GetLastError();

		return (cursorInfo.flags & CURSOR_SHOWING) > 0;
	}

	void MouseService::SetCursorVisible(bool visible)
	{
		if (visible)
		{
			while (ShowCursor(TRUE) < 1)
				;
		}
		else
		{
			while (ShowCursor(FALSE) > -1)
				;
		}
	}

	Extent2D<int> MouseService::GetCursorPosition() const
	{
		CURSORINFO cursorInfo;
		if (GetCursorInfo(&cursorInfo) == FALSE)
			throw SystemError::GetLastError();

		return MakeExtent<int>(cursorInfo.ptScreenPos.x, cursorInfo.ptScreenPos.y);
	}

	void MouseService::SetCursorPosition(const Extent2D<int>& position)
	{
		if (SetCursorPos(position.Width, position.Height) == FALSE)
			throw SystemError::GetLastError();
	}

	bool MouseService::IsPressedButton(MouseButton button) const
	{
		return GetAsyncKeyState(ToWin32Type(button)) & 1 << 15;
	}

	Signal<MouseMovedEvent>& MouseService::OnMouseMovedEvent()
	{
		return mOnMouseMovedEvent;
	}

	Signal<MouseButtonEvent>& MouseService::OnMouseButtonEvent()
	{
		return mOnMouseButtonEvent;
	}

	Signal<MouseWheelEvent>& MouseService::OnMouseWheelEvent()
	{
		return mOnMouseWheelEvent;
	}
}
