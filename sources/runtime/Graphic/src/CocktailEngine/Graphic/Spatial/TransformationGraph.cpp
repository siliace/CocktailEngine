#include <CocktailEngine/Graphic/Spatial/TransformationGraph.hpp>

namespace Ck
{
	TransformationGraph::TransformationGraph()
	{
		mRoot = CreateNode(Transformation::Identity());
	}
}
