#include <Cocktail/Graphic/Scene/Camera/OrhtographicCamera.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <utility>

namespace Ck
{
	OrthographicCamera* OrthographicCamera::Create(std::shared_ptr<Scene> scene, String name, const Rectangle<float>& area, Vector2<float> zBounds)
	{
		std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode();

		UniquePtr<OrthographicCamera> camera = MakeUnique<OrthographicCamera>(std::move(transformationNode), std::move(name), area, zBounds);
		OrthographicCamera* cameraPtr = camera.Get();
		scene->AddCamera(std::move(camera));

		return cameraPtr;
	}

	OrthographicCamera::OrthographicCamera(std::shared_ptr<TransformationNode> transformationNode, String name, Rectangle<float> area, Vector2<float> zBounds) :
		Camera(std::move(transformationNode), std::move(name)),
		mArea(std::move(area)),
		mZBounds(zBounds)
	{
		// Nothing
	}

    const Rectangle<float>& OrthographicCamera::GetArea() const
    {
	    return mArea;
    }

    void OrthographicCamera::SetArea(const Rectangle<float>& area)
    {
        mArea = area;
    }

    Frustum<float> OrthographicCamera::ComputeFrustum() const
	{
	    return Frustum<float>::Orthographic(mArea.Extent, mZBounds, GetPosition(), -GetFront(), GetRight(), GetUp());
	}

	Matrix4<float> OrthographicCamera::ComputeProjectionViewMatrix()
	{
		Matrix4<float> projection;

		float left = mArea.Position.X();
		float right = left + mArea.Extent.Width;

		float bottom = mArea.Position.Y();
		float top = bottom + mArea.Extent.Height;

		float zNear = mZBounds.X();
		float zFar = mZBounds.Y();

		projection.At(0, 0) = 2.f / (right - left);
	    projection.At(1, 1) = 2.f / (top - bottom);
		projection.At(2, 2) = -2.f / (zFar - zNear);

	    projection.At(0, 3) = -(right + left) / (right - left);
	    projection.At(1, 3) = -(top + bottom) / (top - bottom);
	    projection.At(2, 3) = -(zFar + zNear) / (zFar - zNear);

	    projection.At(3, 3) = 1.f;

		return projection * ComputeViewMatrix();
	}
}
