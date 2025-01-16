#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardEvent.hpp>

namespace Ck
{
    /**
     * \brief Service interacting with the system's keyboard
     */
    class KeyboardService
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~KeyboardService() = default;
        
        /**
         * \brief Tell whether a key of the keyboard is pressed
         * \param key The key to test
         * \return True if the \p key is pressed, false otherwise
         */
        virtual bool IsKeyPressed(KeyboardKey key) const = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when such kind of event occur
         * \return The Signal
         */
        virtual Signal<KeyboardEvent>& OnKeyboardEvent() = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when a given key is pressed
         * \param key The keyboard key
         * \return The Signal
         */
        virtual Signal<KeyboardEvent>& OnKeyPressed(KeyboardKey key) = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when a given key is released
         * \param key The keyboard key
         * \return The Signal
         */
        virtual Signal<KeyboardEvent>& OnKeyReleased(KeyboardKey key) = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP