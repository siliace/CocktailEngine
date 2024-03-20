#ifndef COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP
#define COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP

#include <Cocktail/Core/Math/Quaternion.hpp>
#include <Cocktail/Core/Math/Matrix/Detail/Matrix.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Matrix3 : public Detail::SquareMatrix<Matrix3, Vector3, T>
	{
	public:

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
		 * \brief 
		 */
		Matrix3() = default;

		/**
		 * \brief 
		 * \param diagonal 
		 */
		explicit Matrix3(const Vector3<T>& diagonal) :
			Detail::SquareMatrix<Matrix3, Vector3, T>(diagonal)
		{
			/// Nothing
		}
	};
}

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX3_HPP
