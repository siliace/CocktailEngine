#ifndef COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP
#define COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP

#include <cassert>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Polygon/Polygon.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Rectangle : public Polygon<T>
	{
	public:

		/**
		 * \brief 
		 * \param length 
		 * \return 
		 */
		static Rectangle Square(T length)
		{
			return Rectangle(Vector2<T>(length, length));
		}

		/**
		 * \brief 
		 * \param position 
		 * \param length 
		 * \return 
		 */
		static Rectangle Square(Vector2<T> position, T length)
		{
			return Rectangle(position, Vector2<T>(length, length));
		}

		/**
		 * \brief
		 * \param extent
		 */
		explicit Rectangle(Vector2<T> extent) :
			Rectangle(Vector2<T>::Zero(), extent)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 */
		Rectangle() = default;

		/**
		 * \brief 
		 * \param position 
		 * \param extent 
		 */
		Rectangle(Vector2<T> position, Vector2<T> extent) :
			Position(position),
			Extent(extent)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param x 
		 * \param y 
		 * \param width 
		 * \param height 
		 */
		Rectangle(T x, T y, T width, T height) :
			Rectangle(Vector2(x, y), Vector2(width, height))
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		bool Contains(const Vector2<T>& vertex) const override
		{
			if (vertex.X() < Position.X() || vertex.X() > Position.X() + Extent.X())
				return false;

			if (vertex.Y() < Position.Y() || vertex.Y() > Position.Y() + Extent.Y())
				return false;

			return true;
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetVertexCount() const override
		{
			return 4;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Vector2<T> GetVertex(std::size_t index) const override
		{
			assert(index < 4);

			switch (index)
			{

			case 0:
				return { Position.X(), Position.Y() };

			case 1:
				return { Position.X() + Extent.X(), Position.Y() };

			case 2:
				return { Position.X() + Extent.X(), Position.Y() + Extent.Y() };

			case 3:
				return { Position.X(), Position.Y() + Extent.Y() };
			}

			COCKTAIL_UNREACHABLE();
			return {};
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector2<T> GetCenter() const override
		{
			return (Position + Extent) / static_cast<T>(2);
		}

		Vector2<T> Position;
		Vector2<T> Extent;
	};
}

#endif // COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP
