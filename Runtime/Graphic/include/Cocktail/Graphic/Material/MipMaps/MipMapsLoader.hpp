#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>
#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API MipMapsLoader : public Extends<MipMapsLoader, AssetLoader<MipMaps, MipMapsImportParameters, MipMapsExportParameters>>
	{
	public:
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPSLOADER_HPP
