#include <Cocktail/Core/System/Mouse/Mouse.hpp>

namespace Ck
{
	bool Mouse::IsCursorVisible()
	{
		return ResolveFacadeInstance()->IsCursorVisible();
	}

	void Mouse::SetCursorVisible(bool visible)
	{
		ResolveFacadeInstance()->SetCursorVisible(visible);
	}

	Extent2D<int> Mouse::GetCursorPosition()
	{
		return ResolveFacadeInstance()->GetCursorPosition();
	}

	void Mouse::SetCursorPosition(const Extent2D<int>& position)
	{
		ResolveFacadeInstance()->SetCursorPosition(position);
	}

	bool Mouse::IsPressedButton(MouseButton button)
	{
		return ResolveFacadeInstance()->IsPressedButton(button);
	}

	Signal<MouseMovedEvent>& Mouse::OnMouseMovedEvent()
	{
		return ResolveFacadeInstance()->OnMouseMovedEvent();
	}

	Signal<MouseButtonEvent>& Mouse::OnMouseButtonEvent()
	{
		return ResolveFacadeInstance()->OnMouseButtonEvent();
	}

	Signal<MouseWheelEvent>& Mouse::OnMouseWheelEvent()
	{
		return ResolveFacadeInstance()->OnMouseWheelEvent();
	}
}
