#ifndef COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP
#define COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP

#include <Cocktail/Core/Math/Quaternion.hpp>
#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Matrix3 : public SquareMatrix<Matrix3, Vector3, T>
	{
	public:

		/**
		 * \brief
		 * \param matrix4 
		 * \return 
		 */
		static Matrix3<T> From(const Matrix4<T>& matrix4)
		{
			return FromRows(matrix4.GetRow(0).XYZ(), matrix4.GetRow(1).XYZ(), matrix4.GetRow(2).XYZ());
		}

		/**
		 * \brief
		 * \param quaternion
		 */
		static Matrix3 FromQuaternion(const Quaternion<T>& quaternion)
		{
			Matrix3<T> matrix;

			T qw = quaternion.W;
			T qx = quaternion.X;
			T qy = quaternion.Y;
			T qz = quaternion.Z;

			T qx2 = qx * qx;
			T qy2 = qy * qy;
			T qz2 = qz * qz;

			matrix.At(0, 0) = static_cast<T>(1) - static_cast<T>(2) * qy2 - static_cast<T>(2) * qz2;
			matrix.At(0, 1) = static_cast<T>(2) * qx * qy - static_cast<T>(2) * qz * qw;
			matrix.At(0, 2) = static_cast<T>(2) * qx * qz + static_cast<T>(2) * qy * qw;

			matrix.At(1, 0) = static_cast<T>(2) * qx * qy + static_cast<T>(2) * qz * qw;
			matrix.At(1, 1) = static_cast<T>(1) - static_cast<T>(2) * qx2 - static_cast<T>(2) * qz2;
			matrix.At(1, 2) = static_cast<T>(2) * qy * qz - static_cast<T>(2) * qx * qw;

			matrix.At(2, 0) = static_cast<T>(2) * qx * qz - static_cast<T>(2) * qy * qw;
			matrix.At(2, 1) = static_cast<T>(2) * qy * qz + static_cast<T>(2) * qx * qw;
			matrix.At(2, 2) = static_cast<T>(1) - static_cast<T>(2) * qx2 - static_cast<T>(2) * qy2;

			return matrix;
		}

		/**
		 * \brief Create a matrix by specifying its rows
		 * \param firstRow 
		 * \param secondRow 
		 * \param thirdRow 
		 * \return 
		 */
		static Matrix3<T> FromRows(const Vector3<T>& firstRow, const Vector3<float>& secondRow, const Vector3<float>& thirdRow)
		{
			Matrix3<float> matrix;
			matrix.SetRow(0, firstRow);
			matrix.SetRow(1, secondRow);
			matrix.SetRow(2, thirdRow);
			return matrix;
		}

		/**
		 * \brief 
		 */
		Matrix3() = default;

		/**
		 * \brief 
		 * \param diagonal 
		 */
		explicit Matrix3(const Vector3<T>& diagonal) :
			SquareMatrix<Matrix3, Vector3, T>(diagonal)
		{
			/// Nothing
		}

		Matrix3<T> Inverse() const
		{
			T determinantInverse = static_cast<T>(1) / GetDeterminant();

			Matrix3<T> inverse;
			inverse.At(0, 0) = (this->At(1, 1) * this->At(2, 2) - this->At(2, 1) * this->At(1, 2)) * determinantInverse;
			inverse.At(0, 1) = (this->At(0, 2) * this->At(2, 1) - this->At(0, 1) * this->At(2, 2)) * determinantInverse;
			inverse.At(0, 2) = (this->At(0, 1) * this->At(1, 2) - this->At(0, 2) * this->At(1, 1)) * determinantInverse;
			inverse.At(1, 0) = (this->At(1, 2) * this->At(2, 0) - this->At(1, 0) * this->At(2, 2)) * determinantInverse;
			inverse.At(1, 1) = (this->At(0, 0) * this->At(2, 2) - this->At(0, 2) * this->At(2, 0)) * determinantInverse;
			inverse.At(1, 2) = (this->At(1, 0) * this->At(0, 2) - this->At(0, 0) * this->At(1, 2)) * determinantInverse;
			inverse.At(2, 0) = (this->At(1, 0) * this->At(2, 1) - this->At(2, 0) * this->At(1, 1)) * determinantInverse;
			inverse.At(2, 1) = (this->At(2, 0) * this->At(0, 1) - this->At(0, 0) * this->At(2, 1)) * determinantInverse;
			inverse.At(2, 2) = (this->At(0, 0) * this->At(1, 1) - this->At(1, 0) * this->At(0, 1)) * determinantInverse;

			return inverse;
		}

		T GetDeterminant() const
		{
			return this->At(0, 0) * (this->At(1, 1) * this->At(2, 2) - this->At(2, 1) * this->At(1, 2)) -
				this->At(0, 1) * (this->At(1, 0) * this->At(2, 2) - this->At(1, 2) * this->At(2, 0)) +
				this->At(0, 2) * (this->At(1, 0) * this->At(2, 1) - this->At(1, 1) * this->At(2, 0));
		}
	};

	template<typename T>
	inline Quaternion<T> Quaternion<T>::FromMatrix(const Matrix3<T>& matrix)
	{
		Vector3<T> column0 = matrix.GetColumn(0);
		Vector3<T> column1 = matrix.GetColumn(1);
		Vector3<T> column2 = matrix.GetColumn(2);

		if (column2.Z() < 0)
		{
			if (column0.X() > column1.Y())
			{
				T t = 1 + column0.X() - column1.Y() - column2.Z();

				T x = t;
				T y = column0.Y() + column1.X();
				T z = column2.X() + column0.Z();
				T w = column1.Z() - column2.Y();

				return Quaternion<T>(x, y, z, w) * static_cast<T>(0.5 / std::sqrt(t));
			}
			else
			{
				T t = 1 - column0.X() + column1.Y() - column2.Z();
				
				T x = column0.Y() + column1.X();
				T y = t;
				T z = column1.Z() + column2.Y();
				T w = column2.X() - column0.Z();
				
				return Quaternion<T>(x, y, z, w) * static_cast<T>(0.5 / std::sqrt(t));
			}
		}
		else
		{
			if (column0.X() < -column1.Y())
			{
				T t = 1 - column0.X() - column1.Y() + column2.Z();
				
				T x = column2.X() + column0.Z();
				T y = column1.Z() + column2.Y();
				T z = t;
				T w = column0.Y() - column1.X();

				return Quaternion<T>(x, y, z, w) * static_cast<T>(0.5 / std::sqrt(t));
			}
			else
			{
				T t = 1 + column0.X() + column1.Y() + column2.Z();
		
				T x = column1.Z() - column2.Y();
				T y = column2.X() - column0.Z();
				T z = column0.Y() - column1.X();
				T w = t;

				return Quaternion<T>(x, y, z, w) * static_cast<T>(0.5 / std::sqrt(t));
			}
		}
	}
}

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP
