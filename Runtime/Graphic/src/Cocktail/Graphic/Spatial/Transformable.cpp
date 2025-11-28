#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	void Transformable::Move(Vector3<float> offset)
	{
		Transformation transformation = GetLocalTransformation();

		transformation.SetTranslation(transformation.GetTranslation() + offset);
		mTransformationNode->SetLocalTransformation(transformation);
	}

    void Transformable::Rotate(Angle<float> angle, Vector3<float> axis)
    {
	    Transformation transformation = GetLocalTransformation();

	    Quaternion<float> rotation = Quaternion<float>::FromAngleAxis(angle, axis);
	    transformation.SetRotation(transformation.GetRotation().Compose(rotation));
	    mTransformationNode->SetLocalTransformation(transformation);
    }

    void Transformable::AttachTo(const Transformable& parent)
	{
		std::shared_ptr<TransformationNode> transformationNode = parent.GetTransformationNode();
		transformationNode->InsertChild(mTransformationNode);
	}

	Vector3<float> Transformable::GetRight() const
	{
		return mTransformationNode->GetRight();
	}

	Vector3<float> Transformable::GetUp() const
	{
		return mTransformationNode->GetUp();
	}

	Vector3<float> Transformable::GetFront() const
	{
		return mTransformationNode->GetFront();
	}

	Vector3<float> Transformable::GetPosition() const
	{
		return GetLocalTransformation().GetTranslation();
	}

	void Transformable::SetPosition(Vector3<float> position)
	{
		Transformation transformation = GetLocalTransformation();

		transformation.SetTranslation(position);
		mTransformationNode->SetLocalTransformation(transformation);
	}

	Quaternion<float> Transformable::GetRotation() const
	{
		return GetLocalTransformation().GetRotation();
	}

	void Transformable::SetRotation(Quaternion<float> rotation)
	{
		Transformation transformation = GetLocalTransformation();

		transformation.SetRotation(rotation);
		mTransformationNode->SetLocalTransformation(transformation);
	}

	const Transformation& Transformable::GetLocalTransformation() const
	{
		return mTransformationNode->GetLocalTransformation();
	}

	void Transformable::SetLocalTransformation(const Transformation& transformation)
	{
		mTransformationNode->SetLocalTransformation(transformation);
	}

	const Transformation& Transformable::GetWorldTransformation() const
	{
		return mTransformationNode->GetWorldTransformation();
	}

	std::shared_ptr<TransformationNode> Transformable::GetTransformationNode() const
	{
		return mTransformationNode;
	}

	Transformable::Transformable(std::shared_ptr<TransformationNode> transformationNode):
		mTransformationNode(std::move(transformationNode))
	{
		assert(mTransformationNode);
	}
}
