#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP

#include <Cocktail/Core/Asset/AssetImporter.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class DDSImporter final : public AssetImporter<MipMaps, MipMapsImportParameters>
	{
	public:

		/**
		 * \brief 
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		std::shared_ptr<MipMaps> LoadFromPath(const Path& path, const MipMapsImportParameters& parameters) override;

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters 
		 * \return 
		 */
		std::shared_ptr<MipMaps> LoadFromStream(InputStream& inputStream, const MipMapsImportParameters& parameters) override;

		/**
    	 * \brief 
    	 * \param extension 
    	 * \return 
    	 */
    	bool SupportExtension(StringView extension) const override;

		/**
    	 * \brief 
    	 * \param parameters 
    	 * \return 
    	 */
    	bool SupportParameters(const MipMapsImportParameters& parameters) const override;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP
