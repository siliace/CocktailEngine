#ifndef COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP
#define COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Math/Plane.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Math/Volume/Volume.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

namespace Ck
{
	/**
	 * \brief
	 * \tparam T
	 */
	template <typename T>
	class Frustum : public Volume<T>
	{
	public:

		enum class Face
		{
			Near,
			Far,
			Right,
			Left,
			Top,
			Bottom,
		};

		/**
		 * \brief Create a frustum from perspective parameter
		 * \param angle The horizontal field of view opening angle
		 * \param ratio The aspect ratio
		 * \param zBounds The distance to the near and far planes from the \p eye
		 * \param position The position of the eye
		 * \param front The front vector
		 * \param right The right vector
		 * \param up The up vector
		 * \return The created frustum
		 */
		static Frustum Perspective(const Angle<T>& angle, T ratio, const Vector2<T>& zBounds, const Vector3<T>& position, const Vector3<T>& front, const Vector3<T>& right, const Vector3<T>& up = Vector3<T>::Up())
		{
			Frustum<T> frustum;
			T zNear = zBounds.X();
			T zFar = zBounds.Y();

			T halfVSide = zFar * std::tan(angle / 2.f);
			T halfHSide = halfVSide * ratio;
			Vector3<T> targetToFar = zFar * front;

			frustum.mPlanes[Face::Near] = Plane<T>(position + zNear * front, front);
			frustum.mPlanes[Face::Far] = Plane<T>(position + targetToFar, -front);
			frustum.mPlanes[Face::Right] = Plane<T>(position, Vector3<T>::CrossProduct(targetToFar - right * halfHSide, up));
			frustum.mPlanes[Face::Left] = Plane<T>(position, Vector3<T>::CrossProduct(up, targetToFar + right * halfHSide));
			frustum.mPlanes[Face::Top] = Plane<T>(position, Vector3<T>::CrossProduct(right, targetToFar - up * halfVSide));
			frustum.mPlanes[Face::Bottom] = Plane<T>(position, Vector3<T>::CrossProduct(targetToFar + up * halfVSide, right));

			return frustum;
		}

		/**
		 * \brief 
		 */
		Frustum() = default;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsNull() const override
		{
			return false;
		}

		/**
		 * \brief 
		 * \param point 
		 * \return 
		 */
		bool Extend(Vector3<T> point) override
		{
			return false;
		}

		/**
		 * \brief
		 * \param vertex
		 * \return
		 */
		bool Contains(const Vector3<T>& vertex) const override
		{
			for (Face face : Enum<Face>::Values)
			{
				if (mPlanes[face].DistanceTo(vertex) < 0.f)
					return false;
			}

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
			Optional<Vector3<T>> point;
			switch (index)
			{
			case 0:
				point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Bottom], mPlanes[Face::Left]);
				break;

			case 1:
				point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Bottom], mPlanes[Face::Right]);
				break;

			case 2:
				point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Top], mPlanes[Face::Left]);
				break;

			case 3:
				point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Top], mPlanes[Face::Right]);
				break;

			case 4:
				point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Bottom], mPlanes[Face::Left]);
				break;

			case 5:
				point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Bottom], mPlanes[Face::Right]);
				break;

			case 6:
				point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Top], mPlanes[Face::Left]);
				break;

			case 7:
				point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Top], mPlanes[Face::Right]);
				break;
			}

			assert(!point.IsEmpty());
			return point.Get();
		}

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<T> GetCenter() const override
		{
			return Vector3<float>::Zero();
		}

		/**
		 * \brief 
		 * \param face 
		 * \return 
		 */
		Plane<T> GetPlane(Face face) const
		{
			return mPlanes[face];
		}

	private:

		EnumMap<Face, Plane<T>> mPlanes;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP
