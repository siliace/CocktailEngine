#ifndef COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP
#define COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP

#include <Cocktail/Core/Math/Intersection.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Volume
	{
	public:

		/**
		 * \brief 
		 */
		virtual ~Volume() = default;

		/**
		 * \brief 
		 * \param volume
		 * \return 
		 */
		virtual Intersection Intersect(const Volume<T>& volume) const
		{
			bool outside = true, inside = false;

			for (std::size_t i = 0; i < volume.GetVertexCount(); i++)
			{
				bool contains = Contains(volume.GetVertex(i));

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
		virtual bool Contains(const Vector3<T>& vertex) const = 0;

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
		virtual Vector3<T> GetVertex(std::size_t index) const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Vector3<T> GetCenter() const = 0;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP
