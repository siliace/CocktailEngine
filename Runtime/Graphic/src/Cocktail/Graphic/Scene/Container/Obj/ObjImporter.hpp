#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJIMPORTER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJIMPORTER_HPP

#include <Cocktail/Core/Asset/AssetImporter.hpp>

#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
	class ObjImporter : public AssetImporter<SceneContainer, SceneImportParameters>
	{
	public:

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
		Ref<SceneContainer> LoadFromStream(InputStream& inputStream, const SceneImportParameters& parameters) override;
		
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
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_OBJ_OBJIMPORTER_HPP
