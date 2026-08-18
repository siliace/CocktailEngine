#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP

#include <CocktailEngine/Core/Cocktail.hpp>

namespace Ck
{
    /**
     * \brief Enumeration of the decorations and interactions a Window can offer
     *
     * Combined as a WindowStyleFlags and passed to WindowFactory::CreateWindow, these
     * bits describe what the window manager puts around the client area and what the user
     * is allowed to do with it. They are fixed at creation: there is no way to restyle a
     * window afterwards.
     *
     * No flag at all yields a bare client area, with no decoration and no interaction,
     * which is what a splash screen or a borderless presentation window wants.
     *
     * A window manager only ever treats these as a request. Platforms also group them
     * differently, so the absence of a flag is not a guarantee that the matching
     * interaction is gone.
     *
     * \see WindowCreateInfo::Style
     */
    enum class WindowStyle
    {
        /**
         * \brief Lets the user close the window, usually through a cross in the title bar
         *
         * Governs the affordance only. A close request can still reach the window through
         * the window manager, so WindowCloseEvent has to be handled either way.
         */
        Closable = Bit(0),

        /**
         * \brief Gives the window a title bar, displaying the title and allowing the user to drag it
         */
        Titlebar = Bit(1),

        /**
         * \brief Gives the window a border the user can drag to resize it
         *
         * Bounded by Window::SetMinSize and Window::SetMaxSize when they are set.
         */
        Resizable = Bit(2),

        /**
         * \brief Lets the user minimize the window to the taskbar or dock
         *
         * On Win32 the affordance comes with Titlebar, which makes this flag redundant
         * there.
         */
        Minimizable = Bit(3),

        /**
         * \brief Lets the user maximize the window to the work area of its monitor
         *
         * On Win32 the affordance comes with Resizable, which makes this flag redundant
         * there.
         */
        Maximizable = Bit(4),
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWSTYLE_HPP
