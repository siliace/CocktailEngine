#include <CocktailEngine/Core/Application/App.hpp>
#include <CocktailEngine/Core/System/Keyboard/KeyboardService.hpp>
#include <CocktailEngine/Core/System/Monitor/Win32/MonitorService.hpp>
#include <CocktailEngine/Core/System/Mouse/Win32/MouseService.hpp>
#include <CocktailEngine/Core/System/Window/Win32/ImageCursor.hpp>
#include <CocktailEngine/Core/System/Window/Win32/SystemCursor.hpp>
#include <CocktailEngine/Core/System/Window/Win32/Window.hpp>
#include <CocktailEngine/Core/System/Window/Win32/WindowEventDispatcher.hpp>
#include <CocktailEngine/Core/System/Window/Win32/WindowFactory.hpp>

namespace Ck::Detail::Win32
{
    namespace
    {
        LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            WindowEventDispatcher* dispatcher;
            if (msg == WM_CREATE)
            {
                CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
                Window* window = static_cast<Window*>(createStruct->lpCreateParams);

                dispatcher = new WindowEventDispatcher(window);

                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dispatcher));
            }
            else
            {
                dispatcher = reinterpret_cast<WindowEventDispatcher*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            }

            if (msg == WM_DISPLAYCHANGE)
                App::Resolve<MonitorService>()->RefreshTopology();

            if (dispatcher)
            {
                if (msg == WM_SETCURSOR && LOWORD(lParam) == HTCLIENT)
                {
                    Window* window = dispatcher->GetWindow();

                    if (!window->IsCursorVisible())
                    {
                        SetCursor(nullptr);
                    }
                    else if (Cursor* cursor = window->GetCursor())
                    {
                        SetCursor(static_cast<HCURSOR>(cursor->GetSystemHandle()));
                    }
                }
                else if (msg == WM_NCDESTROY)
                {
                    LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);

                    SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
                    delete dispatcher;

                    return result;
                }
                else
                {
                    dispatcher->Dispatch(msg, wParam, lParam);
                }
            }

            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }

    WindowFactory::WindowFactory(HINSTANCE instanceHandle) :
        mInstanceHandle(instanceHandle),
        mWindowClass(mInstanceHandle, MainWindowProc, CK_TEXT("cocktail-window-class"))
    {
        for (SystemCursorType cursorType : Enum<SystemCursorType>::Values)
        {
            if (!WindowFactory::IsSystemCursorSupported(cursorType))
                continue;

            mSystemCursors[cursorType] = MakeUnique<SystemCursor>(cursorType);
        }
    }

    UniquePtr<Ck::ImageCursor> WindowFactory::CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot)
    {
        return MakeUnique<ImageCursor>(image, hotspot);
    }

    UniquePtr<Ck::Window> WindowFactory::CreateWindow(const WindowCreateInfo& createInfo)
    {
        UniquePtr<Window> window = MakeUnique<Window>(createInfo, mWindowClass);

        window->Connect(window->OnKeyboardEvent(), [](const WindowKeyboardEvent& event) {
            App::Resolve<Ck::KeyboardService>()->OnKeyboardEvent().Emit({ event.Key, event.Pressed, event.Repeated });
        });

        window->Connect(window->OnMouseMovedEvent(), [](const WindowMouseMoveEvent& event) {
            App::Resolve<Ck::MouseService>()->OnMouseMovedEvent().Emit({ { event.Flags, event.Position }, event.RelativeDisplacement });
        });

        window->Connect(window->OnMouseButtonEvent(), [](const WindowMouseButtonEvent& event) {
            App::Resolve<Ck::MouseService>()->OnMouseButtonEvent().Emit({ { event.Flags, event.Position }, event.Type, event.Button });
        });

        window->Connect(window->OnMouseWheelEvent(), [](const WindowMouseWheelEvent& event) {
            App::Resolve<Ck::MouseService>()->OnMouseWheelEvent().Emit({ { event.Flags, event.Position }, event.Wheel });
        });

        return window;
    }

    Ck::SystemCursor* WindowFactory::LoadSystemCursor(SystemCursorType type)
    {
        return mSystemCursors[type].Get();
    }

    bool WindowFactory::IsSystemCursorSupported(SystemCursorType systemCursorType) const
    {
        return true;
    }
}
