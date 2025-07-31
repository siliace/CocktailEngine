#ifndef COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>

#include <Cocktail/Graphic/Scene/Container/SceneContainer.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_LOG_CATEGORY(SceneLoaderLogCategory, LogLevel::Info);

	/**
	 * \brief 
	 */
	struct SceneImportParameters
	{
		/**
		 * \brief The base directory of the scene
		 * Used to resolve paths relatives to the scene (like textures files)
		 */
		std::filesystem::path BaseDirectory;

		/**
		 * \brief Flag indicating whether to flip Y component of UV vertex component
		 */
		bool FlipUpUV = false;
	};

	struct SceneExportParameters
	{
		/**
		 * \brief 
		 */
		std::filesystem::path BaseDirectory;
	};

	class SceneLoader : public AssetLoader<SceneContainer, SceneImportParameters, SceneExportParameters>
	{
	public:
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP
