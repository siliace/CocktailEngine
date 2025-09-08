#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP

#include <unordered_map>

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
        std::unordered_map<String, Atom> mAtoms;
    };    
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
