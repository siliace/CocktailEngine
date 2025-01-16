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

    Signal<KeyboardEvent>& Keyboard::OnKeyPressed(KeyboardKey key)
    {
        return ResolveFacadeInstance()->OnKeyPressed(key);
    }

    Signal<KeyboardEvent>& Keyboard::OnKeyReleased(KeyboardKey key)
    {
        return ResolveFacadeInstance()->OnKeyReleased(key);
    }
}
