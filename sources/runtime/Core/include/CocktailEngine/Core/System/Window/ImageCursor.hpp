#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP

#include <CocktailEngine/Core/Extent2D.hpp>
#include <CocktailEngine/Core/System/Window/Cursor.hpp>

namespace Ck
{
    /**
     * \brief Interface of a Cursor whose shape comes from an application-provided Image
     *
     * Created through WindowFactory::CreateCursor, which converts the image into a native
     * cursor once and for all, so the source image can be released as soon as the cursor
     * exists. The caller owns the result and has to keep it alive for as long as a window
     * displays it.
     *
     * \see WindowFactory::CreateCursor
     */
    class ImageCursor : public Cursor
    {
    public:

        /**
         * \brief Retrieves the point of the cursor image that designates the pointed pixel
         *
         * Expressed in pixels from the top-left corner of the image, Width carrying the
         * abscissa and Height the ordinate. The tip of an arrow puts it at the origin,
         * whereas a cross-hair puts it at the center of the image.
         *
         * \return The hot spot as a 2D extent (x, y)
         */
        virtual Extent2D<unsigned int> GetHotSpot() const = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_IMAGECURSOR_HPP
