#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWCREATEINFO_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWCREATEINFO_HPP

#include <CocktailEngine/Core/Extent2D.hpp>
#include <CocktailEngine/Core/Flags.hpp>
#include <CocktailEngine/Core/String.hpp>
#include <CocktailEngine/Core/System/Window/WindowDisplayMode.hpp>
#include <CocktailEngine/Core/System/Window/WindowStyle.hpp>

namespace Ck
{
    class Window;

    /**
     * \brief Flags of WindowStyle
     *
     * Describes the decorations and the interactions of a window as a combination of
     * WindowStyle bits. Flags<WindowStyle>::All() asks for every one of them, which is
     * what an ordinary application window wants, and Flags<WindowStyle>::None() asks for
     * a bare client area.
     */
    using WindowStyleFlags = Flags<WindowStyle>;

    /**
     * \brief Window descriptor structure
     * Describe the set of parameters to create a Window
     *
     * Only the fields that are set are requested, and a default-constructed descriptor
     * asks for a visible, undecorated, zero-sized window at the origin, which is rarely
     * what a caller wants. Size, Title and Style are the three fields worth setting
     * systematically.
     *
     * The descriptor is read during WindowFactory::CreateWindow and not retained, so it
     * can live on the stack of the calling scope.
     *
     * \see WindowFactory::CreateWindow
     */
    struct WindowCreateInfo
    {
        /**
         * \brief Specifies the initial size in pixels of the window's client area
         *
         * Excludes the decorations, so this is the size available for drawing rather than
         * the footprint of the window on the screen.
         */
        Extent2D<unsigned int> Size;

        /**
         * brief Specifies the initial title to display in the window's title bar
         *
         * Also names the window for the task switcher and the window manager, which makes
         * it worth setting even without WindowStyle::Titlebar.
         */
        String Title;

        /**
         * \brief Specifies the initial position on the screen of the Window to create
         *
         * Expressed in the virtual screen space shared by every monitor, Width carrying
         * the abscissa and Height the ordinate, as in Window::SetPosition.
         */
        Extent2D<int> Position;

        /**
         * \brief Style of the window
         *
         * Fixed for the whole lifetime of the window: there is no way to restyle it
         * afterwards.
         */
        WindowStyleFlags Style;

        /**
         * \brief Specifies the parent Window of the Window to create
         *
         * Non-owning pointer, which has to outlive the window being created. How the
         * relationship is realized is platform-specific, so the parented window is best
         * limited to a tool or dialog window following its owner.
         */
        Window* Parent = nullptr;

        /**
         * \brief Specifies the initial WindowDisplayMode of the Window to create
         *
         * Not honored by the current backends, which always create a window in
         * WindowDisplayMode::Normal. Call Window::Minimize or Window::Maximize after
         * creation to reach another state.
         */
        WindowDisplayMode DisplayMode = WindowDisplayMode::Normal;

        /**
         * \brief Specifies the initial visibility of the Window to create
         *
         * Creating a hidden window is the way to get a chance to finish setting it up, an
         * icon or a size constraint for instance, before it ever shows on the screen.
         */
        bool Visible = true;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWCREATEINFO_HPP
