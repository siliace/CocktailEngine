#include <Cocktail/Graphic/Scene/SceneGraph.hpp>

namespace Ck
{
	SceneGraph::SceneGraph(Ref<TransformationNode> rootTransformationNode)
	{
		mRoot = CreateNode(std::move(rootTransformationNode));
	}
}
