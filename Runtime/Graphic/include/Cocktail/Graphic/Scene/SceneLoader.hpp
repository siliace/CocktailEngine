#ifndef COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP

#include <Cocktail/Core/Asset/AssetLoader.hpp>

#include <Cocktail/Graphic/Scene/SceneContainer.hpp>

namespace Ck
{
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
		bool FlipUp = false;
	};

	struct SceneExportParameters
	{
		/**
		 * \brief 
		 */
		std::filesystem::path BaseDirectory;
	};

	class SceneLoader : public Extends<SceneLoader, AssetLoader<SceneContainer, SceneImportParameters, SceneExportParameters>>
	{
	public:
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENELOADER_HPP