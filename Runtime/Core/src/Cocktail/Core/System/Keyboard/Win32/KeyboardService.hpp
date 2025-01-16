#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP

#include <unordered_map>

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class KeyboardService : public Ck::KeyboardService
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit KeyboardService(Application* application);

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

		Signal<KeyboardEvent> mOnKeyboardEvent;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyPressedEvents;
		std::unordered_map<KeyboardKey, Signal<KeyboardEvent>> mOnKeyReleasedEvents;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP
