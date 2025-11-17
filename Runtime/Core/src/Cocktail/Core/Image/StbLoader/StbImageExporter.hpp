#ifndef COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEEXPORTER_HPP
#define COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEEXPORTER_HPP

#include <Cocktail/Core/Asset/AssetExporter.hpp>
#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class StbImageExporter : public AssetExporter<Image, ImageExportParameters>
	{
	public:

		/**
		 * \brief 
		 * \param asset 
		 * \param path 
		 * \param parameters 
		 */
		void SaveToPath(const Image& asset, const Path& path, const ImageExportParameters& parameters) const override;

		/**
		 * \brief 
		 * \param asset 
		 * \param outputStream 
		 * \param parameters 
		 */
		void SaveToStream(const Image& asset, OutputStream<>& outputStream, const ImageExportParameters& parameters) const override;

		/**
		 * \brief 
		 * \param asset 
		 * \param parameters
		 * \return
		 */
		ByteArray SaveToMemory(const Image& asset, const ImageExportParameters& parameters) const override;

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
		bool SupportParameters(const ImageExportParameters& parameters) const override;
	};
}

#endif // COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEEXPORTER_HPP