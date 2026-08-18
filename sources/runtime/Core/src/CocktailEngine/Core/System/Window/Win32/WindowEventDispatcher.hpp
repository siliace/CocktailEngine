#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP

#include <type_traits>

#include <CocktailEngine/Core/System/Window/Win32/Window.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Translates the native messages of a Win32 window into the signals of that window
     *
     * Win32 delivers messages to a window procedure, which is registered per window class
     * and therefore shared by every window of the process. One dispatcher is created per
     * window to hold the state that translation needs, and the window procedure finds it
     * back through the user data of the native window.
     *
     * The dispatcher is created when the window is, and destroyed when the native window
     * is finally gone, so it is tied to the native handle rather than to the Window
     * object. It keeps a non-owning pointer to that object.
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
         * \brief Tells which kind of click a mouse button message reports
         *
         * Win32 encodes the direction of the click and the double click in the message
         * identifier itself rather than in its parameters.
         *
         * \param msg Native Windows message identifier, which has to be a mouse button message
         *
         * \return The click type the message reports
         */
        static MouseButtonEvent::ClickType GetMessageClickType(UINT msg);

        /**
         * \brief Maps a Windows virtual key code to the matching engine key
         *
         * \param wParam The virtual key code carried by the message
         *
         * \return The matching keyboard key
         */
        static KeyboardKey GetMessageKey(WPARAM wParam);

        /**
         * \brief Tells which mouse button a mouse button message reports
         *
         * Only resolves the three standard buttons: the extra ones share a single message
         * and are told apart by its parameters instead.
         *
         * \param msg Native Windows message identifier, which has to be a mouse button message
         *
         * \return The button the message reports
         */
        static MouseButton GetMessageMouseButton(UINT msg);

        /**
         * \brief Builds a window event already attributed to a window
         *
         * \tparam T The window event type to build
         *
         * \param window The window to attribute the event to
         *
         * \return The event, with only its source filled in
         */
        template <typename T, typename = std::enable_if_t<std::is_base_of_v<WindowEvent, T>>>
        T CreateWindowEvent(Window* window) const
        {
            T event;
            event.Source = window;

            return event;
        }

        /**
         * \brief Builds a mouse window event, decoding the parts every mouse message carries
         *
         * Fills in the source, the state of the buttons and modifier keys, and the cursor
         * position, leaving only the fields specific to the message to the caller.
         *
         * \tparam T The mouse window event type to build
         *
         * \param window The window to attribute the event to
         * \param wParam The button and modifier key state carried by the message
         * \param lParam The cursor position carried by the message
         *
         * \return The event, with everything but its message-specific fields filled in
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

        /// Whether the move the user started is a resize, told apart by where they grabbed the frame.
        bool mResizing;

        /// High surrogate awaiting its pair, Win32 reporting a codepoint as two UTF-16 messages.
        WildChar mPendingHighSurrogate;

        /// Last known cursor position, used to derive the relative displacement of a move.
        Extent2D<int> mMousePosition;

        /// Non-owning monitor cached to detect a window-monitor transition.
        Monitor* mCurrentMonitor;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWEVENTDISPATCHER_HPP
