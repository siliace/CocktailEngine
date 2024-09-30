#ifndef COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP
#define COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP

#include <array>

namespace Ck
{
	/**
	 * \brief Matrix class with custom width and height
	 * Matrices are always represented in row major ordering
	 * \tparam Row 
	 * \tparam Column 
	 * \tparam T 
	 */
	template <template <class> class Row, template <class> class Column, typename T>
	class Matrix
	{
	public:

		using RowType = Row<T>;
		using ColumnType = Column<T>;

		static constexpr std::size_t Width = RowType::Size;
		static constexpr std::size_t Height = ColumnType::Size;

		/**
		 * \brief Constructor
		 * Fill every element of the matrix with 0
		 */
		Matrix() :
			Matrix(0.0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * Fill every element of the matrix with \p value
		 * \param value 
		 */
		explicit Matrix(T value)
		{
			std::fill(mElements.begin(), mElements.end(), value);
		}

		/**
		 * \brief Access to an element of the matrix
		 * \param x The abscissa of the element
		 * \param y The ordinate of the element
		 * \return The element
		 */
		T& At(std::size_t x, std::size_t y)
		{
			return mElements.at(y + x * Width);
		}

		/**
		 * \brief Access to an element of the matrix
		 * \param x The abscissa of the element
		 * \param y The ordinate of the element
		 * \return The element
		 */
		const T& At(std::size_t x, std::size_t y) const
		{
			return mElements.at(y + x * Width);
		}

		/**
		 * \brief 
		 * \param rowIndex 
		 * \return 
		 */
		RowType GetRow(std::size_t rowIndex) const
		{
			RowType row;
			for (std::size_t i = 0; i < Width; i++)
				row.At(i) = At(rowIndex, i);

			return row;
		}

		/**
		 * \brief 
		 * \param rowIndex 
		 * \param row 
		 */
		void SetRow(std::size_t rowIndex, const RowType& row)
		{
			for (std::size_t i = 0; i < Width; i++)
				At(rowIndex, i) = row.At(i);
		}

		/**
		 * \brief
		 * \param columnIndex
		 * \return
		 */
		ColumnType GetColumn(std::size_t columnIndex) const
		{
			ColumnType column;
			for (std::size_t i = 0; i < Height; i++)
				column.At(i) = At(i, columnIndex);

			return column;
		}

		/**
		 * \brief
		 * \param columnIndex
		 * \param row
		 */
		void SetColumn(std::size_t columnIndex, const ColumnType& row)
		{
			for (std::size_t i = 0; i < Height; i++)
				At(i, columnIndex) = row.At(i);
		}

		/**
		 * \brief 
		 */
		explicit operator const T*() const
		{
			return mElements.data();
		}

	private:

		std::array<T, Width * Height> mElements;
	};

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

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP
