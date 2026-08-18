#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP

#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/ImageCursor.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Win32 implementation of an ImageCursor, backed by an HCURSOR
     */
    class ImageCursor : public Ck::ImageCursor
    {
    public:

        /**
         * \brief Builds a native cursor out of an image
         *
         * The image is copied into a device-independent bitmap, its channels being
         * reordered on the way since Win32 expects them the other way around, so the
         * source image is no longer needed once the constructor returns.
         *
         * \param image The image to use as the cursor shape, expected in the ImageRawFormat::Type::RedGreenBlueAlpha8 raw format
         * \param hotspot The pixel of the image designating the pointed position, from its top-left corner
         */
        ImageCursor(const Image& image, const Extent2D<unsigned int>& hotspot);

        /**
         * \brief Destroys the native cursor
         */
        ~ImageCursor() override;

        Extent2D<unsigned int> GetHotSpot() const override;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return The HCURSOR of this cursor, as an untyped pointer
         */
        void* GetSystemHandle() const override;

    private:

        Extent2D<unsigned int> mHotspot;

        /// Native cursor built from the image, owned by this cursor.
        HCURSOR mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_IMAGECURSOR_HPP
