#ifndef COCKTAIL_CORE_IMAGE_GAMMASPACE_HPP
#define COCKTAIL_CORE_IMAGE_GAMMASPACE_HPP

namespace Ck
{
    /**
     * \brief Specifies the gamma space used to interpret color values
     *
     * GammaSpace defines how color components stored in memory should be
     * interpreted and converted when performing color operations.
     */
    enum class GammaSpace
    {
        Linear, /*!< Linear color space (no gamma correction applied) */
        sRGB, /*!< Standard RGB gamma-corrected color space */
    };
}

#endif // COCKTAIL_CORE_IMAGE_GAMMASPACE_HPP
