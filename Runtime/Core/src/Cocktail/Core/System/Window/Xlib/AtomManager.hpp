#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP

#include <string>
#include <unordered_map>

#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
    class AtomManager
    {
    public:

        explicit AtomManager(::Display* display);

        Atom GetAtom(const std::string& name, bool mustExists = true);

    private:

        ::Display* mDisplay;
        std::unordered_map<std::string, Atom> mAtoms;
    };    
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
