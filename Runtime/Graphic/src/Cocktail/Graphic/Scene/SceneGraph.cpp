#include <Cocktail/Graphic/Scene/SceneGraph.hpp>

namespace Ck
{
	SceneGraph::SceneGraph(Scene* scene, Ref<TransformationNode> rootTransformationNode) :
		mScene(scene)
	{
		mRoot = CreateNode(mScene, std::move(rootTransformationNode));
	}
}
