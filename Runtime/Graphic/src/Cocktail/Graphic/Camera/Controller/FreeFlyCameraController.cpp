#include <Cocktail/Core/Math/Quaternion.hpp>

#include <Cocktail/Graphic/Camera/Camera.hpp>
#include <Cocktail/Graphic/Camera/Controller/FreeFlyCameraController.hpp>

namespace Ck
{
	FreeFlyCameraViewController::FreeFlyCameraViewController(Ref<Camera> camera, Angle<float> pitchLimit) :
		mCamera(std::move(camera)),
		mPitchLimit(std::abs(pitchLimit))
	{
		assert(mPitchLimit > Angle<float>::Degree(0.f) && mPitchLimit <= Angle<float>::Degree(89.f));

		for (TranslationAxis translationAxis : Enum<TranslationAxis>::Values)
			EnableTranslationAxis(translationAxis);

		for (RotationAxis rotationAxis : Enum<RotationAxis>::Values)
			EnableRotationAxis(rotationAxis);
	}

	void FreeFlyCameraViewController::Update()
	{
		Vector3<float> position = mCamera->GetPosition();
		position += mTranslations[TranslationAxis::Right] * mCamera->GetRight();
		position += mTranslations[TranslationAxis::Up] * mCamera->GetUp();
		position += mTranslations[TranslationAxis::Front] * mCamera->GetFront();
		mCamera->SetPosition(position);

		Angle<float> yaw = mRotations[RotationAxis::Yaw];
		Angle<float> pitch = std::clamp(mRotations[RotationAxis::Pitch], -mPitchLimit, mPitchLimit);

		Vector3<float> frontAxis;
		frontAxis.X() = std::cos(yaw) * std::cos(pitch);
		frontAxis.Y() = std::sin(pitch);
		frontAxis.Z() = std::sin(yaw) * std::cos(pitch);
		frontAxis = Vector3<float>::Normalize(frontAxis);

		Vector3<float> rightAxis = Vector3<float>::Normalize(
			Vector3<float>::CrossProduct(Vector3<float>::Up(), frontAxis)
		);

		Vector3<float> upAxis = Vector3<float>::CrossProduct(frontAxis, rightAxis);

		Matrix3<float> rotationMatrix;
		rotationMatrix.SetColumn(0, rightAxis);
		rotationMatrix.SetColumn(1, upAxis);
		rotationMatrix.SetColumn(2, frontAxis);
		mCamera->SetRotation(Quaternion<float>::FromMatrix(rotationMatrix));

		for (TranslationAxis translationAxis : Enum<TranslationAxis>::Values)
			mTranslations[translationAxis] = 0.f;
	}

	void FreeFlyCameraViewController::EnableTranslationAxis(TranslationAxis axis, bool enable)
	{
		mEnabledTranslations[axis] = enable;
	}

	void FreeFlyCameraViewController::EnableRotationAxis(RotationAxis axis, bool enable)
	{
		mEnabledRotations[axis] = enable;
	}

	void FreeFlyCameraViewController::Translate(TranslationAxis axis, float amount)
	{
		if (mEnabledTranslations[axis])
			mTranslations[axis] += amount;
	}

	void FreeFlyCameraViewController::Rotate(RotationAxis axis, Angle<float> amount)
	{
		if (mEnabledRotations[axis])
			mRotations[axis] += amount;
	}
}
