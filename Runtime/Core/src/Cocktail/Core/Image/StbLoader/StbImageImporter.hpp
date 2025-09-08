#ifndef COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEIMPORTER_HPP
#define COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEIMPORTER_HPP

#include <stb_image/stb_image.h>

#include <Cocktail/Core/Asset/AssetImporter.hpp>
#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class StbImageImporter : public AssetImporter<Image, ImageImportParameters>
	{
	public:

		/**
		 * \brief 
		 */
		StbImageImporter();

		/**
		 * \brief 
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		std::shared_ptr<Image> LoadFromPath(const Path& path, const ImageImportParameters& parameters) override;

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters
		 * \return 
		 */
		std::shared_ptr<Image> LoadFromStream(InputStream& inputStream, const ImageImportParameters& parameters) override;

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
		bool SupportParameters(const ImageImportParameters& parameters) const override;

	private:
		
		/**
		 * \brief Callback implementation of read internally used by stb_image
		 * Read \p data with \p size bytes from a \p user InputStream
		 * \param user A pointer to the stream
		 * \param data 
		 * \param size 
		 * \return The number of bytes actually red
		 */
		static int ReadCallback(void* user, char* data, int size);

		/**
		 * \brief Callback implementation of skip internally used by stb_image
		 * Skips the next \p n bytes from the \p user InputStream
		 * \param user A pointer to the stream
		 * \param n The number of bytes to skip
		 */
		static void SkipCallback(void* user, int n);

		/**
		 * \brief 
		 * \param user 
		 * \return 
		 */
		static int EofCallback(void* user);

		stbi_io_callbacks mCallbacks;
	};
}

#endif // COCKTAIL_CORE_IMAGE_STBLOADER_STBIMAGEIMPORTER_HPP
