#include <CocktailEngine/Graphic/Scene/SceneGraph.hpp>

namespace Ck
{
	SceneGraph::SceneGraph(Scene* scene, TransformationNode* rootTransformationNode) :
		mScene(scene)
	{
		mRoot = CreateNode(mScene, Move(rootTransformationNode));
	}
}
