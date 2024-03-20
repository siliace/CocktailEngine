#include <Cocktail/Graphic/Spatial/TransformationNode.hpp>

namespace Ck
{
	TransformationNode::TransformationNode(Transformation localTransformation) :
		mLocalTransformation(std::move(localTransformation))
	{
		/// Nothing
	}

	const Transformation& TransformationNode::GetLocalTransformation() const
	{
		return mLocalTransformation;
	}

	const Transformation& TransformationNode::GetWorldTransformation() const
	{
		return mWorldTransformation.Get([&]() {
			if (Ref<TransformationNode> parent = GetParent())
				return parent->GetWorldTransformation().Compose(mLocalTransformation);

			return mLocalTransformation;
		});
	}
}
