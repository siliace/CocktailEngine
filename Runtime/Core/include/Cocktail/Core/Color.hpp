#ifndef COCKTAIL_CORE_COLOR_HPP
#define COCKTAIL_CORE_COLOR_HPP

#include <array>

namespace Ck
{
	/**
     * \brief 
     */
    template <typename T, std::size_t N>
    class Color
    {
    public:

	    /**
         * \brief 
         */
        Color() :
			Color(0)
        {
	        /// Nothing
        }

        /**
         * \brief 
         * \param value 
         */
        explicit Color(T value)
        {
            std::fill(mValues.begin(), mValues.end(), value);
        }

	    /**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		T& At(std::size_t index)
		{
            return mValues.at(index);
		}

        /**
         * \brief
         * \param index
         * \return
         */
        const T& At(std::size_t index) const
        {
            return mValues.at(index);
        }

	    /**
	     * \brief 
	     * \param lhs 
	     * \param rhs 
	     * \return 
	     */
	    friend bool operator==(const Color& lhs, const Color& rhs)
	    {
		    return lhs.mValues == rhs.mValues;
	    }

	    /**
	     * \brief 
	     * \param lhs 
	     * \param rhs 
	     * \return 
	     */
	    friend bool operator!=(const Color& lhs, const Color& rhs)
	    {
		    return !(lhs == rhs);
	    }

		/**
		 * \brief
		 */
		explicit operator T*()
		{
			return mValues.data();
		}

	    /**
		 * \brief 
		 */
		explicit operator const T*() const
        {
			return mValues.data();
        }

    private:

        std::array<T, N> mValues;
    };

    /**
     * \brief 
     * \tparam T 
     */
    template <typename T>
    class Color<T, 3>
    {
    public:

		static const Color<T, 3> Black;
		static const Color<T, 3> White;

	    /**
         * \brief 
         */
        Color() :
			Color(0)
        {
	        /// Nothing
        }

        /**
         * \brief 
         * \param value 
         */
        explicit Color(T value) :
			Red(value),
			Green(value),
			Blue(value)
        {
	        /// Nothing
        }

		Color(T red, T green, T blue) :
			Red(red),
			Green(green),
			Blue(blue)
		{
			/// Nothing
		}

	    /**
	     * \brief 
	     * \param lhs 
	     * \param rhs 
	     * \return 
	     */
	    friend bool operator==(const Color& lhs, const Color& rhs)
	    {
		    return std::tie(lhs.Red, lhs.Green, lhs.Blue) == std::tie(rhs.Red, rhs.Green, rhs.Blue);
	    }

	    /**
	     * \brief 
	     * \param lhs 
	     * \param rhs 
	     * \return 
	     */
	    friend bool operator!=(const Color& lhs, const Color& rhs)
	    {
		    return !(lhs == rhs);
	    }

	    /**
         * \brief 
         */
        T Red;

	    /**
         * \brief 
         */
        T Green;

	    /**
         * \brief 
         */
        T Blue;
    };

    /**
     * \brief 
     * \tparam T 
     */
    template <typename T>
    class Color<T, 4>
    {
    public:

		static const Color<T, 4> Black;
		static const Color<T, 4> White;

		/**
		* \brief
		*/
		Color() :
			Color(0)
		{
			/// Nothing
		}

		/**
		* \brief
		* \param value
		*/
		explicit Color(T value) :
			Red(value),
			Green(value),
			Blue(value),
			Alpha(value)
		{
			/// Nothing
		}

		Color(T red, T green, T blue, T alpha) :
			Red(red),
			Green(green),
			Blue(blue),
    		Alpha(alpha)
		{
			/// Nothing
		}

		friend bool operator==(const Color& lhs, const Color& rhs)
		{
			return std::tie(lhs.Red, lhs.Green, lhs.Blue, lhs.Alpha) == std::tie(rhs.Red, rhs.Green, rhs.Blue, rhs.Alpha);
		}

		friend bool operator!=(const Color& lhs, const Color& rhs)
		{
			return !(lhs == rhs);
		}

		T Red;

        T Green;

        T Blue;

        T Alpha;
    };

    template <typename T>
    using RgbColor = Color<T, 3>;

	template <typename T>
	const Color<T, 3> Color<T, 3>::Black(0, 0, 0);

	template <typename T>
	const Color<T, 3> Color<T, 3>::White(255, 255, 255);

    template <typename T>
    using RgbaColor = Color<T, 4>;

	template <typename T>
	const Color<T, 4> Color<T, 4>::Black(0, 0, 0, 255);

	template <typename T>
	const Color<T, 4> Color<T, 4>::White(255, 255, 255, 255);
}
#endif // COCKTAIL_CORE_COLOR_HPP
