#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck::Detail::Xlib
{
    AtomManager::AtomManager(::Display *display) :
        mDisplay(display)
    {
        /// Nothing
    }

    Atom AtomManager::GetAtom(const String& name, bool mustExists)
    {
        if (auto it = mAtoms.find(name); it != mAtoms.end())
            return it->second;

        XLockDisplay(mDisplay);
        Atom atom = XInternAtom(mDisplay, CK_TEXT_TO_ANSI(name.GetData()), mustExists ? True : False);
        XUnlockDisplay(mDisplay);

        mAtoms[name] = atom;

        return atom;
    }
}
