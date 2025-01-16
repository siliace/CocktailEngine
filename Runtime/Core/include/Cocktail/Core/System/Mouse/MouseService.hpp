#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_MOUSESERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_MOUSESERVICE_HPP

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/Mouse/MouseEvent.hpp>

namespace Ck
{
    /**
     * \brief Service interacting with the system's mouse
     */
    class MouseService
    {
    public:

		/**
		 * \brief Destructor
		 */
		virtual ~MouseService() = default;

        /**
         * \brief Tell whether the mouse cursor is visible
         * \return True if the cursor is visible, false otherwise
         */
        virtual bool IsCursorVisible() const = 0;

        /**
         * \brief Show or hide the mouse cursor
         * \param visible True to show the cursor, false to hide it
         */
        virtual void SetCursorVisible(bool visible = true) = 0;

        /**
         * \brief Get the position of the mouse cursor on the screen
         * \return The position of the cursor
         */
        virtual Extent2D<int> GetCursorPosition() const = 0;

        /**
         * \brief
         * \param position
         */
        virtual void SetCursorPosition(const Extent2D<int>& position) = 0;

        /**
         * \brief Tell whether a button of the mouse is pressed
         * \param button The button to test
         * \return True if the \p button is pressed, false otherwise
         */
        virtual bool IsPressedButton(MouseButton button) const = 0;

        /**
         * \brief Get the Signal firing MouseMovedEvent when such kind of event occur
         * \return The Signal
         */
        virtual Signal<MouseMovedEvent>& OnMouseMovedEvent() = 0;

        /**
         * \brief Get the Signal firing MouseButtonEvent when such kind of event occur
         * \return The Signal
         */
        virtual Signal<MouseButtonEvent>& OnMouseButtonEvent() = 0;

        /**
         * \brief 
         * \param button 
         * \return 
         */
        virtual Signal<MouseButtonEvent>& OnMouseButtonPressedEvent(MouseButton button) = 0;

        /**
         * \brief
         * \param button
         * \return
         */
        virtual Signal<MouseButtonEvent>& OnMouseButtonReleasedEvent(MouseButton button) = 0;

        /**
         * \brief Get the Signal firing MouseWheelEvent when such kind of event occur
         * \return The Signal
         */
        virtual Signal<MouseWheelEvent>& OnMouseWheelEvent() = 0;

        /**
         * \brief 
         * \param wheel 
         * \return 
         */
        virtual Signal<MouseWheelEvent>& OnMouseWheelEvent(MouseWheel wheel) = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_MOUSESERVICE_HPP