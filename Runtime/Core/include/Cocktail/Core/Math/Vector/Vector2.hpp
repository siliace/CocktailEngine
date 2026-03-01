#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP

#include <Cocktail/Core/Extent2D.hpp>
#include <Cocktail/Core/Math/Vector/Vector.hpp>

namespace Ck
{
	/**
	 * \brief
	 * \tparam T
	 */
	template <typename T>
	class Vector2 : public Vector<Vector2, T, 2>
	{
	public:

	    using ElementType = typename Vector<Vector2, T, 2>::ElementType;

		static constexpr std::size_t Size = Vector<Vector2, T, 2>::Size;

		/**
		 * \brief
		 */
		Vector2() = default;

		/**
		 * \brief
		 * \param x
		 * \param y
		 */
		Vector2(T x, T y)
		{
			X() = x;
			Y() = y;
		}

        /**
         * \brief
         *
         * \tparam U
         *
         * \param other
         */
	    template <typename U>
	    Vector2(const Vector2<U>& other)
		{
		    X() = static_cast<T>(other.X());
		    Y() = static_cast<T>(other.Y());
		}

		/**
		 * \brief
		 * \return
		 */
		T& X()
		{
			return Vector<Vector2, T, 2>::At(0);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& X() const
		{
			return Vector<Vector2, T, 2>::At(0);
		}

		/**
		 * \brief
		 * \return
		 */
		T& Y()
		{
			return Vector<Vector2, T, 2>::At(1);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& Y() const
		{
			return Vector<Vector2, T, 2>::At(1);
		}
	};

    template <typename T>
    Vector2<T> operator+(const Vector2<T>& lhs, const Extent2D<T>& rhs)
    {
        Vector2<T> result = lhs;
        result.X() += rhs.Width;
        result.Y() += rhs.Height;

        return result;
    }
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP
