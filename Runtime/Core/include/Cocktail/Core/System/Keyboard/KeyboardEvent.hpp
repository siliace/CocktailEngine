#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDEVENT_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDEVENT_HPP

#include <Cocktail/Core/System/Keyboard/KeyboardKey.hpp>

namespace Ck
{
    /**
     * \brief Describe a keyboard event
     * \see Keyboard::OnKeyboardEvent
     * \see KeyboardService::OnKeyboardEvent
     */
    struct KeyboardEvent
    {
        /**
         * \brief Specifies the key involved by the event
         */
        KeyboardKey Key = KeyboardKey::Backspace;

        /**
         * \brief Specifies whether the \p Key has been pressed or released
         */
        bool Pressed = false;
        
        /** 
         * \brief Specifies whether the event has been fired because a key is pressed since enough time to trigger system key repeat
         */
        bool Repeated = false;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARDEVENT_HPP