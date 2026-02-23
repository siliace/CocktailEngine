#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP

#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>
#include <Cocktail/Core/Math/Quaternion.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Utility/LazyValue.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
    /**
     * \brief Represents a 3D transformation composed of translation, rotation and scale
     *
     * Transformation encapsulates a TRS (Translation-Rotation-Scale) representation
     * and provides utilities for composition, inversion and application to vertices.
     *
     * The internal matrix representation is computed lazily and cached to avoid
     * unnecessary recomputation.
     */
    class COCKTAIL_GRAPHIC_API Transformation
    {
    public:

        /**
         * \brief Returns the identity transformation.
         *
         * Identity corresponds to:
         * - zero translation
         * - identity rotation
         * - unit scale
         */
        static Transformation Identity();

        /**
         * \brief Decomposes a transformation matrix into translation, rotation and scale
         *
         * \param transformationMatrix The matrix to decompose
         *
         * \return A Transformation representing the decomposed TRS components
         */
        static Transformation Decompose(const Matrix4<float>& transformationMatrix);

        /**
         * \brief Default constructor
         *
         * Creates a new instance of transformation with zero translation, rotation and scale.
         */
        Transformation() = default;

        /**
         * \brief Constructor
         *
         * Creates a new instance of transformation from translation, rotation and scale.
         *
         * \param translation Translation component
         * \param rotation Rotation component
         * \param scale Scale component
         */
        Transformation(Vector3<float> translation, Quaternion<float> rotation, Vector3<float> scale);

        /**
         * \brief Returns whether the scale is uniform on all axes
         *
         * \return True if scale.x == scale.y == scale.z
         */
        bool IsScaleUniform() const;

        /**
         * \brief Composes this transformation with a parent transformation
         *
         * The resulting transformation represents the combination:
         *
         * result = parent * this
         *
         * \param parent The parent transformation
         *
         * \return The composed transformation
         */
        Transformation Compose(const Transformation& parent) const;

        /**
         * \brief Returns the inverse transformation
         *
         * \return The inverse of this transformation
         */
        Transformation GetInverse() const;

        /**
         * \brief Applies the transformation to a 3D vertex
         *
         * \param vertex The vertex in local space
         *
         * \return The transformed vertex
         */
        Vector3<float> Apply(const Vector3<float>& vertex) const;

        /**
         * \brief Returns whether this transformation is the identity
         */
        bool IsIdentity() const;

        /**
         * \brief Returns the translation component
         */
        Vector3<float> GetTranslation() const;

        /**
         * \brief Sets the translation component
         *
         * Invalidates the cached matrix.
         *
         * \param translation New translation value
         *
         * \return Reference to this transformation
         */
        Transformation& SetTranslation(Vector3<float> translation);

        /**
         * \brief Returns the rotation component
         */
        Quaternion<float> GetRotation() const;

        /**
         * \brief Sets the rotation component
         *
         * Invalidates the cached matrix.
         *
         * \param rotation New rotation value
         *
         * \return Reference to this transformation
         */
        Transformation& SetRotation(Quaternion<float> rotation);

        /**
         * \brief Returns the scale component
         */
        Vector3<float> GetScale() const;

        /**
         * \brief Sets a uniform scale value
         *
         * \param scale Uniform scale factor applied to all axes
         *
         * \return Reference to this transformation
         */
        Transformation& SetScale(float scale);

        /**
         * \brief Sets a non-uniform scale
         *
         * \param scale Scale value for each axis
         *
         * \return Reference to this transformation
         */
        Transformation& SetScale(Vector3<float> scale);

        /**
         * \brief Converts the transformation to a 4x4 matrix
         *
         * The matrix is computed lazily and cached internally
         *
         * \return The transformation matrix
         */
        const Matrix4<float>& ToMatrix() const;

        /**
         * \brief Equality operator
         *
         * Compares translation, rotation and scale components
         */
        bool operator==(const Transformation& rhs) const;

        /**
         * \brief Inequality operator.
         */
        bool operator!=(const Transformation& rhs) const;

    private:

        /** Translation component. */
        Vector3<float> mTranslation;

        /** Rotation component (stored as quaternion). */
        Quaternion<float> mRotation;

        /** Scale component. */
        Vector3<float> mScale;

        /**
         * \brief Lazily computed transformation matrix.
         *
         * Recomputed only when TRS components change.
         */
        LazyValue<Matrix4<float>> mMatrix;
    };
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP
