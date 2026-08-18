#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWDISPLAYMODE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWDISPLAYMODE_HPP

namespace Ck
{
    /**
     * \brief Enumeration of the states a Window can occupy on its monitor
     *
     * Describes how much of its monitor a window takes, which is a state of the window
     * rather than a style: unlike WindowStyle it changes over the lifetime of the window,
     * and every change is reported by a WindowResizedEvent.
     *
     * \see WindowCreateInfo::DisplayMode
     * \see WindowResizedEvent::DisplayMode
     */
    enum class WindowDisplayMode
    {
        /**
         * \brief The window occupies the position and the size it was given
         */
        Normal,

        /**
         * \brief The window is collapsed to the taskbar or dock and has no client area left to draw into
         *
         * A window entering this state reports a null size, which is why a render surface
         * has to tolerate it rather than try to match it.
         */
        Minimized,

        /**
         * \brief The window is expanded to the work area of its monitor, leaving the system bars visible
         */
        Maximized,
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WINDOWDISPLAYMODE_HPP
