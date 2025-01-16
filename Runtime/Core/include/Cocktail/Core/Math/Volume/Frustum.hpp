#ifndef COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP
#define COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Math/BoxCorner.hpp>
#include <Cocktail/Core/Math/Plane.hpp>
#include <Cocktail/Core/Math/Vector/Vector2.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	enum class FrustumPlane
	{
		Bottom,
		Far,
		Left,
		Near,
		Right,
		Top,
	};

	/**
	 * \brief
	 * \tparam T
	 */
	template <typename T>
	class Frustum
	{
	public:

		/**
		 * \brief
		 * \param angle
		 * \param ratio
		 * \param zBounds
		 * \param eye
		 * \param target
		 * \param up
		 * \return
		 */
		static Frustum Perspective(const Angle<T>& angle, T ratio, const Vector2<T>& zBounds, const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up = Vector3<T>::Up())
		{
			Angle<T> halfAngle = angle / T(2.0);

			T tangent = std::tan(halfAngle);

			T nearH = zBounds.X() * tangent;
			T nearW = nearH * ratio;

			T farH = zBounds.Y() * tangent;
			T farW = farH * ratio;

			Vector3<T> forward = Vector3<T>::Normalize(target - eye);
			Vector3<T> normalizedUp = Vector3<T>::Normalize(up);
			Vector3<T> side = Vector3<T>::Normalize(Vector3<T>::CrossProduct(forward, normalizedUp));
			normalizedUp = Vector3<T>::CrossProduct(side, forward);

			Vector3<T> nc = eye + forward * zBounds.X();
			Vector3<T> fc = eye + forward * zBounds.Y();

			// Computing the frustum
			EnumMap<BoxCorner, Vector3<T>> corners;
			corners[BoxCorner::FarLeftBottom] = fc - normalizedUp * farH - side * farW;
			corners[BoxCorner::FarLeftTop] = fc + normalizedUp * farH - side * farW;
			corners[BoxCorner::FarRightTop] = fc + normalizedUp * farH + side * farW;
			corners[BoxCorner::FarRightBottom] = fc - normalizedUp * farH + side * farW;

			corners[BoxCorner::NearLeftBottom] = nc - normalizedUp * nearH - side * nearW;
			corners[BoxCorner::NearLeftTop] = nc + normalizedUp * nearH - side * nearW;
			corners[BoxCorner::NearRightTop] = nc + normalizedUp * nearH + side * nearW;
			corners[BoxCorner::NearRightBottom] = nc - normalizedUp * nearH + side * nearW;

			// Construction of frustum's planes

			Frustum frustum;
			frustum.Planes[FrustumPlane::Bottom] = Plane(corners[BoxCorner::NearLeftBottom], corners[BoxCorner::NearRightBottom], corners[BoxCorner::FarRightBottom]);
			frustum.Planes[FrustumPlane::Far] = Plane(corners[BoxCorner::FarRightTop], corners[BoxCorner::FarLeftTop], corners[BoxCorner::FarLeftBottom]);
			frustum.Planes[FrustumPlane::Left] = Plane(corners[BoxCorner::NearLeftTop], corners[BoxCorner::NearLeftBottom], corners[BoxCorner::FarLeftBottom]);
			frustum.Planes[FrustumPlane::Near] = Plane(corners[BoxCorner::NearLeftTop], corners[BoxCorner::NearRightTop], corners[BoxCorner::NearRightBottom]);
			frustum.Planes[FrustumPlane::Right] = Plane(corners[BoxCorner::NearRightBottom], corners[BoxCorner::NearRightTop], corners[BoxCorner::FarRightBottom]);
			frustum.Planes[FrustumPlane::Top] = Plane(corners[BoxCorner::NearRightTop], corners[BoxCorner::NearLeftTop], corners[BoxCorner::FarLeftTop]);

			return frustum;
		}

		/**
		 * \brief 
		 */
		Frustum() = default;

		/**
		 * \brief 
		 * \param planes 
		 */
		explicit Frustum(const EnumMap<FrustumPlane, Plane<T>>& planes) :
			Planes(planes)
		{
			/// Nothing
		}

		/**
		 * \brief 
		 * \param other 
		 */
		Frustum(const Frustum& other) = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Frustum& operator=(const Frustum& other) = default;

		EnumMap<FrustumPlane, Plane<T>> Planes;
	};
}

#endif // COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP
