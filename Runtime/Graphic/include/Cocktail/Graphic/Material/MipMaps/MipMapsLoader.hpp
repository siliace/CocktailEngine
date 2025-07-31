#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>
#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_LOG_CATEGORY(MipMapsLoaderLogCategory, LogLevel::Info);

	class COCKTAIL_GRAPHIC_API MipMapsLoader : public AssetLoader<MipMaps, MipMapsImportParameters, MipMapsExportParameters>
	{
	public:
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP
