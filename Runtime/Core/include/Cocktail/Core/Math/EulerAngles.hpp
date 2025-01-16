#ifndef COCKTAIL_CORE_MATH_EULERANGLES_HPP
#define COCKTAIL_CORE_MATH_EULERANGLES_HPP

#include <tuple>

#include <Cocktail/Core/Math/Angle.hpp>

namespace Ck
{
	template <typename T>
	class Quaternion;

	template <typename T>
	class EulerAngles
	{
	public:

		/**
		 * \brief Create an EulerAngles instance from angles in degree
		 * \param yaw The yaw angle in degree
		 * \param pitch The pitch angle in degree
		 * \param roll The roll angle in degree
		 * \return The created EulerAngles instance
		 */
		static EulerAngles<T> Degree(T yaw, T pitch, T roll)
		{
			return EulerAngles<T>(
				Angle<T>::Degree(yaw),
				Angle<T>::Degree(pitch),
				Angle<T>::Degree(roll)
			);
		}

		/**
		 * \brief Create an EulerAngles instance from angles in radian
		 * \param yaw The yaw angle in radian
		 * \param pitch The pitch angle in radian
		 * \param roll The roll angle in radian
		 * \return The created EulerAngles instance
		 */
		static EulerAngles<T> Radian(T yaw, T pitch, T roll)
		{
			return EulerAngles<T>(
				Angle<T>::Radian(yaw),
				Angle<T>::Radian(pitch),
				Angle<T>::Radian(roll)
			);
		}

		/**
		 * \brief Create an EulerAngles instance from Quaternion
		 * \param quaternion The quaternion to convert
		 * \return The created EulerAngles instance
		 */
		static EulerAngles<T> FromQuaternion(const Quaternion<T>& quaternion);

		/**
		 * \brief Default constructor
		 * Initialize all angles to zero
		 */
		EulerAngles() = default;

		/**
		 * \brief Constructor
		 * Create a new instance of EulerAngles
		 * \param yaw
		 * \param pitch
		 * \param roll
		 */
		EulerAngles(Angle<T> yaw, Angle<T> pitch, Angle<T> roll) :
			Yaw(yaw),
			Pitch(pitch),
			Roll(roll)
		{
			/// Nothing
		}

		Angle<T> Yaw;
		Angle<T> Pitch;
		Angle<T> Roll;
	};

	/**
	 * \brief Equality operator
	 * Implement equality test operator
	 * \param rhs The EulerAngles to compare to this
	 * \return True if this and \p rhs are equal, false otherwise
	 */
	template <typename T>
	bool operator==(const EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return std::tie(lhs.Yaw, lhs.Pitch, lhs.Roll) == 
			std::tie(rhs.Yaw, rhs.Pitch, rhs.Roll);
	}

	/**
	 * \brief Equality operator
	 * Implement inequality test operator
	 * \param rhs The EulerAngles to compare to this
	 * \return True if this and \p rhs are not equal, false otherwise
	 */
	template <typename T>
	bool operator!=(const EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return !(lhs == rhs);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator+(const EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return EulerAngles<T>(
			lhs.Yaw + rhs.Yaw, lhs.Pitch + rhs.Pitch, lhs.Roll + rhs.Roll
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T>& operator+=(EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return lhs = lhs + rhs;
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator-(const EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return EulerAngles<T>(
			lhs.Yaw - rhs.Yaw, lhs.Pitch - rhs.Pitch, lhs.Roll - rhs.Roll
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T>& operator-=(EulerAngles<T>& lhs, const EulerAngles<T>& rhs)
	{
		return lhs = lhs - rhs;
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator*(const EulerAngles<T>& lhs, T rhs)
	{
		return EulerAngles<T>(
			lhs.Yaw * rhs, lhs.Pitch * rhs, lhs.Roll * rhs
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator*(T lhs, const EulerAngles<T>& rhs)
	{
		return EulerAngles<T>(
			lhs * rhs.Yaw, lhs * rhs.Pitch, lhs * rhs.Roll
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T>& operator*=(EulerAngles<T>& lhs, T rhs)
	{
		return lhs = lhs * rhs;
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator/(const EulerAngles<T>& lhs, T rhs)
	{
		return EulerAngles<T>(
			lhs.Yaw / rhs, lhs.Pitch / rhs, lhs.Roll / rhs
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T> operator/(T lhs, const EulerAngles<T>& rhs)
	{
		return EulerAngles<T>(
			lhs / rhs.Yaw, lhs / rhs.Pitch, lhs / rhs.Roll
		);
	}

	/**
	 * \brief
	 * \param lhs
	 * \param rhs
	 * \return
	 */
	template <typename T>
	EulerAngles<T>& operator/=(EulerAngles<T>& lhs, T rhs)
	{
		return lhs = lhs / rhs;
	}
}

#endif // COCKTAIL_CORE_MATH_EULERANGLES_HPP
