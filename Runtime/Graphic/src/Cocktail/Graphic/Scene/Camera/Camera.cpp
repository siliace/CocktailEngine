#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>

namespace Ck
{
    Matrix4<float> Camera::ComputeViewMatrix() const
	{
		return GetTransformationNode()->GetWorldTransformation().GetInverse().ToMatrix();
	}

    const String& Camera::GetName() const
    {
        return mName;
    }

    Camera::Camera(TransformationNode* transformationNode, String name) :
		Transformable(transformationNode),
        mName(std::move(name))
	{
		/// Nothing
	}
}
