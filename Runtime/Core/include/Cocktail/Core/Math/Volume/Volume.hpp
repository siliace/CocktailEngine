#ifndef COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP
#define COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP

#include <Cocktail/Core/Extent3D.hpp>
#include <Cocktail/Core/Math/Intersection.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
    /**
     * \brief Base class representing a generic bounding volume
     *
     * \tparam T Scalar type used for geometric computations
     */
    template <typename T>
    class Volume
    {
    public:

        /**
         * \brief Virtual destructor
         */
        virtual ~Volume() = default;

        /**
         * \brief Tests the intersection relationship with another volume
         *
         * The default implementation classifies the relationship by testing
         * all vertices of the other volume against this one
         *
         * \param volume The volume to test against
         *
         * \return The intersection classification result
         */
        virtual Intersection Intersect(const Volume<T>& volume) const
        {
            bool outside = false, inside = false;

            for (std::size_t i = 0; i < volume.GetVertexCount(); i++)
            {
                bool contains = Contains(volume.GetVertex(i));

                inside |= contains;
                outside |= !contains;

                if (inside && outside)
                    return Intersection::Intersect;
            }

            return inside ? Intersection::Inside : Intersection::Outside;
        }

        /**
         * \brief Tell whether the volume is null
         *
         * \return True if the volume is null, false otherwise
         */
        virtual bool IsNull() const = 0;

        /**
         * \brief Extends the volume to encompass a point
         *
         * \param point The point to encompass
         *
         * \return True if the volume grew, false otherwise
         */
        virtual bool Extend(Vector3<T> point) = 0;

        /**
         * \brief Extends the volume to encompass all vertices of another volume
         *
         * \param other The volume to encompass
         *
         * \return True if the volume grew, false otherwise
         */
        virtual bool Extend(const Volume<T>& other)
        {
            bool grown = false;
            if (!other.IsNull())
            {
                for (std::size_t i = 0; i < other.GetVertexCount(); i++)
                    grown |= Extend(other.GetVertex(i));
            }

            return grown;
        }

        /**
         * \brief Tests whether a vertex is contained inside the volume
         *
         * \param vertex The vertex to test
         *
         * \return True if the vertex is inside the volume, false otherwise
         */
        virtual bool Contains(const Vector3<T>& vertex) const = 0;

        /**
         * \brief Computes the axis-aligned extents of the volume
         *
         * \return The width, height and depth of the volume
         */
        Extent3D<T> GetExtents() const
        {
            if (IsNull())
                return MakeExtent<T>(0, 0, 0);

            Vector3<float> min = Vector3<float>::Infinite();
            Vector3<float> max = -Vector3<float>::Infinite();

            for (std::size_t i = 0; i < GetVertexCount(); i++)
            {
                Vector3<float> point = GetVertex(i);
                min = Vector3<float>::Min(min, point);
                max = Vector3<float>::Max(max, point);
            }

            return MakeExtent(max.X() - min.X(), max.Y() - min.Y(), max.Z() - min.Z());
        }

        /**
         * \brief Returns the number of vertices defining the volume
         *
         * \return The vertex count
         */
        virtual std::size_t GetVertexCount() const = 0;

        /**
         * \brief Returns a vertex of the volume
         *
         * \param index Index of the vertex
         *
         * \return The requested vertex
         */
        virtual Vector3<T> GetVertex(std::size_t index) const = 0;

        /**
         * \brief Returns the geometric center of the volume
         *
         * \return The center of the volume
         */
        virtual Vector3<T> GetCenter() const = 0;
    };
}

#endif // COCKTAIL_CORE_MATH_VOLUME_VOLUME_HPP
