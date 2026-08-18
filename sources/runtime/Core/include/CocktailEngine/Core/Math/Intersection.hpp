#ifndef COCKTAILENGINE_CORE_MATH_VOLUME_INTERSECTION_HPP
#define COCKTAILENGINE_CORE_MATH_VOLUME_INTERSECTION_HPP

namespace Ck
{
	/**
	 * \brief Results of intersection queries between volumes
	 */
	enum class Intersection
	{
		/**
		 * \brief The queried volume is inside the volume
		 */
		Inside,

		/**
		 * \brief The queried volume is outside of the volume
		 */
		Outside,

		/**
		 * \brief The queried volume intersect the volume
		 */
		Intersect,
	};
}

#endif // COCKTAILENGINE_CORE_MATH_VOLUME_INTERSECTION_HPP
