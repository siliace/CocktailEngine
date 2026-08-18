#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP

#include <CocktailEngine/Core/System/Keyboard/KeyboardEvent.hpp>
#include <CocktailEngine/Core/System/Mouse/MouseEvent.hpp>
#include <CocktailEngine/Core/System/Window/WindowDisplayMode.hpp>

namespace Ck
{
    class Monitor;
    class Window;

    /**
     * \brief Base struct for Window events
     *
     * Every window event names the window it came from, which is what lets a single
     * callback serve several windows. The input events additionally inherit from their
     * device-level counterpart in Keyboard and Mouse, so they expose the fields of both.
     *
     * Events are only produced from inside Window::PollEvents and are passed to the
     * callbacks by reference, so none of the pointers they carry is meant to be retained
     * after the callback returns.
     */
    struct WindowEvent
    {
        /**
         * \brief The Window involved by the event
         */
        Window* Source = nullptr;
    };

    /**
     * \brief Event fired when a Window is about to be closed
     * This event can be fired when the user hit on the cross button of the titlebar
     *
     * The window is still alive while the callbacks run, but it is already gone when the
     * enclosing Window::PollEvents returns false. Nothing here can cancel the close.
     */
    struct WindowCloseEvent : WindowEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when a Window gain or lose focus
     */
    struct WindowFocusEvent : WindowEvent
    {
        /**
         * \brief Specifies whether the Window acquired (true) or lost (false) the focus
         */
        bool HasFocus = false;
    };

    /**
     * \brief Event fired when the Window has been moved on the screen
     */
    struct WindowMovedEvent : WindowEvent
    {
        /**
         * \brief The new position of the Window on the screen
         *
         * Expressed like Window::GetPosition, so the origin is that of the whole window,
         * decorations included.
         */
        Extent2D<int> Position;
    };

    /**
     * \brief Event fired when the user starts resizing a Window by dragging one of its borders
     *
     * Opens the sequence closed by WindowResizingEndEvent, during which
     * WindowResizingEvent is fired repeatedly. A resize that does not come from the user,
     * such as Window::SetSize or Window::Maximize, produces no such sequence and only
     * reports a WindowResizedEvent.
     */
    struct WindowResizingStartEvent : WindowEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired repeatedly while the user is dragging a border of the Window
     *
     * Reports an intermediate size the window is taking, which makes this the event to
     * follow a resize live. Fired only between a WindowResizingStartEvent and a
     * WindowResizingEndEvent.
     */
    struct WindowResizingEvent : WindowEvent
    {
        /**
         * \brief The size in pixels the client area is about to take
         */
        Extent2D<unsigned int> Size;
    };

    /**
     * \brief Event fired when the user stops resizing a Window
     *
     * Closes the sequence opened by WindowResizingStartEvent. The size the window settled
     * on is reported by the accompanying WindowResizedEvent rather than here.
     */
    struct WindowResizingEndEvent : WindowEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired every time the client area of a Window took a new size
     *
     * Fired whatever the cause of the change, an interactive resize, a minimize, a
     * maximize or a Window::SetSize call, which makes it the one event a render surface
     * has to follow to stay in sync with its window.
     */
    struct WindowResizedEvent : WindowEvent
    {
        /**
         * \brief The new size in pixels of the client area, which is null when the Window got minimized
         */
        Extent2D<unsigned int> Size;

        /**
         * \brief The display mode the Window is in now that it has been resized
         */
        WindowDisplayMode DisplayMode = WindowDisplayMode::Normal;
    };

    /**
     * \brief Event fired when the user typed a character in a Window
     *
     * Carries the text the keyboard layout and the input method produced, as opposed to
     * the physical keys reported by WindowKeyboardEvent. A single keystroke can fire both
     * events, none, or several of this one, so text entry should be read from here and
     * shortcuts from WindowKeyboardEvent.
     */
    struct WindowTextEvent : WindowEvent
    {
        /**
         * \brief UTF-32 Unicode value of the character
         */
        Utf32Char Codepoint = 0;
    };

    /**
     * \brief Event fired when the user press or release a key of the keyboard from a Window
     *
     * Reports physical keys, independently of the keyboard layout. Inherits the fields of
     * KeyboardEvent, which tell which key moved, in which direction, and whether the
     * system auto-repeat produced it.
     */
    struct WindowKeyboardEvent : WindowEvent, KeyboardEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user move the cursor of the mouse from a Window
     *
     * Inherits the fields of MouseMovedEvent, which carry the cursor position and the
     * displacement since the previous move.
     */
    struct WindowMouseMoveEvent : WindowEvent, MouseMovedEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user press or release a button of the mouse from a Window
     *
     * Inherits the fields of MouseButtonEvent, which tell which button moved and whether
     * the system recognized a double click.
     */
    struct WindowMouseButtonEvent : WindowEvent, MouseButtonEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user roll a wheel of the mouse from a Window
     *
     * Inherits the fields of MouseWheelEvent, which tell which wheel rolled and by how
     * much, in notches.
     */
    struct WindowMouseWheelEvent : WindowEvent, MouseWheelEvent
    {
        /// Reserved
    };

    /**
     * \brief Event emitted when a window becomes associated with another monitor.
     *
     * Monitor pointers are non-owning views managed by MonitorService. They may be
     * invalidated by a subsequent MonitorTopologyChangedEvent and must not be retained
     * beyond that refresh.
     */
    struct WindowMonitorChangedEvent : public WindowEvent
    {
        /// Monitor associated with the window before the transition, if available.
        Monitor* Previous = nullptr;

        /// Monitor associated with the window after the transition, if available.
        Monitor* Current = nullptr;
    };

}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP
