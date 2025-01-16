#include <Cocktail/Core/Math/Matrix/Matrix3.hpp>

#include <Cocktail/Graphic/Spatial/Transformation.hpp>

namespace Ck
{
	Transformation Transformation::Identity()
	{
		return Transformation(
			Vector3<float>::Zero(),
			Quaternion<float>::Identity(),
			Vector3<float>::Unit()
		);
	}

	Transformation Transformation::Decompose(const Matrix4<float>& transformationMatrix)
	{
		Transformation transformation;

		Vector4<float> rightAxis = transformationMatrix.GetColumn(0);
		Vector4<float> upAxis = transformationMatrix.GetColumn(1);
		Vector4<float> frontAxis = transformationMatrix.GetColumn(2);

		Vector3<float> scale;
		scale.X() = rightAxis.GetLength();
		scale.Y() = upAxis.GetLength();
		scale.Z() = frontAxis.GetLength();
		transformation.SetScale(scale);

		Matrix3<float> rotationMatrix;
		rotationMatrix.SetColumn(0, Vector3<float>::Normalize(Vector3(rightAxis.X(), rightAxis.Y(), rightAxis.Z())));
		rotationMatrix.SetColumn(1, Vector3<float>::Normalize(Vector3(upAxis.X(), upAxis.Y(), upAxis.Z())));
		rotationMatrix.SetColumn(2, Vector3<float>::Normalize(Vector3(frontAxis.X(), frontAxis.Y(), frontAxis.Z())));
		transformation.SetRotation(Quaternion<float>::FromMatrix(rotationMatrix));

		Vector4<float> translation = transformationMatrix.GetColumn(3);
		transformation.SetTranslation(Vector3(translation.X(), translation.Y(), translation.Z()));

		return transformation;
	}

	Transformation::Transformation(Vector3<float> translation, Quaternion<float> rotation, Vector3<float> scale):
		mTranslation(translation),
		mRotation(rotation),
		mScale(scale)
	{
		/// Nothing
	}

	bool Transformation::IsScaleUniform() const
	{
		return NearlyEqual(mScale.X(), mScale.Y()) && NearlyEqual(mScale.X(), mScale.Z());
	}

	Transformation Transformation::Compose(const Transformation& parent) const
	{
		return Transformation(
			mRotation.Rotate(parent.mTranslation) + mTranslation,
			mRotation.Compose(parent.mRotation),
			Vector3<float>::HadamardProduct(parent.mScale, mScale)
		);
	}

	Transformation Transformation::GetInverse() const
	{
		return Transformation(mRotation.RotateInverse(-mTranslation), mRotation.GetConjugate(), 1.f / mScale);
	}

	Vector3<float> Transformation::Apply(const Vector3<float>& vertex) const
	{
		return Matrix3<float>::FromQuaternion(mRotation) * Vector3<float>::HadamardProduct(mScale, vertex) + mTranslation;
	}

	Vector3<float> Transformation::GetTranslation() const
	{
		return mTranslation;
	}

	Transformation& Transformation::SetTranslation(Vector3<float> translation)
	{
		mMatrix.SetDirty();
		mTranslation = translation;

		return *this;
	}

	Quaternion<float> Transformation::GetRotation() const
	{
		return mRotation;
	}

	Transformation& Transformation::SetRotation(Quaternion<float> rotation)
	{
		mMatrix.SetDirty();
		mRotation = rotation;

		return *this;
	}

	Vector3<float> Transformation::GetScale() const
	{
		return mScale;
	}

	Transformation& Transformation::SetScale(float scale)
	{
		return SetScale(Vector3<float>(scale));
	}

	Transformation& Transformation::SetScale(Vector3<float> scale)
	{
		mMatrix.SetDirty();
		mScale = scale;

		return *this;
	}

	const Matrix4<float>& Transformation::ToMatrix() const
	{
		return mMatrix.Get([&]() {
			Matrix4<float> transformationMatrix = Matrix4<float>::Identity();
			Matrix3<float> rotationMatrix = Matrix3<float>::FromQuaternion(mRotation);
			
			for (std::size_t i = 0; i < 3; i++)
			{
				Vector3 row = rotationMatrix.GetRow(i);
				row *= mScale.At(i);

				rotationMatrix.SetRow(i, row);
			}

			for (std::size_t i = 0; i < 3; i++)
			{
				Vector4 row(rotationMatrix.GetRow(i), 0.f);
				transformationMatrix.SetRow(i, row);
			}

			transformationMatrix.SetColumn(3, Vector4(mTranslation, 1.f));

			return transformationMatrix;
		});
	}
}
