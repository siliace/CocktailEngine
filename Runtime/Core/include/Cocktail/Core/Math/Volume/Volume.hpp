#ifndef COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP
#define COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP

#include <Cocktail/Core/Extent3D.hpp>
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
		Intersection Intersect(const Volume<T>& volume) const
		{
			bool outside = false, inside = false;

			for (std::size_t i = 0; i < volume.GetVertexCount(); i++)
			{
				bool contains = Contains(volume.GetVertex(i));

				inside |= contains;
				outside |= !contains;

				if (inside && outside)
					return Intersection::Intersect;
			}

			return inside ? Intersection::Inside : Intersection::Outside;
		}

		/**
		 * \brief Tell whether the volume is null
		 * \return True if the volume is null, false otherwise
		 */
		virtual bool IsNull() const = 0;

		/**
		 * \brief Extend the volume to encompass a \p point
		 * \param point The point to encompass
		 * \return True if the volume grown, false otherwise
		 */
		virtual bool Extend(Vector3<T> point) = 0;

		/**
		 * \brief Extent the volume to encompass all points of a \p other volume
		 * \param other The volume to encompass
		 * \return True if the volume grown, false otherwise
		 */
		virtual bool Extend(const Volume<T>& other)
		{
			bool grown = false;
			if (!other.IsNull())
			{
				for (std::size_t i = 0; i < other.GetVertexCount(); i++)
					grown |= Extend(other.GetVertex(i));
			}

			return grown;
		}

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		virtual bool Contains(const Vector3<T>& vertex) const = 0;

		/**
		 * \brief Get the extents of the box
		 * \return Extents of the box
		 */
		Extent3D<T> GetExtents() const
		{
			if (IsNull())
				return MakeExtent<T>(0, 0, 0);

			Vector3<float> min = Vector3<float>::Infinite();
			Vector3<float> max = -Vector3<float>::Infinite();

			for (std::size_t i = 0; i < GetVertexCount(); i++)
			{
				Vector3<float> point = GetVertex(i);
				min = Vector3<float>::Min(min, point);
				max = Vector3<float>::Max(max, point);
			}

			return MakeExtent(
				max.X() - min.X(),
				max.Y() - min.Y(),
				max.Z() - min.Z()
			);
		}


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
