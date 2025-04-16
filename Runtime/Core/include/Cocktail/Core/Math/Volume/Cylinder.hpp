#ifndef COCKTAIL_CORE_MATH_VOLUME_CYLINDER_HPP
#define COCKTAIL_CORE_MATH_VOLUME_CYLINDER_HPP

#include <Cocktail/Core/Math/Volume/Volume.hpp>

namespace Ck
{
	template <typename T>
	class Cylinder : public Volume<T>
	{
	public:

		/**
		 * \brief Default constructor
		 * Create a null cylinder
		 */
		Cylinder() :
			mRadius(0),
			mHalfExtent(0)
		{
			/// Nothing
		}

		/**
		 * \brief Constructor
		 * \param radius 
		 * \param halfExtent 
		 * \param center 
		 */
		Cylinder(T radius, T halfExtent, Vector3<T> center = Vector3<T>::Z()) :
			mRadius(radius),
			mHalfExtent(halfExtent),
			mCenter(center)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \return 
		 */
		bool IsNull() const override
		{
			return NearlyEqual(mRadius, static_cast<T>(0)) || NearlyEqual(mHalfExtent, static_cast<T>(0));
		}

		/**
		 * \brief 
		 * \param point 
		 * \return 
		 */
		bool Extend(Vector3<T> point) override
		{
			if (Contains(point))
				return false;

			if (!IsNull())
			{
				if (point.X() > mCenter.X() + mRadius || point.X() < mCenter.X() - mRadius)
					mRadius = std::max(mRadius, AbsoluteDifference(point.X(), mCenter.X()));

				if (point.Y() > mCenter.Y() + mHalfExtent || point.Y() < mCenter.Y() - mHalfExtent)
					mHalfExtent = AbsoluteDifference(point.Y(), mCenter.Y());

				if (point.Z() > mCenter.Z() + mRadius || point.Z() < mCenter.Z() - mRadius)
					mRadius = std::max(mRadius, AbsoluteDifference(point.Z(), mCenter.Z()));
			}
			else
			{
				mRadius = std::numeric_limits<T>::epsilon() * 2;
				mHalfExtent = mRadius;
				mCenter = point;
			}

			return true;
		}

		/**
		 * \brief 
		 * \param point 
		 * \return 
		 */
		bool Contains(const Vector3<T>& point) const override
		{
			if (point.X() > mCenter.X() + mRadius || point.X() < mCenter.X() - mRadius)
				return false;

			if (point.Y() > mCenter.Y() + mHalfExtent || point.Y() < mCenter.Y() - mHalfExtent)
				return false;

			if (point.Z() > mCenter.Z() + mRadius || point.Z() < mCenter.Z() - mRadius)
				return false;

			return true;
		}

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetVertexCount() const override
		{
			return 8;
		}

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Vector3<T> GetVertex(std::size_t index) const override
		{
			if (IsNull())
			{
				return Vector3<float>::Zero();
			}
			else
			{
				switch (index)
				{
				case 0: return { mCenter.X() + mRadius, mCenter.Y() + mHalfExtent, mCenter.Z() };
				case 1: return { mCenter.X() - mRadius, mCenter.Y() + mHalfExtent, mCenter.Z() };
				case 2: return { mCenter.X(), mCenter.Y() + mHalfExtent, mCenter.Z() + mRadius };
				case 3: return { mCenter.X(), mCenter.Y() + mHalfExtent, mCenter.Z() - mRadius };
				case 4: return { mCenter.X() + mRadius, mCenter.Y() - mHalfExtent, mCenter.Z() };
				case 5: return { mCenter.X() - mRadius, mCenter.Y() - mHalfExtent, mCenter.Z() };
				case 6: return { mCenter.X(), mCenter.Y() - mHalfExtent, mCenter.Z() + mRadius };
				case 7: return { mCenter.X(), mCenter.Y() - mHalfExtent, mCenter.Z() - mRadius };
				}

				COCKTAIL_UNREACHABLE();
				return {};
			}
		}

		/**
		 * \brief 
		 * \return 
		 */
		T GetRadius() const
		{
			return mRadius;
		}

		/**
		 * \brief 
		 * \return 
		 */
		T GetHalfExtent() const
		{
			return mHalfExtent;
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
		T mHalfExtent;
		Vector3<T> mCenter;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_CYLINDER_HPP
