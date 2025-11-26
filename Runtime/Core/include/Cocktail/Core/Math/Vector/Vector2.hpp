#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP

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
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR2_HPP
