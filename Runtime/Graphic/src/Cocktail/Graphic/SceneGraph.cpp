#include <Cocktail/Graphic/SceneGraph.hpp>

namespace Ck
{
	SceneGraph::SceneGraph(Ref<TransformationNode> rootTransformationNode)
	{
		mRoot = CreateNode(std::move(rootTransformationNode));
	}
}
