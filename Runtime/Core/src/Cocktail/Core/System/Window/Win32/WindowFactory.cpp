#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>
#include <Cocktail/Core/System/Mouse/Win32/MouseService.hpp>
#include <Cocktail/Core/System/Window/Win32/ImageCursor.hpp>
#include <Cocktail/Core/System/Window/Win32/SystemCursor.hpp>
#include <Cocktail/Core/System/Window/Win32/Window.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowEventDispatcher.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowFactory.hpp>

namespace Ck::Detail::Win32
{
	namespace
	{
		LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			Window* window;
			if (msg == WM_CREATE)
			{
				CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
				window = static_cast<Window*>(createStruct->lpCreateParams);

				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			}
			else
			{
				window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			}

			if (window)
			{
				if (msg == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT)
				{
					if (!window->IsCursorVisible())
					{
						SetCursor(nullptr);
					}
					else if (Ref<Cursor> cursor = window->GetCursor())
					{
						SetCursor(static_cast<HCURSOR>(cursor->GetSystemHandle()));
					}
				}
				else
				{
					App::Resolve<WindowEventDispatcher>()->Dispatch(window, msg, wParam, lParam);
				}
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
	}

	WindowFactory::WindowFactory(HINSTANCE instanceHandle) :
		mInstanceHandle(instanceHandle),
		mWindowClass(mInstanceHandle, MainWindowProc, "cocktail-window-class")
	{
		for (SystemCursorType cursorType : Enum<SystemCursorType>::Values)
		{
			if (!WindowFactory::IsSystemCursorSupported(cursorType))
				continue;
				
			mSystemCursors[cursorType] = SystemCursor::New(cursorType);
		}
	}

	Ref<Ck::ImageCursor> WindowFactory::CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot)
	{
		return ImageCursor::New(image, hotspot);
	}

	Ref<Ck::Window> WindowFactory::CreateWindow(const WindowCreateInfo& createInfo)
	{
		Ref<Window> window = Window::New(createInfo, mWindowClass);

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

	Ref<Ck::SystemCursor> WindowFactory::LoadSystemCursor(SystemCursorType type)
	{
		return mSystemCursors[type];
	}

	bool WindowFactory::IsSystemCursorSupported(SystemCursorType systemCursorType) const
	{
		return true;
	}

	Signal<Ref<Ck::Window>>& WindowFactory::OnWindowCreated()
	{
		return mOnWindowCreated;
	}
}
