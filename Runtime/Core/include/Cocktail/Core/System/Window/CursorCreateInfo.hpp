#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_CURSORCREATEINFO_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_CURSORCREATEINFO_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Extent2D.hpp>

namespace Ck
{
    /**
     * \brief
     */
    struct CursorCreateInfo
    {
        /**
         * \brief
         */
        const Uint32* Pixels = nullptr;
        
        /**
         * \brief
         */
        Extent2D<unsigned int> Size;

        /**
         * \brief
         */
        Extent2D<unsigned int> HotSpot;

        /**
         * \brief
         */
        Uint32 RedMask = 0xff000000;

        /**
         * \brief
         */
        Uint32 GreenMask = 0x00ff0000;

        /**
         * \brief
         */
        Uint32 BlueMask = 0x0000ff00;

        /**
         * \brief
         */
        Uint32 AlphaMask = 0x000000ff;
    };
}   

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_CURSORCREATEINFO_HPP