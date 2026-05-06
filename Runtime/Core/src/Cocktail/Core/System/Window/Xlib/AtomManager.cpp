#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>

namespace Ck::Detail::Xlib
{
    AtomManager::AtomManager(::Display* display) :
        mDisplay(display)
    {
        /// Nothing
    }

    Atom AtomManager::GetAtom(const String& name, bool mustExists)
    {
        return mAtoms.ComputeIfMissing(name, [this, mustExists](const String& name) {
            XLockDisplay(mDisplay);
            Atom atom = XInternAtom(mDisplay, reinterpret_cast<const AnsiChar*>(name.GetData()), mustExists ? True : False);
            XUnlockDisplay(mDisplay);

            return atom;
        });
    }
}
