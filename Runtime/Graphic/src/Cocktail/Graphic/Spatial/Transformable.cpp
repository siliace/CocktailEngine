#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	Ref<TransformationNode> Transformable::GetTransformationNode() const
	{
		return mTransformationNode;
	}

	Transformable::Transformable(Ref<TransformationNode> transformationNode):
		mTransformationNode(std::move(transformationNode))
	{
		assert(mTransformationNode != nullptr);
	}
}
