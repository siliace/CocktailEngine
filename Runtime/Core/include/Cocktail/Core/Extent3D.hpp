#ifndef COCKTAIL_CORE_EXTENT3D_HPP
#define COCKTAIL_CORE_EXTENT3D_HPP

#include <Cocktail/Core/Extent2D.hpp>

namespace Ck
{
	/**
	 * \brief Utility structure describing the size of 3D objects
	 * \tparam T
	 */
	template <typename T>
	struct Extent3D
	{
		/**
		 * \brief Constructor
		 * Initialize width, height and depth to 0
		 */
		Extent3D() :
			Width(0),
			Height(0),
			Depth(0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param width The width
		 * \param height The height
		 * \param depth The depth
		 */
		Extent3D(T width, T height, T depth) :
			Width(width),
			Height(height),
			Depth(depth)
		{
			/// Nothong
		}

		T Width;

		T Height;

		T Depth;
	};

	/**
	 * \brief
	 * \param width
	 * \param height
	 * \param depth
	 * \return
	 */
	template <typename T>
	Extent3D<T> MakeExtent(T width, T height, T depth)
	{
		return { width, height, depth };
	}

	/**
	 * \brief
	 * \param widthHeight
	 * \param depth
	 * \return
	 */
	template <typename T>
	Extent3D<T> MakeExtent(const Extent2D<T>& widthHeight, T depth)
	{
		return { widthHeight.Width, widthHeight.Height, depth };
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	bool operator==(const Extent3D<T>& lhs, const Extent3D<T>& rhs)
	{
		return lhs.Width == rhs.Width && lhs.Height != rhs.Height;
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	bool operator!=(const Extent3D<T>& lhs, const Extent3D<T>& rhs)
	{
		return !(lhs == rhs);
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Extent3D<T>& operator*=(Extent3D<T>& lhs, T rhs)
	{
		lhs.Width *= rhs;
		lhs.Height *= rhs;
		lhs.Depth *= rhs;

		return lhs;
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Extent3D<T> operator*(const Extent3D<T>& lhs, T rhs)
	{
		return Extent3D<T>(lhs) *= rhs;
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Extent3D<T>& operator/=(Extent3D<T>& lhs, T rhs)
	{
		lhs.Width /= rhs;
		lhs.Height /= rhs;
		lhs.Depth /= rhs;

		return lhs;
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	Extent3D<T> operator/(const Extent3D<T>& lhs, T rhs)
	{
		return Extent3D<T>(lhs) /= rhs;
	}
}

#endif // COCKTAIL_CORE_EXTENT3D_HPP
