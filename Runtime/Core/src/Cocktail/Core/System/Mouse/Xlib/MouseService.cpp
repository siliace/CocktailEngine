#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Mouse/Xlib/MouseService.hpp>

namespace Ck::Detail::Xlib
{
    MouseService::MouseService(::Display *display, Application* application) :
		mDisplay(display)
    {
		application->Connect(MouseService::OnMouseButtonEvent(), [&](const MouseButtonEvent& event) {
			switch (event.Type)
			{
			case MouseButtonEvent::ClickType::Double:
			case MouseButtonEvent::ClickType::Pressed:
				OnMouseButtonPressedEvent(event.Button).Emit(event);
				break;


			case MouseButtonEvent::ClickType::Released:
				OnMouseButtonReleasedEvent(event.Button).Emit(event);
				break;
			}
		});

		application->Connect(MouseService::OnMouseWheelEvent(), [&](const MouseWheelEvent& event) {
			OnMouseWheelEvent(event.Wheel).Emit(event);
		});
    }

    bool MouseService::IsCursorVisible() const
	{
		return false;
	}

	void MouseService::SetCursorVisible(bool visible)
	{
	}

	Extent2D<int> MouseService::GetCursorPosition() const
	{
        int x, y;
        int rootX, rootY;
        ::Window root, child;
        unsigned int buttons;

		XQueryPointer(mDisplay, DefaultRootWindow(mDisplay), &root, &child, &rootX, &rootY, &x, &y, &buttons);
		
		return {x, y}; 
	}

	void MouseService::SetCursorPosition(const Extent2D<int>& position)
	{
		XWarpPointer(mDisplay, 0, DefaultRootWindow(mDisplay), 0, 0, 0, 0, position.Width, position.Height);
	}

	bool MouseService::IsPressedButton(MouseButton button) const
	{
		int x, y;
        int rootX, rootY;
        ::Window root, child;
        unsigned int buttons;

		XQueryPointer(mDisplay, DefaultRootWindow(mDisplay), &root, &child, &rootX, &rootY, &x, &y, &buttons);

		switch(button)
    	{
        case MouseButton::Left:
			return buttons & Button1Mask;
        
		case MouseButton::Middle:
            return buttons & Button2Mask;
        
		case MouseButton::Right:
            return buttons & Button3Mask;
        
		case MouseButton::Extra1:
            return buttons & Button4Mask;
        
		case MouseButton::Extra2:
            return buttons & Button5Mask;
        }

		return false;
	}

	Signal<MouseMovedEvent>& MouseService::OnMouseMovedEvent()
	{
		return mOnMouseMovedEvent;
	}

	Signal<MouseButtonEvent>& MouseService::OnMouseButtonEvent()
	{
		return mOnMouseButtonEvent;
	}

	Signal<MouseButtonEvent>& MouseService::OnMouseButtonPressedEvent(MouseButton button)
	{
		return mOnButtonPressedEvents[button];
	}

	Signal<MouseButtonEvent>& MouseService::OnMouseButtonReleasedEvent(MouseButton button)
	{
		return mOnButtonReleasedEvents[button];
	}

	Signal<MouseWheelEvent>& MouseService::OnMouseWheelEvent()
	{
		return mOnMouseWheelEvent;
	}

	Signal<MouseWheelEvent>& MouseService::OnMouseWheelEvent(MouseWheel wheel)
	{
		return mOnMouseWheelEvents[wheel];
	}
}
