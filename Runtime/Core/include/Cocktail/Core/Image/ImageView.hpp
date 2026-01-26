#ifndef COCKTAIL_CORE_IMAGE_IMAGEVIEW_HPP
#define COCKTAIL_CORE_IMAGE_IMAGEVIEW_HPP

#include <Cocktail/Core/Image/ImageInfo.hpp>
#include <Cocktail/Core/Utility/ByteArrayView.hpp>

namespace Ck
{
    /**
     * \brief Non-owning view over image pixel data
     *
     * ImageView provides a lightweight, non-owning representation of an image.
     * It stores image metadata (size, raw format and gamma space) and a view
     * over an existing pixel buffer, without taking ownership of the memory.
     *
     * This class is typically used to reference image data owned elsewhere,
     * such as an \ref Image instance, a mapped GPU buffer, or external memory.
     */
    class COCKTAIL_CORE_API ImageView
    {
    public:

        /**
         * \brief Constructs an ImageView from an \ref ImageInfo and a pixel view
         *
         * Initializes the image view using the provided metadata and a view
         * over raw pixel data. The pixel memory is not copied and must remain
         * valid for the lifetime of the ImageView.
         *
         * \param imageInfo Metadata describing size, format and gamma space
         * \param pixels View over the raw pixel data
         */
        ImageView(const ImageInfo& imageInfo, LargeByteArrayView pixels);

        /**
         * \brief Constructs an image view over an existing pixel buffer
         *
         * Initializes the image view using the given image metadata and a view
         * over raw pixel data. The pixel memory is not copied and must remain
         * valid for the lifetime of the ImageView.
         *
         * \param size Image dimensions in pixels
         * \param format Raw pixel format describing channel layout and storage
         * \param gammaSpace Gamma space used to interpret pixel values
         * \param pixels View over the raw pixel data
         */
        ImageView(const Extent2D<unsigned int>& size, ImageRawFormat::Type format, GammaSpace gammaSpace, LargeByteArrayView pixels);

        /**
         * \brief Retrieves the pointer to the memory area of a pixel withing the image view
         *
         * Fetches the pointer to the pixel located at coordinates (\p x, \p y)
         *
         * \param x Horizontal pixel coordinate
         * \param y Vertical pixel coordinate
         *
         * \return Pixel pointer
         */
        const Byte* GetPixelPointer(unsigned int x, unsigned int y) const;

        /**
         * \brief Retrieves the color of a pixel in linear color space
         *
         * Reads the pixel located at coordinates (\p x, \p y), interprets it using
         * the image raw format and gamma space, and converts it to a \ref LinearColor.
         *
         * \param x Horizontal pixel coordinate
         * \param y Vertical pixel coordinate
         *
         * \return Pixel color in linear color space
         */
        LinearColor GetPixelColor(unsigned int x, unsigned int y) const;

        /**
         * \brief Returns the dimensions of the image view
         *
         * \return Image width and height in pixels
         */
        Extent2D<unsigned int> GetSize() const;

        /**
         * \brief Returns the raw pixel format of the image view
         *
         * \return Raw image format describing channel layout and bit depth
         */
        ImageRawFormat::Type GetRawFormat() const;

        /**
         * \brief Returns the gamma space of the image view
         *
         * \return Gamma space used to interpret pixel values
         */
        GammaSpace GetGammaSpace() const;

        /**
         * \brief Returns the image metadata
         *
         * \return Reference to the \ref ImageInfo describing the image
         */
        const ImageInfo& GetImageInfo() const;

        /**
         * \brief Returns a non-owning view of the pixel data
         *
         * The returned view references the pixel data but does not transfer ownership.
         *
         * \return A \ref LargeByteArrayView over the raw pixel buffer
         */
        LargeByteArrayView GetPixels() const;

    private:

        ImageInfo mImageInfo; /*!< Image metadata: size, format, gamma */
        LargeByteArrayView mPixels; /*!< Non-owning view over the pixel buffer */
    };
}

#endif // COCKTAIL_CORE_IMAGE_IMAGEVIEW_HPP
