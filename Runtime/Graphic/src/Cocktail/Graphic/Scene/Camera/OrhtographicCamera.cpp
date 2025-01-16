#include <Cocktail/Graphic/Scene/Camera/OrhtographicCamera.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	Ref<OrthographicCamera> OrthographicCamera::Create(Ref<Scene> scene, Rectangle<unsigned int> area, Vector2<float> zBounds)
	{
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode();
		Ref<OrthographicCamera> camera = OrthographicCamera::New(std::move(transformationNode), area, zBounds);
		scene->AddCamera(camera);

		return camera;
	}

	OrthographicCamera::OrthographicCamera(Ref<TransformationNode> transformationNode, Rectangle<unsigned int> area, Vector2<float> zBounds) :
		Super(std::move(transformationNode)),
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
