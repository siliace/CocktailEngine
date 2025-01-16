#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
	/**
	 * \brief 
	 */
	class KeyboardService : public Ck::KeyboardService
	{
	public:

		static constexpr KeyCode InvalidKeyCode = 0;

		/**
		 * \brief Tell whether a KeyCode is valid
		 * \param keyCode The KeyCode to test
		 * \return true if the \p keyCode is valid, false otherwise
		 */
		static bool IsValidKeycode(KeyCode keyCode);

		/**
		 * \brief 
		 * \param display
		 * \param application 
		 */
		KeyboardService(::Display* display, Application* application);

		/**
		 * \brief Utility function to convert a KeyboardKey to its equivalent KeyCode
		 * \param key The key to convert
		 * \return The KeyCode matching the \p key
		 */
		KeyCode KeyboardKeyToKeyCode(KeyboardKey key) const;

		/**
		 * \brief Utility function to convert a KeyboardKey to its equivalent KeySym
		 * \param key The key to convert
		 * \return The KeySym matching the \p key
		 */
		KeySym KeyboardKeyToKeySym(KeyboardKey key) const;

		/**
		 * \brief Tell whether a key of the keyboard is pressed
		 * \param key The key to test
		 * \return True if the \p key is pressed, false otherwise
		 */
		bool IsKeyPressed(KeyboardKey key) const override;

		/**
		 * \brief Get the Signal firing KeyboardEvent when such kind of event occur
		 * \return The Signal
		 */
		Signal<KeyboardEvent>& OnKeyboardEvent() override;

		/**
		 * \brief Get the Signal firing KeyboardEvent when a given key is pressed
		 * \param key The keyboard key
		 * \return The Signal
		 */
		Signal<KeyboardEvent>& OnKeyPressed(KeyboardKey key) override;

		/**
		 * \brief Get the Signal firing KeyboardEvent when a given key is released
		 * \param key The keyboard key
		 * \return The Signal
		 */
		Signal<KeyboardEvent>& OnKeyReleased(KeyboardKey key) override;

	private:

		::Display* mDisplay;
		Signal<KeyboardEvent> mOnKeyboardEvent;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyPressedEvents;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyReleasedEvents;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_XLIB_KEYBOARDSERVICE_HPP
