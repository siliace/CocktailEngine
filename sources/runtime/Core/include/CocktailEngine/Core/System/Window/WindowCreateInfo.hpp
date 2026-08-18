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
     */
    using WindowStyleFlags = Flags<WindowStyle>;

    /**
     * \brief Window descriptor structure
     * Describe the set of parameters to create a Window
     * \see WindowFactory::CreateWindow
     */
    struct WindowCreateInfo
    {
	    /**
         * \brief Specifies the initial size in pixels of the window's client area
         */
        Extent2D<unsigned int> Size;

	    /**
         * brief Specifies the initial title to display in the window's title bar
         */
        String Title;

	    /**
         * \brief Specifies the initial position on the screen of the Window to create
         */
        Extent2D<int> Position;

        /**
         * \brief Style of the window
         */
        WindowStyleFlags Style;

	    /**
         * \brief Specifies the parent Window of the Window to create
         */
        Window* Parent = nullptr;

	    /**
         * \brief Specifies the initial WindowDisplayMode of the Window to create 
         */
        WindowDisplayMode DisplayMode = WindowDisplayMode::Normal;

	    /**
         * \brief Specifies the initial visibility of the Window to create
         */
        bool Visible = true;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWCREATEINFO_HPP
