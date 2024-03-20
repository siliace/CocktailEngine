#ifndef COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP
#define COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Keyboard/KeyboardService.hpp>

namespace Ck
{
    /**
	 * \brief Facade to KeyboardService
     */
    class COCKTAIL_CORE_API Keyboard : public ServiceFacade<KeyboardService>
    {
    public:

        /**
         * \brief Tell whether a key of the keyboard is pressed
         * \param key The key to test
         * \return True if the key is pressed, false otherwise
         */
        static bool IsKeyPressed(KeyboardKey key);

        /**
         * \brief Get the Signal firing KeyboardEvent when such kind of event occur
         * \return The Signal
         */
        static Signal<KeyboardEvent>& OnKeyboardEvent();
    };
}

#endif // COCKTAIL_CORE_SYSTEM_KEYBOARD_KEYBOARD_HPP