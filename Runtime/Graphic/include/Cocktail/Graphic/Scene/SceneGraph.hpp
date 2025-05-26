#ifndef COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP

#include <Cocktail/Graphic/Scene/SceneNode.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API SceneGraph : public AcyclicGraph<SceneNode>
	{
	public:

		/**
		 * \brief
		 */
		SceneGraph(Scene* scene, std::shared_ptr<TransformationNode> rootTransformationNode);

	private:

		Scene* mScene;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENEGRAPH_HPP
