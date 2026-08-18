#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_IMAGECURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_IMAGECURSOR_HPP

#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/System/Unix/Xlib/Xlib.hpp>
#include <CocktailEngine/Core/System/Window/ImageCursor.hpp>

namespace Ck::Detail::Xlib
{
    /**
     * \brief Xlib implementation of an ImageCursor, backed by an Xcursor image
     *
     * Goes through the Xcursor extension rather than through plain Xlib, a cursor built
     * from two bitmaps being limited to two colors whereas Xcursor takes a full ARGB
     * image.
     */
    class ImageCursor : public Ck::ImageCursor
    {
    public:

        /**
         * \brief Builds a native cursor out of an image
         *
         * The image is packed into an Xcursor image and uploaded to the server, so the
         * source image is no longer needed once the constructor returns.
         *
         * \param display The shared display connection, which has to outlive this cursor
         * \param image The image to use as the cursor shape, expected in the ImageRawFormat::Type::RedGreenBlueAlpha8 raw format
         * \param hotspot The pixel of the image designating the pointed position, from its top-left corner
         */
        ImageCursor(::Display* display, const Image& image, const Extent2D<unsigned int>& hotspot);

        /**
         * \brief Releases the cursor on the display connection
         */
        ~ImageCursor() override;

        Extent2D<unsigned int> GetHotSpot() const override;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return The X11 cursor identifier, as an untyped pointer
         */
        void* GetSystemHandle() const override;

    private:

        /// Connection the cursor was created on, owned by the service provider.
        ::Display* mDisplay;

        Extent2D<unsigned int> mHotSpot;

        /// X11 cursor identifier, owned by this cursor.
        ::Cursor mHandle;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_XLIB_IMAGECURSOR_HPP
