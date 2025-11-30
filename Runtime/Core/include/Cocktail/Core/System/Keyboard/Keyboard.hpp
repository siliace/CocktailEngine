#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>

namespace Ck
{
    /**
	 * \brief Facade to KeyboardService
	 *
     * Provides high-level access to keyboard state and events.
     * This class acts as a static interface around KeyboardService,
     * allowing the user to query key states and subscribe to keyboard-related signals.
     */
    class COCKTAIL_CORE_API Keyboard : public ServiceFacade<KeyboardService>
    {
    public:

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
        static bool IsKeyPressed(KeyboardKey key);

        /**
         * \brief Get the global Signal firing a KeyboardEvent whenever any keyboard event occurs
         *
         * This Signal is triggered for all keyboard events, including key presses,
         * key releases, and possibly other keyboard-related interactions depending on the implementation.
         * Users can subscribe to this signal to observe all raw keyboard events.
         *
         * \return A reference to the global keyboard event Signal
         */
        static Signal<KeyboardEvent>& OnKeyboardEvent();

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
        static Signal<KeyboardEvent>& OnKey(KeyboardKey key);

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
        static Signal<KeyboardEvent>& OnKeyPressed(KeyboardKey key);

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
        static Signal<KeyboardEvent>& OnKeyReleased(KeyboardKey key);
    };
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP