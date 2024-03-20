#include <Cocktail/Graphic/SceneNode.hpp>

namespace Ck
{
	SceneNode::SceneNode(Ref<TransformationNode> transformationNode) :
		Inherit(std::move(transformationNode))
	{
		/// Nothing
	}

	void SceneNode::AddToQueue(RenderQueue& queue)
	{
		queue.PushStaticMesh({});
	}
}
