#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>
#include <Cocktail/Graphic/Scene/Camera/Controller/CameraController.hpp>

namespace Ck
{
	Matrix4<float> Camera::ComputeViewMatrix() const
	{
		return GetTransformationNode()->GetWorldTransformation().GetInverse().ToMatrix();
	}

	Camera::Camera(Ref<TransformationNode> transformationNode) :
		Super(std::move(transformationNode))
	{
		/// Nothing
	}
}
