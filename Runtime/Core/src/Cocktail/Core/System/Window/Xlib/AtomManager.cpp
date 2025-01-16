#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>

namespace Ck::Detail::Xlib
{
    AtomManager::AtomManager(::Display *display) : mDisplay(display)
    {
        /// Nothing
    }

    Atom AtomManager::GetAtom(const std::string& name, bool mustExists)
    {
        if (auto it = mAtoms.find(name); it != mAtoms.end())
            return it->second;

        XLockDisplay(mDisplay);
        Atom atom = XInternAtom(mDisplay, name.c_str(), mustExists ? True : False);
        XUnlockDisplay(mDisplay);

        mAtoms[name] = atom;

        return atom;
    }
}