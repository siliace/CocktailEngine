#ifndef COCKTAIL_CORE_IMAGE_IMAGERAWFORMAT_HPP
#define COCKTAIL_CORE_IMAGE_IMAGERAWFORMAT_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/DataType.hpp>
#include <Cocktail/Core/Extent2D.hpp>
#include <Cocktail/Core/Image/GammaSpace.hpp>

namespace Ck
{
    /**
     * \brief Describes raw, uncompressed image pixel formats
     *
     * This utility class defines a set of raw image formats along with helper
     * functions to query their memory layout characteristics, such as channel
     * count, channel stride and total allocation size.
     *
     * The formats described here are tightly packed and do not include any
     * padding or compression.
     */
    class COCKTAIL_CORE_API ImageRawFormat
    {
    public:

        /**
         * \brief Enumeration of supported raw image formats
         *
         * Each format specifies:
         * - the number of color channels
         * - the order of the channels
         * - the bit depth and numeric representation of each channel
         *
         * Supported channel layouts:
         * - Grey           : single luminance channel
         * - GreyAlpha      : luminance + alpha channel
         * - RGB            : red, green and blue channels
         * - RGBA           : red, green, blue and alpha channels
         *
         * Naming convention:
         * - 8 / 16       : unsigned normalized integer
         * - 32F          : floating-point representation
         * - Channel order is explicitly stated (e.g. RGB, RGBA)
         */
        enum class Type
        {
            Grey8, /*!< Single-channel 8-bit unsigned normalized greyscale format */
            Grey16, /*!< Single-channel 16-bit unsigned normalized greyscale format */
            Grey32F, /*!< Single-channel 32-bit floating-point greyscale format */

            GreyAlpha8, /*!< Two-channel 8-bit unsigned normalized greyscale + alpha format */
            GreyAlpha16, /*!< Two-channel 16-bit unsigned normalized greyscale + alpha format */
            GreyAlpha32F, /*!< Two-channel 32-bit floating-point greyscale + alpha format */

            RedGreenBlue8, /*!< Three-channel 8-bit unsigned normalized RGB format */
            RedGreenBlue16, /*!< Three-channel 16-bit unsigned normalized RGB format */
            RedGreenBlue32F, /*!< Three-channel 32-bit floating-point RGB format */

            RedGreenBlueAlpha8, /*!< Four-channel 8-bit unsigned normalized RGBA format */
            RedGreenBlueAlpha16, /*!< Four-channel 16-bit unsigned normalized RGBA format */
            RedGreenBlueAlpha32F, /*!< Four-channel 32-bit floating-point RGBA format */
        };

        /**
         * \brief Returns the raw image format matching a channel count and data type
         *
         * Selects the appropriate \ref ImageRawFormat::Type based on the number of
         * color channels and the underlying data representation of each channel.
         *
         * Supported mappings:
         * - UnsignedInt8  : Grey8, GreyAlpha8, RGB8, RGBA8
         * - UnsignedInt16 : Grey16, GreyAlpha16, RGB16, RGBA16
         * - Float32       : Grey32F, GreyAlpha32F, RGB32F, RGBA32F
         *
         * Only channel counts of 1 (Grey), 2 (GreyAlpha), 3 (RGB) and 4 (RGBA) are supported.
         *
         * \param channelCount Number of color channels (1, 2, 3 or 4)
         * \param type Data type used to represent each channel
         *
         * \return The corresponding raw image format
         *
         * \warning If the combination of \p channelCount and \p type is not supported,
         *          the function triggers an assertion failure.
         */
        static Type Of(unsigned int channelCount, DataType type);

        /**
         * \brief Converts a raw pixel to a linear color
         *
         * Reads a pixel from memory, interprets it according to the specified \p format,
         * and converts its values to a \ref LinearColor in linear space.
         *
         * The function automatically applies gamma correction if the pixel is stored
         * in a non-linear color space (e.g., sRGB), based on \p gammaSpace.
         *
         * \param pixel Pointer to the raw pixel data in memory
         * \param format The raw image format of the pixel
         * \param gammaSpace The color space of the stored pixel values (linear or gamma-corrected)
         *
         * \return The pixel color converted to linear space as a \ref LinearColor
         *
         * \note The function assumes that \p pixel points to a valid memory block
         *       large enough to hold a full pixel in the specified \p format.
         */
        static LinearColor GetPixelColor(const Byte* pixel, Type format, GammaSpace gammaSpace);

        /**
         * \brief Returns the number of color channels for a given format
         *
         * This function indicates how many distinct components are stored per
         * pixel (e.g. 1 for Grey, 2 for GreyAlpha, 3 for RGB, 4 for RGBA).
         *
         * \param format The image raw format
         *
         * \return Number of channels per pixel
         */
        static unsigned int GetChannelCount(Type format);

        /**
         * \brief Returns the byte size of a single channel for a given format
         *
         * The returned stride applies to a single channel, regardless of the total
         * number of channels in the format.
         *
         * For example:
         * - Red8        -> 1 byte per channel
         * - Red32F      -> 2 bytes per channel
         * - Red32F      -> 4 bytes per channel
         *
         * \param format The image raw format
         *
         * \return Size in bytes of a single channel
         */
        static unsigned int GetChannelStride(Type format);

        /**
         * \brief Returns the size in bytes of a single pixel
         *
         * Computes the total memory size of one pixel for the given format.
         * This is equal to the number of channels multiplied by the stride of
         * each channel.
         *
         * \param format The image raw format
         *
         * \return Size of one pixel in bytes
         */
        static Uint64 GetPixelStride(Type format);

        /**
         * \brief Computes the memory size required to store a 2D image
         *
         * Calculates the total number of bytes required to store an image of the
         * given dimensions and format, assuming tightly packed pixels and no
         * additional padding.
         *
         * \tparam T Numeric type used to represent the extent dimensions
         *
         * \param size 2D image dimensions in pixels
         * \param format Image raw format
         *
         * \return Required allocation size in bytes
         */
        template <typename T>
        static Uint64 ComputeAllocationSize(Extent2D<T> size, Type format)
        {
            return GetPixelStride(format) * size.Width * size.Height;
        }

        /**
         * \brief Indicates whether the format is greyscale-based
         *
         * A format is considered greyscale if it has one or two channels representing
         * luminance (Grey or GreyAlpha), regardless of whether it has an alpha channel.
         *
         * \param format The image raw format
         *
         * \return \c true if the format is Grey or GreyAlpha, \c false otherwise
         */
        static bool IsGrey(Type format);

        /**
         * \brief Indicates whether the format contains color channels (RGB)
         *
         * A format is considered color if it contains red, green and blue channels.
         * Grey / GreyAlpha formats are not considered color formats.
         *
         * \param format The image raw format
         *
         * \return \c true if the format contains RGB channels, \c false otherwise
         */
        static bool IsColor(Type format);

        /**
         * \brief Indicates whether a format uses high dynamic range (HDR) storage
         *
         * A format is considered HDR when at least one of its channels is stored
         * using a floating-point representation, allowing values outside the
         * normalized [0, 1] range.
         *
         * This function typically identifies formats suitable for physically-based
         * rendering, lighting data or high-precision color storage.
         *
         * \param format The image raw format
         *
         * \return \c true if the format uses floating-point channels (32F suffix),
         *         \c false otherwise
         */
        static bool IsHdr(Type format);

        /**
         * \brief Indicates whether a format contains an alpha channel
         *
         * Determines if the given raw image format includes an alpha component in
         * addition to its color channels.
         *
         * Formats with an alpha channel are typically used for transparency,
         * compositing or masking operations.
         *
         * \param format The image raw format
         *
         * \return \c true if the format includes an alpha channel,
         *         \c false otherwise
         */
        static bool HasAlphaChannel(Type format);
    };
}

#endif // COCKTAIL_CORE_IMAGE_IMAGERAWFORMAT_HPP
