#ifndef COCKTAILENGINE_CORE_IMAGE_IMAGELOADER_HPP
#define COCKTAILENGINE_CORE_IMAGE_IMAGELOADER_HPP

#include <CocktailEngine/Core/Asset/AssetLoader.hpp>
#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/Log/LogCategory.hpp>

namespace Ck
{
    COCKTAIL_DECLARE_LOG_CATEGORY(ImageLoaderLogCategory, Ck::LogLevel::Info);

    /**
     * \brief Asset loader responsible for image resources
     *
     * The ImageLoader handles the loading and exporting of \ref Image assets.
     * It is responsible for decoding image data from supported file formats,
     * converting it into the engine's internal image representation, and
     * optionally exporting images back to disk.
     *
     * This loader works in conjunction with:
     * - \ref ImageImportParameters to control how images are imported
     * - \ref ImageExportParameters to control how images are exported
     *
     * The ImageLoader is typically registered by the \ref ImageServiceProvider
     * during the application initialization phase.
     */
    class ImageLoader : public AssetLoader<Image, ImageImportParameters, ImageExportParameters>
    {
    public:
    };
}

#endif // COCKTAILENGINE_CORE_IMAGE_IMAGELOADER_HPP
