#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEEVENT_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEEVENT_HPP

#include <Cocktail/Core/Extent2D.hpp>
#include <Cocktail/Core/Flags.hpp>
#include <Cocktail/Core/System/Mouse/MouseButton.hpp>
#include <Cocktail/Core/System/Mouse/MouseWheel.hpp>

namespace Ck
{
    /**
     * \brief Specifies whether various mouse button or keyboard key are pressed when a MouseEvent is fired
     */
    enum class MouseEventFlagBits
    {
        /**
         * \brief The CTRL key is pressed
         * \see KeyboardKey::Control
         */
        ControlKey = Bit(0),

        /**
         * \brief The left mouse button is pressed
         * \see MouseButton::Left
         */
        LeftButton = Bit(1),

        /**
         * \brief The middle mouse button is pressed
         * \see MouseButton::Middle
         */
        MiddleButton = Bit(2),

        /**
         * \brief The right mouse button is pressed
         * \see MouseButton::Right
         */
        RightButton = Bit(3),

        /**
         * \brief The SHIFT key is pressed
         * \see KeyboardKey::Shift
         */
        ShiftKey = Bit(4),

        /**
         * \brief The extra1 mouse button is pressed
         * \see MouseButton::Extra1
         */
        ExtraButton1 = Bit(5),

        /**
         * \brief The extra2 mouse button is pressed
         * \see MouseButton::Extra2
         */
        ExtraButton2 = Bit(6),
    };

    using MouseEventFlags = Flags<MouseEventFlagBits>;

    /**
     * \brief Base struct for mouse events
     */
    struct MouseEvent
    {
        /**
         * \brief Specifies keys and buttons states when the event was fired
         */
        MouseEventFlags Flags;

        /**
         * \brief Specifies the position of the cursor on the screen when the event was fired
         */
        Extent2D<int> Position;
    };

    /**
     * \brief Event fired when the mouse cursor is moved
     */
    struct MouseMovedEvent : MouseEvent
    {
        /**
         * \brief Specifies relative displacement of the cursor since the last moved event
         */
        Extent2D<int> RelativeDisplacement;
    };

    /**
     * \brief Event fired when a button of the mouse is pressed or released
     */
    struct MouseButtonEvent : MouseEvent
    {
        /**
         * \brief Enumeration of possible mouse button event type
         */
        enum class ClickType
        {
            /**
             * \brief The button has been pressed
             */
            Pressed,

            /**
             * \brief The button has been released
             */
            Released,

            /**
             * \brief The button has been clicked (pressed then released) twice
             */
            Double,
        };

        /**
         * \brief Specifies the type of the event
         */
        ClickType Type = ClickType::Pressed;

        /**
         * \brief Specifies the MouseButton involved in the event
         */
        MouseButton Button = MouseButton::Left;
    };

    /**
     * \brief Event fired when a wheel of the mouse is rolled
     */
    struct MouseWheelEvent : MouseEvent
    {
        /** 
         * \brief Specifies the MouseWheel involved in the event
         */
        MouseWheel Wheel = MouseWheel::Vertical;

        /**
         * \brief Specifies distance the wheel is rotated
         * A positive value indicates the wheel has been rotated forward or on the right depending of the wheel.
         * A negative value indicates the wheel has been rotated backward or on the left depending of the wheel.
         */
        float Delta = 0.f;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEEVENT_HPP