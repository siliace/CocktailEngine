#ifndef COCKTAIL_CORE_MATH_MATRIX_SQUAREMATRIX_HPP
#define COCKTAIL_CORE_MATH_MATRIX_SQUAREMATRIX_HPP

#include <Cocktail/Core/Math/Matrix/Matrix.hpp>

namespace Ck
{
	template <template<class> class BaseMatrix, template <class> class Line, typename T>
	class SquareMatrix : public Matrix<Line, Line, T>
	{
	public:

		/**
		 * \brief
		 */
		using LineType = Line<T>;

		/**
		 * \brief
		 */
		using MatrixType = BaseMatrix<T>;

		/**
		 * \brief
		 */
		static constexpr std::size_t Size = LineType::Size;

		/**
		 * \brief
		 * \return
		 */
		static MatrixType Identity()
		{
			return MatrixType(
				LineType(
					static_cast<T>(1)
				)
			);
		}

		/**
		 * \brief
		 */
		SquareMatrix() = default;

		/**
		 * \brief
		 * \param value
		 */
		explicit SquareMatrix(T value) :
			Matrix<Line, Line, T>(value)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param diagonal
		 */
		explicit SquareMatrix(const LineType& diagonal)
		{
			SetDiagonal(diagonal);
		}

		/**
		 * \brief
		 * \return
		 */
		MatrixType Transpose() const
		{
			MatrixType transposed;
			for (std::size_t i = 0; i < MatrixType::Size; i++)
			{
				for (std::size_t j = 0; j < MatrixType::Size; j++)
					transposed.At(i, j) = this->At(j, i);
			}

			return transposed;
		}

		/**
		 * \brief
		 * \return
		 */
		LineType GetDiagonal() const
		{
			LineType diagonal;
			for (std::size_t i = 0; i < diagonal.GetSize(); i++)
				diagonal.At(i) = this->At(i, i);

			return diagonal;
		}

		/**
		 * \brief
		 * \param diagonal
		 */
		void SetDiagonal(const LineType& diagonal)
		{
			for (std::size_t i = 0; i < diagonal.GetSize(); i++)
				this->At(i, i) = diagonal.At(i);
		}
	};

	template <template<class> class BaseMatrix, template <class> class Line, typename T>
	Line<T> operator*(const SquareMatrix<BaseMatrix, Line, T>& left, const Line<T>& right)
	{
		Line<T> product;
		for (std::size_t i = 0; i < SquareMatrix<BaseMatrix, Line, T>::Size; i++)
			product.At(i) = Line<T>::DotProduct(right, left.GetColumn(i));

		return product;
	}

	template <template<class> class BaseMatrix, template <class> class Line, typename T>
	BaseMatrix<T> operator*(const SquareMatrix<BaseMatrix, Line, T>& left, const SquareMatrix<BaseMatrix, Line, T>& right)
	{
		BaseMatrix<T> product;
		for (std::size_t x = 0; x < SquareMatrix<BaseMatrix, Line, T>::Size; x++)
		{
			Line<T> row = left.GetRow(x);
			for (std::size_t y = 0; y < SquareMatrix<BaseMatrix, Line, T>::Size; y++)
			{
				T sum = 0;

				Line<T> column = right.GetColumn(y);
				for (std::size_t i = 0; i < SquareMatrix<BaseMatrix, Line, T>::Size; i++)
					sum += row.At(i) * column.At(i);

				product.At(x, y) = sum;
			}
		}

		return product;
	}
}

#endif // COCKTAIL_CORE_MATH_MATRIX_SQUAREMATRIX_HPP
