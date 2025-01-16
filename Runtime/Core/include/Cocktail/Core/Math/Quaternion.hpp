#ifndef COCKTAIL_CORE_MATH_QUATERNION_HPP
#define COCKTAIL_CORE_MATH_QUATERNION_HPP

#include <cassert>

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Math/EulerAngles.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	template <typename>
	class Matrix3;

	/**
	 * \brief Class representing a Quaternion
	 * \tparam T 
	 */
	template <typename T>
	class Quaternion
	{
	public:

		/**
		 * \brief Identity helper
		 * Create a quaternion representing an identity rotation
		 * \return The identity quaternion
		 */
		static Quaternion Identity()
		{
			return Quaternion(0, 0, 0, 1);
		}

		/**
		 * \brief 
		 * \param angle 
		 * \param axis 
		 * \return 
		 */
		static Quaternion FromAngleAxis(const Angle<T>& angle, const Vector3<T>& axis)
		{
			assert(NearlyEqual(axis.GetLength(), static_cast<T>(1)));

			const T a = angle.AsRadian() * static_cast<T>(0.5);
			const T s = std::sin(a);

			Quaternion quaternion;
			quaternion.W = std::cos(a);
			quaternion.X = axis.X() * s;
			quaternion.Y = axis.Y() * s;
			quaternion.Z = axis.Z() * s;

			return quaternion;
		}

		/**
		 * \brief
		 * \param eulerAngles
		 * \return
		 */
		static Quaternion FromEulerAngles(const EulerAngles<T>& eulerAngles)
		{
			T c1 = std::cos(eulerAngles.Yaw / 2.f);
			T c2 = std::cos(eulerAngles.Roll / 2.f);
			T c3 = std::cos(eulerAngles.Pitch / 2.f);

			T s1 = std::sin(eulerAngles.Yaw / 2.f);
			T s2 = std::sin(eulerAngles.Roll / 2.f);
			T s3 = std::sin(eulerAngles.Pitch / 2.f);

			T x = c1 * c2 * c3 - s1 * s2 * s3;
			T y = s1 * s2 * c3 + c1 * c2 * s3;
			T z = s1 * c2 * c3 + c1 * s2 * s3;
			T w = c1 * s2 * c3 - s1 * c2 * s3;
			return Quaternion::Identity(x, y, z, w);
		}

		/**
		 * \brief
		 * \param matrix
		 * \return
		 */
		static Quaternion<T> FromMatrix(const Matrix3<T>& matrix);

		/**
		 * \brief Default constructor
		 */
		Quaternion() :
			Quaternion(0, 0, 0, 0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param x
		 * \param y 
		 * \param z 
		 * \param w 
		 */
		Quaternion(T x, T y, T z, T w) :
			X(x),
			Y(y),
			Z(z),
			W(w)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param source 
		 * \return 
		 */
		Quaternion Compose(const Quaternion& source) const
		{
			return Quaternion(
				W * source.X + source.W * X + Y * source.Z - source.Y * Z, 
				W * source.Y + source.W * Y + Z * source.X - source.Z * X,
				W * source.Z + source.W * Z + X * source.Y - source.X * Y, 
				W * source.W - X * source.X - Y * source.Y - Z * source.Z
			);
		}

		/**
		 * \brief
		 * \param vector
		 * \return
		 */
		Vector3<T> Rotate(const Vector3<T>& vector) const
		{
			const T vx = static_cast<T>(2.0) * vector.X();
			const T vy = static_cast<T>(2.0) * vector.Y();
			const T vz = static_cast<T>(2.0) * vector.Z();
			const T w2 = W * W - static_cast<T>(0.5);

			const T dot2 = X * vx + Y * vy + Z * vz;

			return Vector3<T>(
				vx * w2 + (Y * vz - Z * vy) * W + X * dot2,
				vy * w2 + (Z * vx - X * vz) * W + Y * dot2,
				vz * w2 + (X * vy - Y * vx) * W + Z * dot2
			);
		}

		/**
		 * \brief
		 * \param vector
		 * \return
		 */
		Vector3<T> RotateInverse(const Vector3<T>& vector) const
		{
			const T vx = static_cast<T>(2.0) * vector.X();
			const T vy = static_cast<T>(2.0) * vector.Y();
			const T vz = static_cast<T>(2.0) * vector.Z();
			const T w2 = W * W - static_cast<T>(0.5);

			const T dot2 = X * vx + Y * vy + Z * vz;

			return Vector3<T>(
				vx * w2 - (Y * vz - Z * vy) * W + X * dot2, 
				vy * w2 - (Z * vx - X * vz) * W + Y * dot2,
				vz * w2 - (X * vy - Y * vx) * W + Z * dot2
			);
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		T Dot(const Quaternion& other) const
		{
			return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T GetMagnitude() const
		{
			return std::sqrt(X * X + Y * Y + Z * Z + W * W);
		}

		/**
		 * \brief 
		 * \return 
		 */
		Quaternion GetNormalized() const
		{
			const T scale = 1.0 / GetMagnitude();
			return Quaternion<T>(X * scale, Y * scale, Z * scale, W * scale);
		}

		/**
		 * \brief 
		 * \return 
		 */
		Quaternion GetConjugate() const
		{
			return { -X, -Y, -Z, W };
		}

		T X;
		T Y;
		T Z;
		T W;
	};

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return std::tie(lhs.X, lhs.Y, lhs.Z, lhs.W) == std::tie(rhs.X, rhs.Y, rhs.Z, rhs.W);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename T>
	Quaternion<T> operator*(const Quaternion<T>& lhs, T r)
	{
		return Quaternion<T>(lhs.X * r, lhs.Y * r, lhs.Z * r, lhs.W * r);
	}

	template <typename T>
	Quaternion<T>& operator*=(Quaternion<T>& lhs, T r)
	{
		lhs.X *= r;
		lhs.Y *= r;
		lhs.Z *= r;
		lhs.W *= r;

		return lhs;
	}

	template <typename T>
	EulerAngles<T> EulerAngles<T>::FromQuaternion(const Quaternion<T>& quaternion)
	{
		Angle<T> yaw, pitch, roll;
		T test = quaternion.X * quaternion.Y + quaternion.Z * quaternion.W;

		if (test > T(0.499))
		{
			yaw = Angle<T>::Degree(0);
			pitch = Angle<T>::Radian(T(2.0) * std::atan2(quaternion.X, quaternion.W));
			roll = Angle<T>::Degree(90);
		}
		else if (test < -T(0.499))
		{
			yaw = Angle<T>::Degree(0);
			pitch = Angle<T>::Radian(T(-2.0) * std::atan2(quaternion.X, quaternion.W));
			roll = Angle<T>::Degree(-90);
		}
		else
		{
			yaw = Angle<T>::Radian(std::atan2(T(2.0) * quaternion.X * quaternion.W - T(2.0) * quaternion.Y * quaternion.Z, T(1.0) - T(2.0) * quaternion.X * quaternion.X - T(2.0) * quaternion.Z * quaternion.Z));
			pitch = Angle<T>::Radian(std::atan2(T(2.0) * quaternion.Y * quaternion.W - T(2.0) * quaternion.X * quaternion.Z, T(1.0) - T(2.0) * quaternion.Y * quaternion.Y - T(2.0) * quaternion.Z * quaternion.Z));
			roll = Angle<T>::Radian(std::asin(T(2.0) * test));
		}

		return EulerAngles<T>(yaw, pitch, roll);
	}
}

#endif // COCKTAIL_CORE_MATH_QUATERNION_HPP
