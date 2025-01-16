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

		bool operator==(const Matrix<Row, Column, T>& rhs) const
		{
			return mElements == rhs.mElements;
		}

		bool operator!=(const Matrix<Row, Column, T>& rhs) const
		{
			return !(*this == rhs);
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
}

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP
