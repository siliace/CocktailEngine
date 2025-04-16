#ifndef COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP
#define COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Volume/Volume.hpp>

namespace Ck
{
	/**
	 * \brief 
	 * \tparam T 
	 */
	template <typename T>
	class Sphere : public Volume<T>
	{
	public:

		/**
		 * \brief Default constructor
		 * Create a null sphere
		 */
		Sphere() :
			mRadius(0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param radius 
		 * \param center 
		 */
		explicit Sphere(T radius, const Vector3<T>& center = Vector3<T>::Zero()) :
			mRadius(radius),
			mCenter(center)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * Create a sphere that contains both \p p1 and \p p2
		 * \param p1 The first point contained in the sphere
		 * \param p2 The second point contained in the sphere
		 */
		Sphere(Vector3<T> p1, Vector3<T> p2)
		{
			mCenter.X() = (p1.X() + p2.X()) / static_cast<T>(2);
			mCenter.Y() = (p1.Y() + p2.Y()) / static_cast<T>(2);
			mCenter.Z() = (p1.Z() + p2.Z()) / static_cast<T>(2);
			mRadius = Vector3<T>::DistanceBetween(p1, p2) / static_cast<T>(2);
		}

		bool Extend(Vector3<T> point) override
		{
			if (Contains(point))
				return false;

			if (!IsNull())
			{
				mRadius = (point - mCenter).GetLength();
			}
			else
			{
				mCenter = Vector3<T>::Zero();
				mRadius = point.GetLength();
			}

			return true;
		}

		/**
		 * \brief Tell whether the sphere is null
		 * A sphere is considered null if its radius is 0
		 * \return True if the sphere is null, false otherwise
		 */
		bool IsNull() const override
		{
			return NearlyEqual(mRadius, static_cast<T>(0));
		}

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		bool Contains(const Vector3<T>& vertex) const override
		{
			if (IsNull())
				return false;

			return Vector3<T>::SquaredDistanceBetween(mCenter, vertex) - mRadius * mRadius <= std::numeric_limits<T>::epsilon();
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetVertexCount() const override
		{
			return 6;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Vector3<T> GetVertex(std::size_t index) const override
		{
			assert(index < 6);

			if (IsNull())
			{
				return Vector3<T>::Zero();
			}
			else
			{
				switch (index)
				{
				case 0:	return { mCenter.X() + mRadius, mCenter.Y(), mCenter.Z() };
				case 1:	return { mCenter.X() - mRadius, mCenter.Y(), mCenter.Z() };
				case 2:	return { mCenter.X(), mCenter.Y() + mRadius, mCenter.Z() };
				case 3:	return { mCenter.X(), mCenter.Y() - mRadius, mCenter.Z() };
				case 4:	return { mCenter.X(), mCenter.Y(), mCenter.Z() + mRadius };
				case 5:	return { mCenter.X(), mCenter.Y(), mCenter.Z() - mRadius };
				}

				COCKTAIL_UNREACHABLE();
				return {};
			}
		}

		T GetRadius() const
		{
			return mRadius;
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<T> GetCenter() const override
		{
			return mCenter;
		}

	private:

		T mRadius;
		Vector3<T> mCenter;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_SPHERE_HPP
