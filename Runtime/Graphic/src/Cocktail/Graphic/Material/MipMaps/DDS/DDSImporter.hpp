#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP

#include <Cocktail/Core/Asset/AssetImporter.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class DDSImporter final : public Inherit<DDSImporter, Object, AssetImporter<MipMaps, MipMapsImportParameters>>
	{
	public:

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters 
		 * \return 
		 */
		Ref<MipMaps> LoadFromStream(const Ref<InputStream>& inputStream, const MipMapsImportParameters& parameters) override;

		/**
    	 * \brief 
    	 * \param extension 
    	 * \return 
    	 */
    	bool SupportExtension(std::string_view extension) const override;

		/**
    	 * \brief 
    	 * \param parameters 
    	 * \return 
    	 */
    	bool SupportParameters(const MipMapsImportParameters& parameters) const override;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_DDS_DDSIMPORTER_HPP