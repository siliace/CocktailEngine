#ifndef COCKTAIL_CORE_MATH_POLYGON_POLYGON_HPP
#define COCKTAIL_CORE_MATH_POLYGON_POLYGON_HPP

#include <utility>

#include <Cocktail/Core/Math/Intersection.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>

namespace Ck
{
	template <typename T>
	class Polygon
	{
	public:

		/**
		 * \brief
		 */
		virtual ~Polygon() = default;

		/**
		 * \brief
		 * \param polygon
		 * \return
		 */
		virtual Intersection Intersect(const Polygon<T>& polygon) const
		{
			bool outside = true, inside = false;

			for (std::size_t i = 0; i < polygon.GetVertexCount(); i++)
			{
				bool contains = Contains(polygon.GetVertex(i));

				inside &= contains == true;
				outside |= contains == false;
			}

			if (outside)
				return Intersection::Outside;

			return inside ? Intersection::Inside : Intersection::Intersect;
		}

		/**
		 * \brief
		 * \param vertex
		 * \return
		 */
		virtual bool Contains(const Vector2<T>& vertex) const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual std::size_t GetVertexCount() const = 0;

		/**
		 * \brief
		 * \param index
		 * \return
		 */
		virtual Vector2<T> GetVertex(std::size_t index) const = 0;

		/**
		 * \brief
		 * \return
		 */
		virtual Vector2<T> GetCenter() const = 0;
	};
}

#endif // COCKTAIL_CORE_MATH_POLYGON_POLYGON_HPP
