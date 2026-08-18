#ifndef COCKTAILENGINE_CORE_MATH_MATRIX_MATRIX4_HPP
#define COCKTAILENGINE_CORE_MATH_MATRIX_MATRIX4_HPP

#include <CocktailEngine/Core/Math/Matrix/SquareMatrix.hpp>
#include <CocktailEngine/Core/Math/Vector/Vector4.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Matrix4 : public SquareMatrix<Matrix4, Vector4, T>
	{
	public:

		/**
		 * \brief 
		 */
		Matrix4() = default;

		/**
		 * \brief 
		 * \param value 
		 */
		explicit Matrix4(T value) :
			SquareMatrix<Matrix4, Vector4, T>(value)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param diagonal 
		 */
		explicit Matrix4(const Vector4<T>& diagonal) :
			SquareMatrix<Matrix4, Vector4, T>(diagonal)
		{
			/// Nothing
		}
	};
}

#endif // COCKTAILENGINE_CORE_MATH_MATRIX_MATRIX4_HPP
