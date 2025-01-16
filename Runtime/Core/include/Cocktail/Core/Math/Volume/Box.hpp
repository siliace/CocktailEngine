#ifndef COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP
#define COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP

#include <cassert>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Volume/Volume.hpp>

namespace Ck
{
	template <typename T>
	class Box : public Volume<T>
	{
	public:

		/**
		 * \brief 
		 * \param center 
		 * \param size 
		 * \return 
		 */
		static Box<T> WithCenter(const Vector3<T>& center, T size)
		{
			return WithCenter(center, Vector3<T>(size));
		}

		/**
		 * \brief 
		 * \param center 
		 * \param halfExtent 
		 * \return 
		 */
		static Box<T> WithCenter(const Vector3<T>& center, const Vector3<T>& halfExtent)
		{
			Box<T> box;
			box.Position = center - halfExtent;
			box.Extent = halfExtent * static_cast<T>(2);

			return box;
		}

		static Box<T> WithMinMax(const Vector3<T>& min, const Vector3<T> max)
		{
			return Box<T>(min, max - min);
		}

		/**
		 * \brief 
		 * \param extent 
		 */
		explicit Box(const Vector3<T>& extent) :
			Box(Vector3<T>::Zero(), extent)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 */
		Box() = default;

		/**
		 * \brief 
		 * \param position 
		 * \param extent 
		 */
		Box(const Vector3<T>& position, const Vector3<T>& extent) :
			Position(position),
			Extent(extent)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \return
		 */
		bool IsCube() const
		{
			if constexpr (std::is_floating_point_v<T>)
			{
				return NearlyEqual(Extent.X(), Extent.Y()) && NearlyEqual(Extent.X(), Extent.Z());
			}
			else
			{
				return Extent.X() == Extent.Y() && Extent.X() == Extent.Z();
			}
		}

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		bool Contains(const Vector3<T>& vertex) const override
		{
			if (vertex.X() < Position.X() || vertex.X() > Position.X() + Extent.X())
				return false;

			if (vertex.Y() < Position.Y() || vertex.Y() > Position.Y() + Extent.Y())
				return false;

			if (vertex.Z() < Position.Z() || vertex.Z() > Position.Z() + Extent.Z())
				return false;

			return true;
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetVertexCount() const override
		{
			return 8;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Vector3<T> GetVertex(std::size_t index) const override
		{
			assert(index < 8);

			switch (index)
			{
			case 0:
				return { Position.X(), Position.Y(), Position.Z() };

			case 1:
				return { Position.X() + Extent.X(), Position.Y(), Position.Z() };

			case 2:
				return { Position.X() + Extent.X(), Position.Y() + Extent.Y(), Position.Z() };

			case 3:
				return { Position.X(), Position.Y() + Extent.Y(), Position.Z() };

			case 4:
				return { Position.X(), Position.Y(), Position.Z() + Extent.Z() };

			case 5:
				return { Position.X() + Extent.X(), Position.Y(), Position.Z() + Extent.Z() };

			case 6:
				return { Position.X() + Extent.X(), Position.Y() + Extent.Y(), Position.Z() + Extent.Z() };

			case 7:
				return { Position.X(), Position.Y() + Extent.Y(), Position.Z() + Extent.Z() };
			}

			COCKTAIL_UNREACHABLE();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<T> GetCenter() const override
		{
			return (Position + Extent) / static_cast<T>(2);
		}

		Vector3<T> Position;
		Vector3<T> Extent;
	};
}
#endif // COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP
