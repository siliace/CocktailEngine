#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Vector3 : public Vector<Vector3, T, 3>
	{
	public:

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Right()
		{
			return { 1, 0, 0 };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Left()
		{
			return { -1, 0, 0 };
		}

		/**
		 * \brief 
		 * \return 
		 */
		static Vector3 Up()
		{
			return { 0, 1, 0 };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Down()
		{
			return { 0, -1, 0 };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Backward()
		{
			return { 0, 0, 1 };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Forward()
		{
			return { 0, 0, -1 };
		}

		/**
		 * \brief Calculates the cross (scalar) product with two vectors
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		static Vector3<T> CrossProduct(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			return Vector3<T>(
				lhs.Y() * rhs.Z() - lhs.Z() * rhs.Y(),
				lhs.Z() * rhs.X() - lhs.X() * rhs.Z(),
				lhs.X() * rhs.Y() - lhs.Y() * rhs.X()
			);
		}

		/**
		 * \brief 
		 * \param lhs 
		 * \param rhs 
		 * \return 
		 */
		static Angle<T> AngleBetween(const Vector3<T>& lhs, const Vector3<T>& rhs)
		{
			T dot = Vector3<T>::DotProduct(lhs, rhs);
			Vector3<T> cross = Vector3<T>::CrossProduct(lhs, rhs);
			T angle = std::atan2(cross.GetLength(), dot);

			return Angle<T>::Radian(angle);
		}

		/**
		 * \brief 
		 */
		Vector3() = default;

		/**
		 * \brief 
		 * \param value 
		 */
		explicit Vector3(T value) :
			Vector<Vector3, T, 3>(value)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param xy 
		 * \param z 
		 */
		Vector3(const Vector2<T>& xy, T z) :
			Vector3(xy.X(), xy.Y(), z)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param x 
		 * \param y 
		 * \param z 
		 */
		Vector3(T x, T y, T z)
		{
			X() = x;
			Y() = y;
			Z() = z;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& X()
		{
			return Vector<Vector3, T, 3>::At(0);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& X() const
		{
			return Vector<Vector3, T, 3>::At(0);
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& Y()
		{
			return Vector<Vector3, T, 3>::At(1);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& Y() const
		{
			return Vector<Vector3, T, 3>::At(1);
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& Z()
		{
			return Vector<Vector3, T, 3>::At(2);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& Z() const
		{
			return Vector<Vector3, T, 3>::At(2);
		}
	};
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP
