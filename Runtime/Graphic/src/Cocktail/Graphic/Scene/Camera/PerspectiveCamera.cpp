#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	Ref<PerspectiveCamera> PerspectiveCamera::Create(Ref<Scene> scene, Angle<float> fov, float aspectRatio, Vector2<float> zBounds)
	{
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode();
		Ref<PerspectiveCamera> camera = PerspectiveCamera::New(std::move(transformationNode), fov, aspectRatio, zBounds);
		scene->AddCamera(camera);

		return camera;
	}

	PerspectiveCamera::PerspectiveCamera(Ref<TransformationNode> transformationNode, Angle<float> fov, float aspectRatio, Vector2<float> zBounds) :
		Super(std::move(transformationNode)),
		mFov(fov),
		mAspectRatio(aspectRatio),
		mZBounds(zBounds)
	{
		/// Nothing
	}

	Angle<float> PerspectiveCamera::GetFov() const
	{
		return mFov;
	}

	void PerspectiveCamera::SetFov(Angle<float> fov)
	{
		mFov = fov;
	}

	float PerspectiveCamera::GetAspectRatio() const
	{
		return mAspectRatio;
	}

	void PerspectiveCamera::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
	}

	Frustum<float> PerspectiveCamera::ComputeFrustum() const
	{
		return Frustum<float>::Perspective(mFov, mAspectRatio, mZBounds, GetPosition(), GetPosition() + GetFront(), GetUp());
	}

	Matrix4<float> PerspectiveCamera::ComputeProjectionViewMatrix()
	{
		float front = mZBounds.X();
		float back = mZBounds.Y();
		float top = front * std::tan(mFov / 2.f);
		float right = top * mAspectRatio;

		Matrix4<float> projection(0.f);
		projection.At(0, 0) = front / right;
		projection.At(1, 1) = front / top;
		projection.At(2, 2) = -(back + front) / (back - front);
		projection.At(2, 3) = -(2 * back * front) / (back - front);
		projection.At(3, 2) = -1.f;

		return projection * ComputeViewMatrix();
	}
}
