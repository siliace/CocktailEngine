#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>
#include <Cocktail/Core/System/Mouse/Mouse.hpp>
#include <Cocktail/Core/System/Window/Xlib/ImageCursor.hpp>
#include <Cocktail/Core/System/Window/Xlib/SystemCursor.hpp>
#include <Cocktail/Core/System/Window/Xlib/Window.hpp>
#include <Cocktail/Core/System/Window/Xlib/WindowFactory.hpp>

namespace Ck::Detail::Xlib
{
    WindowFactory::WindowFactory(::Display* display) :  
        mDisplay(display)
    {
        for (SystemCursorType cursorType : Enum<SystemCursorType>::Values)
        {
            if (IsSystemCursorSupported(cursorType))
                mSystemCursors[cursorType] = std::make_shared<SystemCursor>(mDisplay, cursorType);
        }
    } 

    std::shared_ptr<Ck::ImageCursor> WindowFactory::CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot)
    {
        return std::make_shared<ImageCursor>(mDisplay, image, hotspot);
    }

    std::shared_ptr<Ck::Window> WindowFactory::CreateWindow(const WindowCreateInfo& createInfo)
    {
        std::shared_ptr<Window> window = std::make_shared<Window>(mDisplay, createInfo);

        window->Connect(window->OnKeyboardEvent(), [](const WindowKeyboardEvent& event) {
			App::Resolve<Ck::KeyboardService>()->OnKeyboardEvent().Emit({
				event.Key, event.Pressed, event.Repeated
			});
		});

		window->Connect(window->OnMouseMovedEvent(), [](const WindowMouseMoveEvent& event) {
			App::Resolve<Ck::MouseService>()->OnMouseMovedEvent().Emit({
				{event.Flags, event.Position}, event.RelativeDisplacement
			});
		});

		window->Connect(window->OnMouseButtonEvent(), [](const WindowMouseButtonEvent& event) {
			App::Resolve<Ck::MouseService>()->OnMouseButtonEvent().Emit({
				{event.Flags, event.Position}, event.Type, event.Button
			});
		});

		window->Connect(window->OnMouseWheelEvent(), [](const WindowMouseWheelEvent& event) {
			App::Resolve<Ck::MouseService>()->OnMouseWheelEvent().Emit({
				{event.Flags, event.Position}, event.Wheel
			});
		});

        mOnWindowCreated.Emit(window);

        return window;
    }

	std::shared_ptr<Ck::SystemCursor> WindowFactory::LoadSystemCursor(SystemCursorType type)
    {
		return mSystemCursors[type];
    }

    bool WindowFactory::IsSystemCursorSupported(SystemCursorType cursorType) const
    {
        return cursorType != SystemCursorType::SizeTopLeftBottomRight && cursorType != SystemCursorType::SizeBottomLeftTopRight;
    }

    Signal<std::shared_ptr<Ck::Window>>& WindowFactory::OnWindowCreated() 
    {
        return mOnWindowCreated;
    }

    ::Display* WindowFactory::GetDisplay() const
    {
        return mDisplay;
    }
}
