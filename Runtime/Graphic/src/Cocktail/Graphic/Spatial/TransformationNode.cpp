#include <Cocktail/Graphic/Spatial/TransformationNode.hpp>

namespace Ck
{
	TransformationNode::TransformationNode(Transformation localTransformation) :
		mLocalTransformation(std::move(localTransformation))
	{
		/// Nothing
	}

	Vector3<float> TransformationNode::GetRight() const
	{
		Matrix4<float> matrix = GetWorldTransformation().ToMatrix();
		Vector4<float> right = matrix.GetColumn(0);

		return Vector3<float>::Normalize(Vector3(right.X(), right.Y(), right.Z()));
	}

	Vector3<float> TransformationNode::GetUp() const
	{
		Matrix4<float> matrix = GetWorldTransformation().ToMatrix();
		Vector4<float> up = matrix.GetColumn(1);

		return Vector3<float>::Normalize(Vector3(up.X(), up.Y(), up.Z()));
	}

	Vector3<float> TransformationNode::GetFront() const
	{
		Matrix4<float> matrix = GetWorldTransformation().ToMatrix();
		Vector4<float> front = matrix.GetColumn(2);

		return Vector3<float>::Normalize(Vector3(front.X(), front.Y(), front.Z()));
	}

	const Transformation& TransformationNode::GetLocalTransformation() const
	{
		return mLocalTransformation;
	}

	void TransformationNode::SetLocalTransformation(const Transformation& transformation)
	{
		SetDirty(true);
		mLocalTransformation = transformation;
	}

	const Transformation& TransformationNode::GetWorldTransformation() const
	{
		return mWorldTransformation.Get([&]() {
			if (TransformationNode* parent = GetParent())
				return parent->GetWorldTransformation().Compose(mLocalTransformation);

			return mLocalTransformation;

		});
	}

	void TransformationNode::SetDirty(bool dirty)
	{
		mWorldTransformation.SetDirty(dirty);
		for (Ref<TransformationNode> childNode : GetChildren())
			childNode->SetDirty(dirty);
	}
}
