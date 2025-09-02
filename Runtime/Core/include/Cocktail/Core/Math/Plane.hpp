#ifndef COCKTAIL_CORE_MATH_PLANE_HPP
#define COCKTAIL_CORE_MATH_PLANE_HPP

#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Utility/Optional.hpp>

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
		 * \brief Compute the intersection point between 3 planes
		 * \param p1 
		 * \param p2 
		 * \param p3 
		 * \return 
		 */
		static Optional<Vector3<T>> Intersect(const Plane<T>& p1, const Plane<T>& p2, const Plane<T>& p3)
		{
			Vector3<T> n1 = p1.GetNormal();
			Vector3<T> n2 = p2.GetNormal();
			Vector3<T> n3 = p3.GetNormal();

			T denominator = Vector3<T>::DotProduct(n1, Vector3<T>::CrossProduct(n2, n3));
			if (NearlyEqual(std::abs(denominator), T(0)))
				return Optional<Vector3<T>>::Empty();

			Vector3<T> point;
			point += -p1.GetDistance() * Vector3<T>::CrossProduct(n2, n3);
			point += -p2.GetDistance() * Vector3<T>::CrossProduct(n3, n1);
			point += -p3.GetDistance() * Vector3<T>::CrossProduct(n1, n2);
			point /= denominator;

			return Optional<Vector3<T>>::Of(point);
		}

		/**
		 * \brief Default constructor
		 * Create a empty plane
		 */
		Plane() :
			mDistance(0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param distance The distance to the origin of the plane
		 * \param normal The normal of the plane
		 */
		Plane(T distance, const Vector3<T>& normal) :
			mDistance(distance),
			mNormal(Vector3<T>::Normalize(normal))
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * Create a Plane by computing the distance between a \p point and the origin
		 * \param point The reference point on the plane
		 * \param normal The normal of the plane
		 */
		Plane(const Vector3<T>& point, const Vector3<T>& normal) :
			mNormal(Vector3<T>::Normalize(normal))
		{
			mDistance = -Vector3<T>::DotProduct(mNormal, point);
		}

		/**
		 * \brief Constructor
		 * Create a plane from three points
		 * \param point1 The first point
		 * \param point2 The second point
		 * \param point3 The third point
		 */
		Plane(const Vector3<T>& point1, const Vector3<T>& point2, const Vector3<T>& point3)
		{
			Vector3<T> edge1 = point2 - point1;
			Vector3<T> edge2 = point3 - point1;
			mNormal = Vector3<T>::CrossProduct(edge1, edge2);
			mNormal = Vector3<T>::Normalize(mNormal);

			mDistance = -Vector3<T>::DotProduct(mNormal, point3);
		}

		/**
		 * \brief Compute the distance between a \p point and the plane
		 * If this distance is positive, the \p point is in front of the plane.
		 * If this distance is negative, the \p point is behind the plane.
		 * If this distance is zero, the \p point is on the plane
		 * \param point The point to test
		 * \return The distance between the \p point and the plane
		 */
		T DistanceTo(const Vector3<T>& point) const
		{
			return Vector3<T>::DotProduct(mNormal, point) + mDistance;
		}
		
		/**
		 * \brief Get the distance to the origin of the plane
		 * This distance can be negative when the origin is in the opposition direction of the normal
		 * \return The distance
		 */
		T GetDistance() const
		{
			return mDistance;
		}

		/**
		 * \brief Get the normal vector of the plane
		 * This vector is always normalized
		 * \return The normal
		 */
		Vector3<T> GetNormal() const
		{
			return mNormal;
		}

	private:

		T mDistance;
		Vector3<T> mNormal;
	};
}

#endif // COCKTAIL_CORE_MATH_PLANE_HPP
