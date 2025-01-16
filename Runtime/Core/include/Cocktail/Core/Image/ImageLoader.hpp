#ifndef COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP
#define COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>
#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class ImageLoader : public AssetLoader<Image, ImageImportParameters, ImageExportParameters>
	{
	public:
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP
