#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP

#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardEvent.hpp>

namespace Ck
{
    /**
     * \brief Service interacting with the system's keyboard
     *
     * This abstract service defines the low-level interface used to query
     * the keyboard state and emit keyboard-related events. Concrete platform
     * implementations provide the actual behavior, while higher-level
     * abstractions such as Keyboard delegate their calls to this service.
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
         *
         * Queries the underlying KeyboardService to determine if the given key
         * is currently held down by the user.
         *
         * \param key The key to test
         *
         * \return True if the key is pressed, false otherwise
         */
        virtual bool IsKeyPressed(KeyboardKey key) const = 0;

        /**
         * \brief Get the global Signal firing a KeyboardEvent whenever any keyboard event occurs
         *
         * This Signal is triggered for all keyboard events, including key presses,
         * key releases, and possibly other keyboard-related interactions depending on the implementation.
         * Users can subscribe to this signal to observe all raw keyboard events.
         *
         * \return A reference to the global keyboard event Signal
         */
        virtual Signal<KeyboardEvent>& OnKeyboardEvent() = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when a specific key is pressed or released
         *
         * The returned Signal is triggered each time the specified key state changes.
         * It allows binding callbacks only for the desired key instead of listening to global keyboard events.
         *
         * \param key The key to listen for
         *
         * \return A reference to the Signal associated with this key event
         */
        virtual Signal<KeyboardEvent>& OnKey(KeyboardKey key) = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when a specific key is pressed
         *
         * The returned Signal is triggered each time the specified key transitions
         * from an unpressed to a pressed state. It allows binding callbacks only for
         * the desired key instead of listening to global keyboard events.
         *
         * \param key The key to listen for
         *
         * \return A reference to the Signal associated with this key press event
         */
        virtual Signal<KeyboardEvent>& OnKeyPressed(KeyboardKey key) = 0;

        /**
         * \brief Get the Signal firing KeyboardEvent when a given key is released
         *
         * The returned Signal is triggered whenever the specified key changes
         * from pressed to released. This allows subscribing to key-release events
         * without filtering global keyboard events manually.
         *
         * \param key The keyboard key
         *
         * \return A reference to the Signal associated with this key release event
         */
        virtual Signal<KeyboardEvent>& OnKeyReleased(KeyboardKey key) = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDSERVICE_HPP