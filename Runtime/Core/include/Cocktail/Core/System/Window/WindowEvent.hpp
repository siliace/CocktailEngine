#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP

#include <Cocktail/Core/System/Keyboard/KeyboardEvent.hpp>
#include <Cocktail/Core/System/Mouse/MouseEvent.hpp>
#include <Cocktail/Core/System/Window/WindowDisplayMode.hpp>

namespace Ck
{
    class Window;

    /**
     * \brief Base struct for Window events
     */
    struct WindowEvent
    {
        /**
         * \brief The Window involved by the event
         */
        Window* Window = nullptr;
    };

    /**
     * \brief Event fired when a Window is about to be closed
     * This event can be fired when the user hit on the cross button of the titlebar
     */
    struct WindowCloseEvent : WindowEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when a Window is about to be redrawn
     */
    struct WindowRedrawEvent: WindowEvent
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
         */
        Extent2D<int> Position;
    };
    
    /**
     * \brief
     */
    struct WindowResizedEvent : WindowEvent
    {
        /**
         * \brief
         */
        Extent2D<unsigned int> Size;

        /**
         * \brief
         */
        WindowDisplayMode DisplayMode = WindowDisplayMode::Normal;
    };

    /**
     * \brief 
     */
    struct WindowTextEvent : WindowEvent
    {
	    /**
         * \brief UTF-32 Unicode value of the character
         */
        Uint32 Unicode = 0;
    };

    /**
     * \brief Event fired when the user press or release a key of the keyboard from a Window
     */
    struct WindowKeyboardEvent : WindowEvent, KeyboardEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user move the cursor of the mouse from a Window
     */
    struct WindowMouseMoveEvent : WindowEvent, MouseMovedEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user press or release a button of the mouse from a Window
     */
    struct WindowMouseButtonEvent : WindowEvent, MouseButtonEvent
    {
        /// Reserved
    };

    /**
     * \brief Event fired when the user roll a wheel of the mouse from a Window
     */
    struct WindowMouseWheelEvent : WindowEvent, MouseWheelEvent
    {
        /// Reserved
    };
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_WINDOWEVENT_HPP