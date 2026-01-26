#ifndef COCKTAIL_CORE_IMAGE_IMAGEARRAY_HPP
#define COCKTAIL_CORE_IMAGE_IMAGEARRAY_HPP

#include <Cocktail/Core/Image/Image.hpp>
#include <Cocktail/Core/Utility/ByteArray.hpp>

namespace Ck
{
    /**
     * \brief Container for a collection of images sharing the same format and dimensions
     *
     * ImageArray stores multiple image slices with identical characteristics
     * (size, raw format and gamma space) in a single contiguous memory buffer.
     *
     * This class is typically used to represent:
     * - texture arrays
     * - image stacks
     * - multi-layer images
     *
     * All images added to the array must be compatible with the \ref ImageInfo
     * provided at construction time.
     */
    class COCKTAIL_CORE_API ImageArray
    {
    public:

        /**
         * \brief Constructs an image array with the given image description
         *
         * Initializes an empty image array configured to store images matching
         * the provided \ref ImageInfo.
         *
         * \param imageInfo Description of the images stored in the array
         */
        explicit ImageArray(const ImageInfo& imageInfo);

        /**
         * \brief Adds an image to the array
         *
         * Appends a full \ref Image to the array as a new slice.
         * The image must match the array's size, format and gamma space.
         *
         * \param image Image to add to the array
         */
        void Add(const Image& image);

        /**
         * \brief Adds an image view to the array
         *
         * Appends the pixel data referenced by an \ref ImageView as a new slice.
         * The view must reference image data compatible with the array configuration.
         *
         * \param image Image view to add to the array
         */
        void Add(const ImageView& image);

        /**
         * \brief Adds an image array to the array
         *
         * Appends the pixel data referenced by an \ref ImageArray as new slices.
         * The view must reference image data compatible with the array configuration.
         *
         * \param imageArray Image array to add to the array
         */
        void Add(const ImageArray& imageArray);

        /**
         * \brief Retrieves a view of a specific image slice
         *
         * Returns an \ref ImageView referencing the pixel data of the requested
         * slice within the array.
         *
         * \param slice Index of the slice to retrieve
         *
         * \return Image view representing the selected slice
         */
        ImageView GetSlice(unsigned int slice) const;

        /**
         * \brief Returns the dimensions of each image slice
         *
         * \return Image width and height in pixels
         */
        Extent2D<unsigned int> GetSize() const;

        /**
         * \brief Returns the raw pixel format of the images
         *
         * \return Raw image format used by all slices
         */
        ImageRawFormat::Type GetRawFormat() const;

        /**
         * \brief Returns the gamma space of the images
         *
         * \return Gamma space used by all slices
         */
        GammaSpace GetGammaSpace() const;

        /**
         * \brief Returns the image description used by the array
         *
         * \return Reference to the image information structure
         */
        const ImageInfo& GetImageInfo() const;

        /**
         * \brief Returns the raw pixel buffer containing all slices
         *
         * The returned buffer contains all image slices stored sequentially
         * in memory.
         *
         * \return Reference to the underlying pixel buffer
         */
        const LargeByteArray& GetPixels() const;

        /**
         * \brief Returns the number of image slices in the array
         *
         * \return Number of stored image slices
         */
        unsigned int GetSliceCount() const;

    private:

        /**
         * \brief Returns the size in bytes of a single image slice
         *
         * \return Size of one slice in bytes
         */
        Uint64 GetSliceSize() const;

        /**
         * \brief Computes the byte offset of a given slice in the pixel buffer
         *
         * \param slice Index of the slice
         *
         * \return Byte offset of the slice in the pixel buffer
         */
        Uint64 GetSliceOffset(unsigned int slice) const;

        ImageInfo mImageInfo; /*!< Image metadata: size, format, gamma */
        LargeByteArray mPixels; /*!< Raw pixel storage */
        unsigned int mSliceCount; /*!< Number of images in the array */
    };
}

#endif // COCKTAIL_CORE_IMAGE_IMAGEARRAY_HPP
