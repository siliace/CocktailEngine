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
     * \brief View frustum volume defined by six clipping planes
     *
     * \tparam T Scalar type used for geometric computations
     *
     * Frustum is typically used for visibility determination and frustum culling
     * against bounding volumes
     */
    template <typename T>
    class Frustum : public Volume<T>
    {
    public:

        /**
         * \brief Identifies one of the six frustum planes
         */
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
         * \brief Creates a perspective frustum from camera parameters
         *
         * \param angle Horizontal field of view angle
         * \param ratio Aspect ratio (width / height)
         * \param zBounds Near and far distances from the eye position
         * \param position Camera position
         * \param front Forward direction vector
         * \param right Right direction vector
         * \param up Up direction vector
         *
         * \return The constructed perspective frustum
         */
        static Frustum Perspective(const Angle<T>& angle, T ratio, const Vector2<T>& zBounds, const Vector3<T>& position, const Vector3<T>& front, const Vector3<T>& right, const Vector3<T>& up = Vector3<T>::Up())
        {
            Frustum<T> frustum;
            T zNear = zBounds.X();
            T zFar = zBounds.Y();

            T halfVSide = zFar * std::tan(angle / static_cast<T>(2));
            T halfHSide = halfVSide * ratio;
            Vector3<T> targetToFar = zFar * front;

            Vector3<T> nearCenter = position + front * zNear;
            Vector3<T> farCenter  = position + front * zFar;
            frustum.mCenter = (nearCenter + farCenter) / static_cast<T>(2);

            frustum.mPlanes[Face::Near] = Plane<T>(position + zNear * front, front);
            frustum.mPlanes[Face::Far] = Plane<T>(position + targetToFar, -front);
            frustum.mPlanes[Face::Right] = Plane<T>(position, Vector3<T>::CrossProduct(targetToFar - right * halfHSide, up));
            frustum.mPlanes[Face::Left] = Plane<T>(position, Vector3<T>::CrossProduct(up, targetToFar + right * halfHSide));
            frustum.mPlanes[Face::Top] = Plane<T>(position, Vector3<T>::CrossProduct(right, targetToFar - up * halfVSide));
            frustum.mPlanes[Face::Bottom] = Plane<T>(position, Vector3<T>::CrossProduct(targetToFar + up * halfVSide, right));

            return frustum;
        }

        /**
         * \brief Default constructor
         */
        Frustum() = default;

        /**
         * \brief Tests the intersection between the frustum and another volume
         *
         * \param volume The volume to test
         *
         * \return The intersection classification result
         */
        Intersection Intersect(const Volume<T>& volume) const override
        {
            bool fullyInside = true;

            for (Face face : Enum<Face>::Values)
            {
                const Plane<T>& plane = mPlanes[face];

                bool allOutside = true;
                bool allInside = true;

                for (std::size_t i = 0; i < volume.GetVertexCount(); ++i)
                {
                    T distance = plane.DistanceTo(volume.GetVertex(i));
                    const bool behindPlane = distance < static_cast<T>(0);

                    allOutside &= behindPlane;
                    allInside &= !behindPlane;
                }

                if (allOutside)
                    return Intersection::Outside;

                fullyInside &= allInside;
            }

            return fullyInside ? Intersection::Inside : Intersection::Intersect;
        }

        /**
         * \brief Indicates whether the frustum is null
         *
         * \return Always false for a valid frustum
         */
        bool IsNull() const override
        {
            return false;
        }

        /**
         * \brief Extending a frustum is not supported
         *
         * \param point Point to encompass
         *
         * \return Always false
         */
        bool Extend(Vector3<T> point) override
        {
            return false;
        }

        /**
         * \brief Tests whether a point lies inside the frustum
         *
         * \param vertex The point to test
         *
         * \return True if the point is inside all six planes
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
         * \brief Returns the number of frustum corner vertices
         *
         * \return Always 8
         */
        std::size_t GetVertexCount() const override
        {
            return 8;
        }

        /**
         * \brief Returns one of the eight frustum corner vertices
         *
         * \param index Vertex index in range [0, 7]
         *
         * \return The computed corner position
         */
        Vector3<T> GetVertex(std::size_t index) const override
        {
            Optional<Vector3<T>> point;
            switch (index)
            {
                case 0: point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Bottom], mPlanes[Face::Left]); break;
                case 1: point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Bottom], mPlanes[Face::Right]); break;
                case 2: point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Top], mPlanes[Face::Left]); break;
                case 3: point = Plane<T>::Intersect(mPlanes[Face::Near], mPlanes[Face::Top], mPlanes[Face::Right]); break;
                case 4: point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Bottom], mPlanes[Face::Left]); break;
                case 5: point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Bottom], mPlanes[Face::Right]); break;
                case 6: point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Top], mPlanes[Face::Left]); break;
                case 7: point = Plane<T>::Intersect(mPlanes[Face::Far], mPlanes[Face::Top], mPlanes[Face::Right]); break;
            }

            assert(!point.IsEmpty());
            return point.Get();
        }

        /**
         * \brief Returns the geometric center of the frustum
         *
         * \return The center position
         */
        Vector3<T> GetCenter() const override
        {
            return mCenter;
        }

        /**
         * \brief Returns one of the frustum planes
         *
         * \param face The requested plane
         *
         * \return The corresponding plane
         */
        Plane<T> GetPlane(Face face) const
        {
            return mPlanes[face];
        }

    private:

        Vector3<T> mCenter;
        EnumMap<Face, Plane<T>> mPlanes; /*!< Collection of the six clipping planes defining the frustum */
    };
}

#endif // COCKTAIL_CORE_MATH_VOLUME_FRUSTUM_HPP
