#ifndef COCKTAIL_CORE_MATH_PLANE_HPP
#define COCKTAIL_CORE_MATH_PLANE_HPP

#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	/**
	 * \brief
	 * \tparam T
	 */
	template <typename T>
	class Plane
	{
	public:

		/**
		 * \brief
		 */
		Plane() :
			Distance(0)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param distance
		 * \param normal
		 */
		Plane(T distance, const Vector3<T>& normal) :
			Distance(distance),
			Normal(normal)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param point1
		 * \param point2
		 * \param point3
		 */
		Plane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
		{
			Vector3<T> edge1 = point2 - point1;
			Vector3<T> edge2 = point3 - point1;
			Normal = Vector3<T>::CrossProduct(edge1, edge2);
			Normal = Vector3<T>::Normalize(Normal);

			Distance = Vector3<T>::DotProduct(-Normal, point3);
		}

		T Distance;
		Vector3<T> Normal;
	};
}

#endif // COCKTAIL_CORE_MATH_PLANE_HPP
