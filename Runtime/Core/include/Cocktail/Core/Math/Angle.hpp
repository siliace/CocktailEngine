#ifndef COCKTAIL_CORE_MATH_ANGLE_HPP
#define COCKTAIL_CORE_MATH_ANGLE_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Constants.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Angle
	{
	public:

		/**
		 * \brief 
		 */
		static const Angle Zero;

		/**
		 * \brief 
		 */
		static constexpr T RadianToDegree = static_cast<T>(180.0) / Constants<T>::Pi;

		/**
		 * \brief 
		 */
		static constexpr T DegreeToRadian = Constants<T>::Pi / static_cast<T>(180.0);

		/**
		 * \brief 
		 * \param angle 
		 * \return 
		 */
		static Angle Degree(T angle)
		{
			return Angle(angle);
		}

		/**
		 * \brief 
		 * \param angle 
		 * \return 
		 */
		static Angle Radian(T angle)
		{
			return Angle(angle * RadianToDegree);
		}

		/**
		 * Normalize the value of an Angle
		 * \param value The angle to normalize
		 * \return The normalized angle
		 */
		static Angle Normalize(const Angle<T>& value)
		{
			return Degree(
				std::fmod(value.AsDegree(), static_cast<T>(360.0))
			);
		}

		/**
		 * \brief 
		 */
		constexpr Angle() :
			mAngle(0.0)
		{
			/// Nothing
		}

		Angle Normalized() const
		{
			return Normalize(*this);
		}

		/**
		 * \brief 
		 * \return 
		 */
		T AsDegree() const
		{
			return mAngle;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T AsRadian() const
		{
			return mAngle * DegreeToRadian;
		}

	private:

		explicit Angle(T angle) :
			mAngle(angle)
		{
			/// Nothing
		}

		T mAngle;
	};

	template <typename T>
	const Angle<T> Angle<T>::Zero;

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	constexpr bool operator==(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return NearlyEqual(lhs.AsDegree(), rhs.AsDegree());
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	constexpr bool operator!=(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return !(lhs.AsDegree() == rhs.AsDegree());
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \return 
	 */
	template <typename T>
	constexpr Angle<T> operator-(const Angle<T>& lhs)
	{
		return Angle<T>::Degree(-lhs.AsDegree());
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	constexpr Angle<T> operator+(Angle<T>& lhs, const Angle<T>& rhs)
	{
		return Angle<T>::Radian(lhs.AsRadian() + rhs.AsRadian());
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	constexpr Angle<T>& operator+=(Angle<T>& lhs, const Angle<T>& rhs)
	{
		lhs = Angle<T>::Radian(lhs.AsRadian() + rhs.AsRadian());
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
	constexpr Angle<T> operator-(Angle<T>& lhs, const Angle<T>& rhs)
	{
		return Angle<T>::Radian(lhs.AsRadian() - rhs.AsRadian());
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	constexpr Angle<T>& operator-=(Angle<T>& lhs, const Angle<T>& rhs)
	{
		lhs = Angle<T>::Radian(lhs.AsRadian() - rhs.AsRadian());
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
	constexpr bool operator<(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return lhs.AsDegree() < rhs.AsDegree();
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	constexpr bool operator>(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return lhs.AsDegree() > rhs.AsDegree();
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	constexpr bool operator<=(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return lhs.AsDegree() <= rhs.AsDegree();
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	constexpr bool operator>=(const Angle<T>& lhs, const Angle<T>& rhs)
	{
		return lhs.AsDegree() >= rhs.AsDegree();
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	Angle<T> operator*(const Angle<T>& lhs, T rhs)
	{
		return Angle<T>::Degree(lhs.AsDegree() * rhs);
	}

	/**
	 * \brief 
	 * \tparam T 
	 * \param lhs 
	 * \param rhs 
	 * \return 
	 */
	template <typename T>
	Angle<T> operator*(T lhs, const Angle<T>& rhs)
	{
		return Angle<T>::Degree(lhs * rhs.AsDegree());
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Angle<T> operator/(const Angle<T>& lhs, T rhs)
	{
		return Angle<T>::Degree(lhs.AsDegree() / rhs);
	}

	/**
	 * \brief
	 * \tparam T
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	Angle<T> operator/(T lhs, const Angle<T>& rhs)
	{
		return Angle<T>::Degree(lhs / rhs.AsDegree());
	}
}

namespace std
{
	template <typename T>
	Ck::Angle<T> abs(const Ck::Angle<T>& angle)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			return Ck::Angle<T>::Degree(
				std::fabs(angle.AsDegree())
			);
		}
		else
		{
			return Ck::Angle<T>::Degree(
				std::abs(angle.AsDegree())
			);
		}
	}

	inline float cos(const Ck::Angle<float>& angle)
	{
		return std::cos(angle.AsRadian());
	}

	inline float sin(const Ck::Angle<float>& angle)
	{
		return std::sin(angle.AsRadian());
	}

	inline float tan(const Ck::Angle<float>& angle)
	{
		return std::tan(angle.AsRadian());
	}

	inline float acos(const Ck::Angle<float>& angle)
	{
		return std::acos(angle.AsRadian());
	}

	inline float asin(const Ck::Angle<float>& angle)
	{
		return std::asin(angle.AsRadian());
	}

	inline float atan(const Ck::Angle<float>& angle)
	{
		return std::atan(angle.AsRadian());
	}
}

#endif // COCKTAIL_CORE_MATH_ANGLE_HPP
