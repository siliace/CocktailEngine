#ifndef COCKTAIL_CORE_IMAGE_IMAGE_HPP
#define COCKTAIL_CORE_IMAGE_IMAGE_HPP

#include <Cocktail/Core/Image/ImageView.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
    /**
     * \brief Parameters controlling image import behavior
     *
     * Defines how an image should be interpreted when loaded from an external
     * source. These parameters influence channel layout and color interpretation
     * during the import process.
     */
    struct ImageImportParameters
    {
        /**
         * \brief Desired channel layout for imported images
         *
         * Specifies how the imported image should be converted or interpreted
         * regardless of its original format.
         */
        enum class ImportFormat
        {
            Default, /*!< Preserve the original image format when possible */
            Grey, /*!< Force conversion to a single-channel greyscale image */
            GreyAlpha, /*!< Force conversion to greyscale with alpha channel */
            RGB, /*!< Force conversion to three-channel RGB format */
            RGBA, /*!< Force conversion to four-channel RGBA format */
        };

        ImportFormat Format = ImportFormat::RGBA; /*!< Requested import format */
    };

    /**
     * \brief Parameters controlling image export behavior
     *
     * Defines how an image should be serialized when written to an external file
     * or stream.
     */
    struct ImageExportParameters
    {
        String Format; /*!< Target file format identifier (e.g. "png", "jpg", "bmp") */
    };

    /**
     * \brief Represents an uncompressed image stored in memory
     *
     * This class owns raw pixel data along with its associated metadata such as
     * dimensions, pixel format and gamma space. Pixels are stored in a tightly
     * packed layout without compression.
     *
     * The image can be implicitly converted to an \ref ImageView to provide a
     * lightweight, non-owning view over its data.
     */
    class COCKTAIL_CORE_API Image
    {
    public:

        /**
         * \brief Constructs an image from an \ref ImageInfo and raw pixels
         *
         * Initializes an image with the provided metadata and pixel data buffer.
         * The pixel data is moved into the image, making this image the owner.
         *
         * \param imageInfo Metadata describing size, format and gamma space
         * \param pixels Raw pixel data buffer to own
         */
        Image(const ImageInfo& imageInfo, LargeByteArray pixels);

        /**
         * \brief Constructs an image from raw pixel data
         *
         * Initializes an image with the given dimensions, raw format and gamma
         * space. The pixel data is copied or moved into internal storage.
         *
         * \param size Image dimensions in pixels
         * \param format Raw pixel format describing channel layout and storage
         * \param gammaSpace Gamma space used to interpret pixel values
         * \param pixels Raw pixel data buffer
         */
        Image(const Extent2D<unsigned int>& size, ImageRawFormat::Type format, GammaSpace gammaSpace, LargeByteArray pixels);

        /**
         * \brief Retrieves the pointer to the memory area of a pixel withing the image
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
         * \brief Converts the image to a linear RGBA array
         *
         * Generates a new array of \ref LinearColor values representing the image
         * in RGBA format. Each pixel is expanded to four channels (Red, Green, Blue,
         * Alpha), converted to **linear color space**, regardless of the original
         * image format.
         *
         * \return A \ref LargeArray containing the image pixels in linear RGBA format.
         *
         * \note The returned array is a full copy of the image data; modifications
         *       to the returned array do not affect the original image.
         */
        LargeArray<LinearColor> AsRGBA() const;

        /**
         * \brief Returns the dimensions of the image
         *
         * \return Image width and height in pixels
         */
        Extent2D<unsigned int> GetSize() const;

        /**
         * \brief Returns the raw pixel format of the image
         *
         * \return Raw image format describing channel layout and bit depth
         */
        ImageRawFormat::Type GetRawFormat() const;

        /**
         * \brief Returns the gamma space of the image
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
         * \brief Returns a reference to the raw pixel data
         *
         * The returned references refers to tightly packed pixel data stored in the
         * image's internal buffer.
         *
         * \return Reference to the raw pixel data
         */
        const LargeByteArray& GetPixels() const;

        /**
         * \brief Converts the image to a lightweight image view
         *
         * Creates an \ref ImageView referencing this image's pixel data without
         * transferring ownership.
         *
         * \return An \ref ImageView referencing this image
         */
        operator ImageView() const;

    private:

        ImageInfo mImageInfo; /*!< Image metadata: size, format, gamma */
        LargeByteArray mPixels; /*!< Raw pixel storage */
    };
}

#endif // COCKTAIL_CORE_IMAGE_IMAGE_HPP
