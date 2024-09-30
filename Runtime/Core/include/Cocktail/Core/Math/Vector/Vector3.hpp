#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP

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
			return { 1.f, 0.f, 0.f };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Left()
		{
			return { -1.f, 0.f, 0.f };
		}

		/**
		 * \brief 
		 * \return 
		 */
		static Vector3 Up()
		{
			return { 0.f, 1.f, 0.f };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Down()
		{
			return { 0.f, -1.f, 0.f };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Backward()
		{
			return { 0.f, 0.f, 1.f };
		}

		/**
		 * \brief
		 * \return
		 */
		static Vector3 Forward()
		{
			return { 0.f, 0.f, -1.f };
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
			return At(0);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& X() const
		{
			return At(0);
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& Y()
		{
			return At(1);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& Y() const
		{
			return At(1);
		}

		/**
		 * \brief 
		 * \return 
		 */
		T& Z()
		{
			return At(2);
		}

		/**
		 * \brief 
		 * \return 
		 */
		const T& Z() const
		{
			return At(2);
		}
	};
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR3_HPP
