#ifndef COCKTAIL_GRAPHIC_SCENE_GLTF_OBJIMPORTER_HPP
#define COCKTAIL_GRAPHIC_SCENE_GLTF_OBJIMPORTER_HPP

#include <tiny_gltf.h>

#include <Cocktail/Core/Image/ImageLoader.hpp>

#include <Cocktail/Graphic/Scene/SceneContainer.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>

namespace Ck
{
	class GltfImporter : public Inherit<GltfImporter, Object, AssetImporter<SceneContainer, SceneImportParameters>>
	{
	public:

		/**
		 * \brief Constructor
		 * \param imageLoader 
		 */
		explicit GltfImporter(const Ref<ImageLoader>& imageLoader);

		/**
		 * \brief 
		 * \param path 
		 * \param parameters 
		 * \return 
		 */
		Ref<SceneContainer> LoadFromPath(const std::filesystem::path& path,	const SceneImportParameters& parameters) override;

		/**
		 * \brief 
		 * \param inputStream 
		 * \param parameters 
		 * \return 
		 */
		Ref<SceneContainer> LoadFromStream(const Ref<InputStream>& inputStream, const SceneImportParameters& parameters) override;

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
		bool SupportParameters(const SceneImportParameters& parameters) const override;

	private:

		static bool LoadImageData(tinygltf::Image* image, const int imageIndex, std::string* errors, std::string* warnings, int requestWidth, int requestHeight, const unsigned char* data, int size, void* userData);

        tinygltf::TinyGLTF mLoader;
		Ref<ImageLoader> mImageLoader;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_GLTF_OBJIMPORTER_HPP