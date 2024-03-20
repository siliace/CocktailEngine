#include <Cocktail/Core/System/Keyboard/Keyboard.hpp>

namespace Ck
{
    bool Keyboard::IsKeyPressed(KeyboardKey key)
    {
        return ResolveFacadeInstance()->IsKeyPressed(key);
    }

    Signal<KeyboardEvent>& Keyboard::OnKeyboardEvent()
    {
        return ResolveFacadeInstance()->OnKeyboardEvent();
    }
}