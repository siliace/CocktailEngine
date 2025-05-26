#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>

namespace Ck
{
	Matrix4<float> Camera::ComputeViewMatrix() const
	{
		return GetTransformationNode()->GetWorldTransformation().GetInverse().ToMatrix();
	}

	Camera::Camera(std::shared_ptr<TransformationNode> transformationNode) :
		Transformable(std::move(transformationNode))
	{
		/// Nothing
	}
}
