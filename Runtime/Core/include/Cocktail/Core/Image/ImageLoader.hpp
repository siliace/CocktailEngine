#ifndef COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP
#define COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>
#include <Cocktail/Core/Image/Image.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>

namespace Ck
{
	CK_DECLARE_LOG_CATEGORY(ImageLoaderLogCategory, Ck::LogLevel::Info);

	/**
	 * \brief 
	 */
	class ImageLoader : public AssetLoader<Image, ImageImportParameters, ImageExportParameters>
	{
	public:
	};
}

#endif // COCKTAIL_CORE_IMAGE_IMAGELOADER_HPP
