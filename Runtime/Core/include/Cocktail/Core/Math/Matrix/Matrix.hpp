#ifndef COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP
#define COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP

#include <Cocktail/Core/StaticArray.hpp>

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

		static constexpr unsigned int Width = RowType::Size;
		static constexpr unsigned int Height = ColumnType::Size;

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
		    mElements.Fill(value);
		}

		/**
		 * \brief Access to an element of the matrix
		 * \param x The abscissa of the element
		 * \param y The ordinate of the element
		 * \return The element
		 */
		T& At(unsigned int x, unsigned int y)
		{
			return mElements.At(y + x * Width);
		}

		/**
		 * \brief Access to an element of the matrix
		 * \param x The abscissa of the element
		 * \param y The ordinate of the element
		 * \return The element
		 */
		const T& At(unsigned int x, unsigned int y) const
		{
			return mElements.At(y + x * Width);
		}

		/**
		 * \brief 
		 * \param rowIndex 
		 * \return 
		 */
		RowType GetRow(unsigned int rowIndex) const
		{
			RowType row;
			for (unsigned int i = 0; i < Width; i++)
				row.At(i) = At(rowIndex, i);

			return row;
		}

		/**
		 * \brief 
		 * \param rowIndex 
		 * \param row 
		 */
		void SetRow(unsigned int rowIndex, const RowType& row)
		{
			for (unsigned int i = 0; i < Width; i++)
				At(rowIndex, i) = row.At(i);
		}

		/**
		 * \brief
		 * \param columnIndex
		 * \return
		 */
		ColumnType GetColumn(unsigned int columnIndex) const
		{
			ColumnType column;
			for (unsigned int i = 0; i < Height; i++)
				column.At(i) = At(i, columnIndex);

			return column;
		}

		/**
		 * \brief
		 * \param columnIndex
		 * \param row
		 */
		void SetColumn(unsigned int columnIndex, const ColumnType& row)
		{
			for (unsigned int i = 0; i < Height; i++)
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

		StaticArray<T, Width * Height> mElements;
	};
}

#endif // COCKTAIL_CORE_MATH_MATRIX_MATRIX_HPP
