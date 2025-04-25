#ifndef COCKTAIL_CORE_EXTENT2D_HPP
#define COCKTAIL_CORE_EXTENT2D_HPP

#include <type_traits>

namespace Ck
{
	/**
	 * \brief Utility structure describing the size of 2D objects
	 */
	template <typename T>
	struct Extent2D
	{
		/**
		 * \brief Constructor
		 * Initialize width and height to 0
		 */
		Extent2D() :
			Width(0),
			Height(0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param width The width
		 * \param height The height
		 */
		Extent2D(T width, T height) :
			Width(width),
			Height(height)
		{
			/// Nothong
		}

		T Width;

		T Height;
	};

	/**
	 * \brief 
	 * \param width 
	 * \param height 
	 * \return 
	 */
	template <typename T>
	Extent2D<T> MakeExtent(T width, T height)
	{
		return { width, height };
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	bool operator==(const Extent2D<T>& lhs, const Extent2D<T>& rhs)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return NearlyEqual(lhs.Width, rhs.Width) && NearlyEqual(lhs.Height, rhs.Height);
		}
		else
		{
			return lhs.Width == rhs.Width && lhs.Height == rhs.Height;
		}
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	bool operator!=(const Extent2D<T>& lhs, const Extent2D<T>& rhs)
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
	Extent2D<int> operator+(const Extent2D<T>& lhs, const Extent2D<T>& rhs)
	{
		return MakeExtent(lhs.Width + rhs.Width, lhs.Height + rhs.Height);
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	Extent2D<int> operator-(const Extent2D<T>& lhs, const Extent2D<T>& rhs)
	{
		return MakeExtent(lhs.Width - rhs.Width, lhs.Height - rhs.Height);
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Extent2D<T>& operator*=(Extent2D<T>& lhs, T rhs)
	{
		lhs.Width *= rhs;
		lhs.Height *= rhs;

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
	Extent2D<T> operator*(const Extent2D<T>& lhs, T rhs)
	{
		return Extent2D<T>(lhs) *= rhs;
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Extent2D<T>& operator/=(Extent2D<T>& lhs, T rhs)
	{
		lhs.Width /= rhs;
		lhs.Height /= rhs;

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
	Extent2D<T> operator/(const Extent2D<T>& lhs, T rhs)
	{
		return Extent2D<T>(lhs) /= rhs;
	}
}

#endif // COCKTAIL_CORE_EXTENT2D_HPP
