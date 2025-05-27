#include <Cocktail/Graphic/Scene/Camera/OrhtographicCamera.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	OrthographicCamera* OrthographicCamera::Create(std::shared_ptr<Scene> scene, Rectangle<unsigned int> area, Vector2<float> zBounds)
	{
		std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode();

		std::unique_ptr<OrthographicCamera> camera = std::make_unique<OrthographicCamera>(std::move(transformationNode), area, zBounds);
		OrthographicCamera* cameraPtr = camera.get();
		scene->AddCamera(std::move(camera));

		return cameraPtr;
	}

	OrthographicCamera::OrthographicCamera(std::shared_ptr<TransformationNode> transformationNode, Rectangle<unsigned int> area, Vector2<float> zBounds) :
		Camera(std::move(transformationNode)),
		mArea(area),
		mZBounds(zBounds)
	{
		// Nothing
	}

	Frustum<float> OrthographicCamera::ComputeFrustum() const
	{
		return {};
	}

	Matrix4<float> OrthographicCamera::ComputeProjectionViewMatrix()
	{
		Matrix4<float> projection;

		float left = static_cast<float>(mArea.Position.X());
		float right = left + static_cast<float>(mArea.Extent.X());

		float bottom = mArea.Position.Y();
		float top = bottom + static_cast<float>(mArea.Extent.Y());

		float zNear = mZBounds.X();
		float zFar = mZBounds.Y();

		projection.At(0, 0) = 2.f / (right - left);
		projection.At(0, 3) = -((right + left) / (right - left));
		projection.At(1, 1) = 2.f / (top - bottom);
		projection.At(1, 3) = -((top + bottom) / (top - bottom));
		projection.At(2, 2) = -2.f / (zFar - zNear);
		projection.At(2, 3) = -((zFar - zNear) / (zFar - zNear));
		projection.At(3, 3) = 1.f;

		return projection * ComputeViewMatrix();
	}
}
