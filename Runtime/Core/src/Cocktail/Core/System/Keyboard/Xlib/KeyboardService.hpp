#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Xlib Platform-specific implementation of the KeyboardService
     */
	class KeyboardService : public Ck::KeyboardService
	{
	public:

		static constexpr KeyCode InvalidKeyCode = 0;

	    /**
         * \brief Tell whether a KeyCode is valid
         *
         * Checks if the provided \p keyCode corresponds to a valid keyboard key
         * in the underlying system.
         *
         * \param keyCode The KeyCode to test
         *
         * \return true if the \p keyCode is valid, false otherwise
         */
		static bool IsValidKeycode(KeyCode keyCode);

	    /**
         * \brief Constructor
         *
         * \param display The system display pointer
         * \param application The application instance
         */
		KeyboardService(::Display* display, Application* application);

	    /**
         * \brief Tell whether a key of the keyboard is pressed
         *
         * Queries the current state of the keyboard to determine if the specified key
         * is held down.
         *
         * \param key The key to test
         *
         * \return True if the \p key is pressed, false otherwise
         */
		bool IsKeyPressed(KeyboardKey key) const override;

	    /**
         * \brief Get the Signal firing KeyboardEvent when such kind of event occur
         *
         * This Signal is triggered for all keyboard events, including key presses,
         * key releases, and possibly other low-level events.
         *
         * \return The Signal
         */
		Signal<KeyboardEvent>& OnKeyboardEvent() override;

	    /**
         * \brief Get the Signal firing KeyboardEvent when a specific key is pressed or released
         *
         * The returned Signal is triggered each time the specified key state changes.
         *
         * \param key The key to listen for
         *
         * \return A reference to the Signal associated with this key event
         */
        Signal<KeyboardEvent>& OnKey(KeyboardKey key) override;

        /**
         * \brief Get the Signal firing KeyboardEvent when a given key is pressed
         *
         * The returned Signal is triggered each time the specified key transitions
         * from an unpressed to a pressed state.
         *
         * \param key The keyboard key
         *
         * \return The Signal
         */
		Signal<KeyboardEvent>& OnKeyPressed(KeyboardKey key) override;

	    /**
         * \brief Get the Signal firing KeyboardEvent when a given key is released
         *
         * The returned Signal is triggered whenever the specified key changes
         * from pressed to released.
         *
         * \param key The keyboard key
         * \return The Signal
         */
		Signal<KeyboardEvent>& OnKeyReleased(KeyboardKey key) override;

	private:

	    /**
         * \brief Utility function to convert a KeyboardKey to its equivalent KeyCode
         *
         * \param key The key to convert
         *
         * \return The KeyCode matching the \p key
         */
	    KeyCode KeyboardKeyToKeyCode(KeyboardKey key) const;

	    /**
         * \brief Utility function to convert a KeyboardKey to its equivalent KeySym
         *
         * \param key The key to convert
         *
         * \return The KeySym matching the \p key
         */
	    KeySym KeyboardKeyToKeySym(KeyboardKey key) const;

		::Display* mDisplay;
		Signal<KeyboardEvent> mOnKeyboardEvent;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyEvents;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyPressedEvents;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyReleasedEvents;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP
