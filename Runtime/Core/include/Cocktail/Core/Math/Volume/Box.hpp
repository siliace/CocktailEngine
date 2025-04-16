#ifndef COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP
#define COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP

#include <cassert>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Volume/Volume.hpp>

namespace Ck
{
	template <typename T>
	class Box : public Volume<T>
	{
	public:
		
		/**
		 * \brief Default constructor
		 * Create a null box
		 */
		Box() :
			mMinPoint(1),
			mMaxPoint(-1)
		{
			/// Nothing
		}	
		
		/**
		 * \brief Constructor
		 * Create a box that contains both \p p1 and \p p2
		 * \param p1 The first point contained in the box
		 * \param p2 The second point contained in the box
		 */
		Box(Vector3<T> p1, Vector3<T> p2) : 
			Box()
		{
			Box<T>::Extend(p1);			
			Box<T>::Extend(p2);			
		}
		
		/**
		 * \brief Extend the box to encompass a \p point
		 * \param point The point to encompass
		 * \return True if the box grown, false otherwise
		 */
		bool Extend(Vector3<T> point) override
		{
			if (Contains(point))
				return false;
				
			if (!IsNull())
			{
				mMinPoint = Vector3<T>::Min(mMinPoint, point);
				mMaxPoint = Vector3<T>::Max(mMaxPoint, point);
			}
			else
			{
				mMinPoint = point;
				mMaxPoint = point;
			}

			return true;
		}

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		bool Extend(const Volume<T>& other) override
		{
			return Volume<T>::Extend(other);
		}

		/**
	 	 * \brief Tell whether the box is null 
		 * \return True if the box is null, false otherwise
		 */
		bool IsNull() const override
		{
			return mMaxPoint.X() < mMinPoint.X() || mMaxPoint.Y() < mMinPoint.Y() || mMaxPoint.Z() < mMinPoint.Z();
		}

		/**
		 * \brief Tell whether a \p point is encompassed in the box
		 * \param point The point to test
		 * \return True if the box encompass the point, false otherwise
		 */
		bool Contains(const Vector3<T>& point) const override
		{
			if (IsNull())
				return false;
			
			if (point.X() < mMinPoint.X() || point.X() > mMaxPoint.X())
				return false;
			
			if (point.Y() < mMinPoint.Y() || point.Y() > mMaxPoint.Y())
				return false;
			
			return point.Z() >= mMinPoint.Z() && point.Z() <= mMaxPoint.Z();
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
			assert(index < 8);

			if (IsNull())
			{
				return Vector3<T>::Zero();
			}
			else
			{
				switch (index)
				{
				case 0: return { mMinPoint.X(), mMinPoint.Y(), mMinPoint.Z() };
				case 1:	return { mMaxPoint.X(), mMinPoint.Y(), mMinPoint.Z() };
				case 2:	return { mMaxPoint.X(), mMaxPoint.Y(), mMinPoint.Z() };
				case 3:	return { mMinPoint.X(), mMaxPoint.Y(), mMinPoint.Z() };
				case 4: return { mMinPoint.X(), mMinPoint.Y(), mMaxPoint.Z() };
				case 5:	return { mMaxPoint.X(), mMinPoint.Y(), mMaxPoint.Z() };
				case 6:	return { mMaxPoint.X(), mMaxPoint.Y(), mMaxPoint.Z() };
				case 7:	return { mMinPoint.X(), mMaxPoint.Y(), mMaxPoint.Z() };
				}

				COCKTAIL_UNREACHABLE();
				return {};
			}
		}

		/**
		 * \brief Retrieves the center of the box
		 * \return The center of the box
		 */
		Vector3<T> GetCenter() const override
		{
			if (IsNull())
				return Vector3<T>::Zero();
			
			Vector3<T> diagonale = mMaxPoint - mMinPoint;
			return mMinPoint + (diagonale / static_cast<T>(2));
		}
		
		/**
		 * \brief Retrieves the diagonale of the box
		 * The diagonale is computed as max - min if the box is not null.
		 * Otherwize the diagonale of a null box with be Vector<T>::Zero
		 * \return The diagonale of the box
		 */
		Vector3<T> GetDiagonale() const
		{
			return IsNull() ? Vector3<T>::Zero() : mMaxPoint - mMinPoint;
		}

		/**
		 * \brief 
		 * \return 
		 */
		const Vector3<T>& GetMinPoint() const
		{
			return mMinPoint;
		}

		/**
		 * \brief 
		 * \return 
		 */
		const Vector3<T>& GetMaxPoint() const
		{
			return mMaxPoint;
		}

	private:
	
		Vector3<T> mMinPoint;
		Vector3<T> mMaxPoint;
	};
}
#endif // COCKTAIL_CORE_MATH_VOLUMNE_BOX_HPP
