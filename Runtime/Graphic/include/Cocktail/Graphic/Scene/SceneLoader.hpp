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
		 * \brief
		 */
		std::filesystem::path BaseDirectory;

		/**
		 * \brief Specify to the importer to rearrange vertices and indices to reduce memory footprint 
		 */
		bool OptimizeIndices = true;
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
