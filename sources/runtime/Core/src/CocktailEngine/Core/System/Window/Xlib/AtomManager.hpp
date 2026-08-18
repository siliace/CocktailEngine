#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>

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

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
