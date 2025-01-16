#ifndef COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP
#define COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Volume/Detail/Volume.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Sphere : public Detail::Volume<T>
	{
	public:

		/**
		 * \brief 
		 * \param radius 
		 * \param center 
		 */
		explicit Sphere(T radius = 0, const Vector3<T>& center = Vector3<T>::Zero()) :
			Radius(radius),
			Center(center)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		bool Contains(const Vector3<T>& vertex) const override
		{
			return Vector3<T>::SquaredDistanceBetween(Center, vertex) - Radius * Radius <= std::numeric_limits<T>::epsilon();
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetVertexCount() const override
		{
			return 2;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Vector3<T> GetVertex(std::size_t index) const override
		{
			switch (index)
			{
			case 0:
				return Center;

			case 1:
				return { Center.X() + Radius, Center.Y(), Center.Z() };
			}

			COCKTAIL_UNREACHABLE();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<T> GetCenter() const override
		{
			return Center;
		}

		T Radius;
		Vector3<T> Center;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP
