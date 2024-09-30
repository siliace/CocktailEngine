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
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		Ref<MipMaps> LoadFromPath(const std::filesystem::path& path, const MipMapsImportParameters& parameters) const override;

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters 
		 * \return 
		 */
		Ref<MipMaps> LoadFromStream(const Ref<InputStream>& inputStream, const MipMapsImportParameters& parameters) const override;

		/**
    	 * \brief 
    	 * \param buffer 
    	 * \param length 
    	 * \param parameters 
    	 * \return 
    	 */
    	Ref<MipMaps> LoadFromMemory(const void* buffer, std::size_t length, const MipMapsImportParameters& parameters) const override;

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
