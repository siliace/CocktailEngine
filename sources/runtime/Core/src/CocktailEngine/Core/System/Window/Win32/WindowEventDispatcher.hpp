#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP

#include <type_traits>

#include <CocktailEngine/Core/System/Window/Win32/Window.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief
     */
    class WindowEventDispatcher
    {
    public:

        /**
         * \brief Creates a dispatcher for a window.
         *
         * \param window Non-owning window pointer that must outlive the dispatcher.
         */
        explicit WindowEventDispatcher(Window* window);

        /**
         * \brief Translates a native Windows message into the associated window events.
         *
         * \param msg Native Windows message identifier.
         * \param wParam Message-specific first parameter.
         * \param lParam Message-specific second parameter.
         */
        void Dispatch(UINT msg, WPARAM wParam, LPARAM lParam);

        /**
         * \brief Retrieves the window receiving the dispatched messages.
         *
         * \return The non-owning associated window pointer.
         */
        Window* GetWindow() const;

    private:

        /**
         * \brief
         * \param msg
         * \return
         */
        static MouseButtonEvent::ClickType GetMessageClickType(UINT msg);

        /**
         * \brief
         * \param wParam
         * \return
         */
        static KeyboardKey GetMessageKey(WPARAM wParam);

        /**
         * \brief
         * \param msg
         * \return
         */
        static MouseButton GetMessageMouseButton(UINT msg);

        /**
         * \brief
         * \tparam T
         * \param window
         * \return
         */
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<WindowEvent, T>>>
        T CreateWindowEvent(Window* window) const
        {
            T event;
            event.Source = window;

            return event;
        }

        /**
         * \brief
         * \tparam T
         * \param window
         * \param wParam
         * \param lParam
         * \return
         */
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<MouseEvent, T> && std::is_base_of_v<WindowEvent, T>>>
        T CreateWindowMouseEvent(Window* window, WPARAM wParam, LPARAM lParam) const
        {
            T event = CreateWindowEvent<T>(window);

            if (wParam & MK_CONTROL)
                event.Flags |= MouseEventFlagBits::ControlKey;

            if (wParam & MK_LBUTTON)
                event.Flags |= MouseEventFlagBits::LeftButton;

            if (wParam & MK_MBUTTON)
                event.Flags |= MouseEventFlagBits::MiddleButton;

            if (wParam & MK_RBUTTON)
                event.Flags |= MouseEventFlagBits::RightButton;

            if (wParam & MK_SHIFT)
                event.Flags |= MouseEventFlagBits::ShiftKey;

            if (wParam & MK_XBUTTON1)
                event.Flags |= MouseEventFlagBits::ExtraButton1;

            if (wParam & MK_XBUTTON2)
                event.Flags |= MouseEventFlagBits::ExtraButton2;

            event.Position.Width = GET_X_LPARAM(lParam);
            event.Position.Height = GET_Y_LPARAM(lParam);

            return event;
        }

        /// Non-owning window receiving translated native events.
        Window* mWindow;
        bool mResizing;
        WildChar mPendingHighSurrogate;
        Extent2D<int> mMousePosition;

        /// Non-owning monitor cached to detect a window-monitor transition.
        Monitor* mCurrentMonitor;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
