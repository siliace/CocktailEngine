#include <Cocktail/Graphic/Spatial/TransformationGraph.hpp>

namespace Ck
{
	TransformationGraph::TransformationGraph()
	{
		mRoot = CreateNode(Transformation::Identity());
	}
}
