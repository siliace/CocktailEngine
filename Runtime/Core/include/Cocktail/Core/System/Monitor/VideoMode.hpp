#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP

#include <Cocktail/Core/Extent2D.hpp>

namespace Ck
{
    /**
     * \brief Enumeration of supported display color depths
     */
    enum class DisplayColorDepth
    {
        e24, /*!< Specifies a color depth of 24 bits (RGB) */
        e32 /*!< Specifies a color depth of 32 bits (RGBA or padded RGB) */
    };

    /**
     * \brief Enumeration of supported alpha channel depths
     */
    enum class DisplayAlphaDepth
    {
        e0, /*!< No alpha channel */
        e8 /*!< Alpha channel with 8-bit precision */
    };

    /**
     * \brief Represents a video mode configuration for a display
     *
     * A video mode defines the resolution, refresh rate, and pixel format
     * (color and alpha depth) used by a monitor.
     */
    struct VideoMode
    {
        /**
         * \brief Resolution of the display in pixels
         */
        Extent2D<unsigned int> Size;

        /**
         * \brief Refresh rate of the display in Hertz
         */
        unsigned int Frequency = 0;

        /**
         * \brief Color depth of the display
         */
        DisplayColorDepth ColorDepth = DisplayColorDepth::e32;

        /**
         * \brief Alpha channel depth of the display
         */
        DisplayAlphaDepth AlphaDepth = DisplayAlphaDepth::e8;

        /**
         * \brief Equality comparison operator
         *
         * Compares all video mode parameters for exact match.
         *
         * \param rhs The video mode to compare with
         * \return True if both video modes are identical
         */
        bool operator==(const VideoMode& rhs) const
        {
            return std::tie(Size, Frequency, ColorDepth, AlphaDepth) == std::tie(rhs.Size, rhs.Frequency, rhs.ColorDepth, rhs.AlphaDepth);
        }

        /**
         * \brief Inequality comparison operator
         *
         * \param rhs The video mode to compare with
         * \return True if the video modes differ
         */
        bool operator!=(const VideoMode& rhs) const
        {
            return !(*this == rhs);
        }
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_VIDEOMODE_HPP
