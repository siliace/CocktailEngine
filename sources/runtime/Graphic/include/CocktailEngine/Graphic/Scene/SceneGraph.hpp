#ifndef COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP

#include <CocktailEngine/Graphic/Scene/SceneNode.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief
	 */
	class COCKTAILENGINE_GRAPHIC_API SceneGraph : public AcyclicGraph<SceneNode>
	{
	public:

		/**
		 * \brief
		 */
		SceneGraph(Scene* scene, TransformationNode* rootTransformationNode);

	private:

		Scene* mScene;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
