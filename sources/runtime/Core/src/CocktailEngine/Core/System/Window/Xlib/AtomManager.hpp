#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP

#include <CocktailEngine/Core/HashMap.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Cache of the X11 atoms the windowing code needs, registered as a singleton by the Xlib service provider
     *
     * X11 names everything, properties, protocols and message types alike, with an atom
     * the server hands out in exchange for a string. Resolving one is a round trip to the
     * server, and the same handful of names is needed for every window, which is what this
     * cache is for.
     */
    class AtomManager
    {
    public:

        /**
         * \brief Binds the cache to a display connection
         *
         * \param display The shared display connection, which has to outlive this manager
         */
        explicit AtomManager(::Display* display);

        /**
         * \brief Retrieves the atom a name maps to, resolving it on first use
         *
         * Whether an atom has to already exist is what tells a name the code depends on
         * from one it can do without: asking for an optional atom yields zero when the
         * server knows nothing about it, which is how support for a protocol the window
         * manager may not implement gets detected.
         *
         * \param name The name to resolve, which is case-sensitive
         * \param mustExists True to have the server create the atom when it does not exist yet, false to only look it up
         *
         * \return The atom the name maps to, or zero if it does not exist and was not to be created
         */
        Atom GetAtom(const String& name, bool mustExists = true);

    private:

        /// Connection the atoms are resolved on, owned by the service provider.
        ::Display* mDisplay;

        /// Atoms resolved so far, keyed by name.
        HashMap<String, Atom> mAtoms;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_ATOMMANAGER_HPP
