#include <Cocktail/Graphic/Camera/Camera.hpp>
#include <Cocktail/Graphic/Camera/Controller/CameraController.hpp>

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
