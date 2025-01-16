#ifndef COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP

#include <Cocktail/Graphic/Scene/SceneNode.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API SceneGraph : public Extends<SceneGraph, AcyclicGraph<SceneNode>>
	{
	public:

		/**
		 * \brief
		 */
		SceneGraph(Scene* scene, Ref<TransformationNode> rootTransformationNode);

	private:

		Scene* mScene;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
