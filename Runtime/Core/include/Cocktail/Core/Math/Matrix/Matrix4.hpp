#ifndef COCKTAIL_CORE_MATH_MATRIX_MATRIX4_HPP
#define COCKTAIL_CORE_MATH_MATRIX_MATRIX4_HPP

#include <Cocktail/Core/Math/Matrix/Detail/Matrix.hpp>
#include <Cocktail/Core/Math/Vector/Vector4.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Matrix4 : public Detail::SquareMatrix<Matrix4, Vector4, T>
	{
	public:

		/**
		 * \brief 
		 */
		Matrix4() = default;

		/**
		 * \brief 
		 * \param diagonal 
		 */
		explicit Matrix4(const Vector4<T>& diagonal) :
			Detail::SquareMatrix<Matrix4, Vector4, T>(diagonal)
		{
			/// Nothing
		}
	};
}

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX4_HPP
