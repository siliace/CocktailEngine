#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR4_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR4_HPP

#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	/**
	 * \brief
	 * \tparam T
	 */
	template <typename T>
	class Vector4 : public Vector<Vector4, T, 4>
	{
	public:

	    using ElementType = typename Vector<Vector4, T, 4>::ElementType;

		static constexpr std::size_t Size = Vector<Vector4, T, 4>::Size;

		/**
		 * \brief
		 */
		Vector4() = default;

		/**
		 * \brief 
		 * \param value 
		 */
		explicit Vector4(T value) :
			Vector4(value, value, value, value)
		{
			/// Nothing
		}

		/**
		 * \brief
		 * \param x
		 * \param y
		 * \param z
		 * \param w
		 */
		Vector4(T x, T y, T z, T w)
		{
			X() = x;
			Y() = y;
			Z() = z;
			W() = w;
		}

		/**
		 * \brief 
		 * \param xyz 
		 * \param w 
		 */
		Vector4(const Vector3<T>& xyz, T w)
		{
			X() = xyz.X();
			Y() = xyz.Y();
			Z() = xyz.Z();
			W() = w;
		}

		/**
		 * \brief
		 * \return
		 */
		T& X()
		{
			return Vector<Vector4, T, 4>::At(0);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& X() const
		{
			return Vector<Vector4, T, 4>::At(0);
		}

		/**
		 * \brief
		 * \return
		 */
		T& Y()
		{
			return Vector<Vector4, T, 4>::At(1);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& Y() const
		{
			return Vector<Vector4, T, 4>::At(1);
		}

		/**
		 * \brief
		 * \return
		 */
		T& Z()
		{
			return Vector<Vector4, T, 4>::At(2);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& Z() const
		{
			return Vector<Vector4, T, 4>::At(2);
		}

		/**
		 * \brief
		 * \return
		 */
		T& W()
		{
			return Vector<Vector4, T, 4>::At(3);
		}

		/**
		 * \brief
		 * \return
		 */
		const T& W() const
		{
			return Vector<Vector4, T, 4>::At(3);
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<T> XYZ() const
		{
			return Vector3<T>(X(), Y(), Z());
		}
	};
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR4_HPP
