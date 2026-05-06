#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP

#include <Cocktail/Core/HashMap.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
    class AtomManager
    {
    public:

        explicit AtomManager(::Display* display);

        Atom GetAtom(const String& name, bool mustExists = true);

    private:

        ::Display* mDisplay;
        HashMap<String, Atom> mAtoms;
    };    
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
