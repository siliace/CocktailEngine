#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>

namespace Ck::Detail::Win32
{
	/**
	 * \brief 
	 */
	class KeyboardService : public Inherit<KeyboardService, Object, Ck::KeyboardService>
	{
	public:

		/**
		 * \brief 
		 * \param key 
		 * \return 
		 */
		bool IsKeyPressed(KeyboardKey key) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Signal<KeyboardEvent>& OnKeyboardEvent() override;

	private:

		Signal<KeyboardEvent> mOnKeyboardEvent;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_WIN32_KEYBOARDSERVICE_HPP
