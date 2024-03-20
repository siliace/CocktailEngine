#ifndef COCKTAIL_CORE_MATH_QUATERNION_HPP
#define COCKTAIL_CORE_MATH_QUATERNION_HPP

#include <cassert>

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
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

		/**
		 * \brief 
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		friend bool operator==(const Quaternion& lhs, const Quaternion& rhs)
		{
			return std::tie(lhs.X, lhs.Y, lhs.Z, lhs.W) == std::tie(rhs.X, rhs.Y, rhs.Z, rhs.W);
		}

		/**
		 * \brief 
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs)
		{
			return !(lhs == rhs);
		}

		T X;
		T Y;
		T Z;
		T W;
	};
}

#endif // COCKTAIL_CORE_MATH_QUATERNION_HPP
