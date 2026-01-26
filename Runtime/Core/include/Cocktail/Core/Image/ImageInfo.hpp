#ifndef COCKTAIL_CORE_IMAGE_IMAGEINFO_HPP
#define COCKTAIL_CORE_IMAGE_IMAGEINFO_HPP

#include <Cocktail/Core/Color.hpp>
#include <Cocktail/Core/Image/GammaSpace.hpp>
#include <Cocktail/Core/Image/ImageRawFormat.hpp>

namespace Ck
{
    /**
     * \brief Stores metadata describing an image
     *
     * ImageInfo encapsulates the essential properties of an image:
     * its dimensions, raw pixel format, and gamma space. This class does
     * not own pixel data; it only describes how the pixels are stored and
     * interpreted.
     *
     * ImageInfo is used by \ref Image, \ref ImageView, and \ref ImageArray
     * to store consistent image metadata alongside the pixel data.
     */
    class COCKTAIL_CORE_API ImageInfo
    {
    public:

        /**
         * \brief Constructs image metadata
         *
         * Initializes the image with the given dimensions, raw format and gamma
         * space. No pixel data is stored at this level.
         *
         * \param size Image dimensions in pixels
         * \param format Raw pixel format describing channel layout and storage
         * \param gammaSpace Gamma space used to interpret pixel values
         */
        ImageInfo(Extent2D<unsigned int> size, ImageRawFormat::Type format, GammaSpace gammaSpace);

        /**
         * \brief Virtual destructor
         */
        virtual ~ImageInfo() = default;

        /**
         * \brief Computes the byte offset of a pixel in a raw pixel buffer
         *
         * Calculates the memory offset corresponding to the pixel located at
         * coordinates (\p x, \p y), assuming tightly packed pixel data.
         *
         * \param x Horizontal pixel coordinate
         * \param y Vertical pixel coordinate
         *
         * \return Byte offset of the pixel in a raw buffer
         */
        Uint64 GetPixelOffset(unsigned int x, unsigned int y) const;

        /**
         * \brief Computes the number of pixels in the image
         *
         * \return Number of pixel
         */
        Uint64 GetPixelCount() const;

        /**
         * \brief Returns the image dimensions
         *
         * \return Image size in pixels
         */
        const Extent2D<unsigned int>& GetSize() const;

        /**
         * \brief Returns the raw pixel format
         *
         * \return Image raw format
         */
        ImageRawFormat::Type GetRawFormat() const;

        /**
         * \brief Returns the gamma space of the image
         *
         * \return Gamma space used to interpret pixel values
         */
        GammaSpace GetGammaSpace() const;

        /**
         * \brief Compares this ImageInfo instance with another for equality
         *
         * Two ImageInfo objects are considered equal if their dimensions,
         * raw format, and gamma space all match.
         *
         * \param rhs The other ImageInfo to compare against
         *
         * \return \c true if all fields are identical, \c false otherwise
         */
        bool operator==(const ImageInfo& rhs) const
        {
            return std::tie(mSize, mFormat, mGammaSpace) == std::tie(rhs.mSize, rhs.mFormat, rhs.mGammaSpace);
        }

        /**
         * \brief Compares this ImageInfo instance with another for inequality
         *
         * Two ImageInfo objects are considered different if any of their
         * dimensions, raw format, or gamma space differ.
         *
         * \param rhs The other ImageInfo to compare against
         *
         * \return \c true if any field differs, \c false otherwise
         */
        bool operator!=(const ImageInfo& rhs) const
        {
            return !(*this == rhs);
        }

    private:

        Extent2D<unsigned int> mSize; /*!< Image dimensions in pixels */
        ImageRawFormat::Type mFormat; /*!< Raw pixel format */
        GammaSpace mGammaSpace; /*!< Gamma space of the stored pixels */
    };
}

#endif // COCKTAIL_CORE_IMAGE_IMAGEINFO_HPP
