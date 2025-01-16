#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_MOUSE_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_MOUSE_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Mouse/MouseService.hpp>

namespace Ck
{
    /**
     * \brief Facade to MouseService
     */
    class COCKTAIL_CORE_API Mouse : public ServiceFacade<MouseService>
    {
    public:

        /**
         * \brief Tell whether the mouse cursor is visible
         * \return True if the cursor is visible, false otherwise
         */
        static bool IsCursorVisible();

        /**
         * \brief Show or hide the mouse cursor
         * \param visible True to show the cursor, false to hide it
         */
        static void SetCursorVisible(bool visible = true);

        /**
         * \brief Get the position of the mouse cursor on the screen
         * \return The position of the cursor
         */
        static Extent2D<int> GetCursorPosition();

        /**
         * \brief
         * \param position
         */
        static void SetCursorPosition(const Extent2D<int>& position);

        /**
         * \brief Tell whether a button of the mouse is pressed
         * \param button The button to test
         * \return True if the \p button is pressed, false otherwise
         */
        static bool IsPressedButton(MouseButton button);

        /**
         * \brief Get the Signal firing MouseMovedEvent when such kind of event occur
         * \return The Signal
         */
        static Signal<MouseMovedEvent>& OnMouseMovedEvent();

        /**
         * \brief Get the Signal firing MouseButtonEvent when such kind of event occur
         * \return The Signal
         */
        static Signal<MouseButtonEvent>& OnMouseButtonEvent();

        /**
         * \brief Get the Signal firing MouseWheelEvent when such kind of event occur
         * \return The Signal
         */
        static Signal<MouseWheelEvent>& OnMouseWheelEvent();
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_MOUSE_HPP