#ifndef COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEWHEEL_HPP
#define COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEWHEEL_HPP

namespace Ck
{
    /**
     * \brief Enumeration of supported wheels of the mouse
     */
    enum class MouseWheel
    {
        /**
         * \brief The vertical wheel
         * Likely the one between the left and the right button
         */
        Vertical,

        /**
         * \brief
         * Likely the one under the thumb or the vertical one pushed horizontally
         */
        Horizontal,
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MOUSE_MOUSEWHEEL_HPP