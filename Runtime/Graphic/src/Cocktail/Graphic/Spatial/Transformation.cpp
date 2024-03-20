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

	Transformation Transformation::GetInverse(const Transformation& transformation) const
	{
		return Transformation(
			transformation.mRotation.RotateInverse(transformation.mTranslation),
			mRotation.GetConjugate(),
			mScale
		);
	}

	Vector3<float> Transformation::Apply(const Vector3<float>& vertex) const
	{
		return Matrix3<float>::FromQuaternion(mRotation) * Vector3<float>::HadamardProduct(mScale, vertex) + mTranslation;
	}

	Vector3<float> Transformation::GetTranslation() const
	{
		return mTranslation;
	}

	void Transformation::SetTranslation(Vector3<float> translation)
	{
		mTranslation = translation;
	}

	Quaternion<float> Transformation::GetRotation() const
	{
		return mRotation;
	}

	void Transformation::SetRotation(Quaternion<float> rotation)
	{
		mRotation = rotation;
	}

	Vector3<float> Transformation::GetScale() const
	{
		return mScale;
	}

	void Transformation::SetScale(Vector3<float> scale)
	{
		mScale = scale;
	}

	const Matrix4<float>& Transformation::ToMatrix() const
	{
		return mMatrix.Get([&]() {
			Matrix3<float> rotationMatrix = Matrix3<float>::FromQuaternion(mRotation);
			for (std::size_t i = 0; i < 3; i++)
			{
				Vector3 row = rotationMatrix.GetRow(i);
				row *= mScale.At(i);

				rotationMatrix.SetRow(i, row);
			}

			Matrix4<float> transformationMatrix;
			for (std::size_t i = 0; i < 3; i++)
			{
				Vector4 column(rotationMatrix.GetColumn(i), 0.f);
				transformationMatrix.SetColumn(i, column);
			}

			transformationMatrix.SetColumn(
				3, Vector4(mTranslation, 1.f)
			);

			return transformationMatrix;
		});
	}
}
