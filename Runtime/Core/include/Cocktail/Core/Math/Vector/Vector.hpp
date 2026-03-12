#ifndef COCKTAIL_CORE_MATH_VECTOR_VECTOR_HPP
#define COCKTAIL_CORE_MATH_VECTOR_VECTOR_HPP

#include <algorithm>
#include <cmath>

#include <Cocktail/Core/StaticArray.hpp>

namespace Ck
{
    /**
     * \brief Base class for size fixed vectors
     *
     * \tparam TVector The vector class implemented
     *
     * \tparam T The type each component of the vector
     * \tparam S The number of component in the vector
     */
    template <template <class> class TVector, typename T, unsigned int S>
    class Vector
    {
    public:

        using ElementType = T;

        using VectorType = TVector<T>;

        static constexpr unsigned int Size = S;

        /**
         * \brief
         * \return
         */
        static VectorType Zero()
        {
            return VectorType();
        }

        /**
         * \brief
         * \return
         */
        static VectorType Epsilon()
        {
            return VectorType(std::numeric_limits<float>::epsilon());
        }

        /**
         * \brief
         * \return
         */
        static VectorType Unit()
        {
            return VectorType(static_cast<T>(1));
        }

        /**
         * \brief
         * \return
         */
        static VectorType Infinite()
        {
            return VectorType(std::numeric_limits<float>::max());
        }

        /**
         * \brief
         * \param value
         * \return
         */
        static VectorType Normalize(const Vector& value)
        {
            T length = value.GetLength();

            VectorType normalized;
            for (unsigned int i = 0; i < S; i++)
                normalized.At(i) = value.At(i) / length;

            return normalized;
        }

        /**
         * \brief
         * \param lhs
         * \param rhs
         * \return
         */
        static T DotProduct(const Vector& lhs, const Vector& rhs)
        {
            T result = 0;
            for (unsigned int i = 0; i < S; i++)
                result += lhs.At(i) * rhs.At(i);

            return result;
        }

        /**
         * \brief Multiplication operator
         * Compute the hadamard product of two vectors
         * \param lhs
         * \param rhs
         * \return
         */
        static TVector<T> HadamardProduct(const Vector& lhs, const Vector& rhs)
        {
            TVector<T> product;
            for (unsigned int i = 0; i < S; i++)
                product.At(i) = lhs.At(i) * rhs.At(i);

            return product;
        }

        /**
         * \brief Compute the squared distance between two points
         * \param lhs
         * \param rhs
         * \return
         */
        static T SquaredDistanceBetween(const Vector& lhs, const Vector& rhs)
        {
            VectorType difference;
            for (unsigned int i = 0; i < S; i++)
                difference.At(i) = AbsoluteDifference(lhs.At(i), rhs.At(i));

            return difference.GetSquaredLength();
        }

        /**
         * \brief
         * \param lhs
         * \param rhs
         * \return
         */
        static T DistanceBetween(const Vector& lhs, const Vector& rhs)
        {
            return std::sqrt(SquaredDistanceBetween(lhs, rhs));
        }

        /**
         * \brief
         * \param lhs
         * \param rhs
         * \return
         */
        static TVector<T> Min(const Vector& lhs, const Vector& rhs)
        {
            TVector<T> result;
            for (unsigned int i = 0; i < S; i++)
                result.At(i) = std::min(lhs.At(i), rhs.At(i));

            return result;
        }

        /**
         * \brief
         * \param lhs
         * \param rhs
         * \return
         */
        static TVector<T> Max(const Vector& lhs, const Vector& rhs)
        {
            TVector<T> result;
            for (unsigned int i = 0; i < S; i++)
                result.At(i) = std::max(lhs.At(i), rhs.At(i));

            return result;
        }

        /**
         * \brief Constructor
         * Initialize every component to 0
         */
        Vector() :
            Vector(0)
        {
            /// Nothing
        }

        /**
         * \brief Constructor
         * \param value
         */
        explicit Vector(T value)
        {
            mComponents.Fill(value);
        }

        /**
         * \brief Normalize
         * \return
         */
        TVector<T> Normalized() const
        {
            return Normalize(*this);
        }

        /**
         * \brief
         * \return
         */
        T GetSquaredLength() const
        {
            T length = 0.0;
            for (unsigned int i = 0; i < S; i++)
                length += std::pow(mComponents[i], static_cast<T>(2));

            return length;
        }

        /**
         * \brief
         * \return
         */
        T GetLength() const
        {
            return std::sqrt(GetSquaredLength());
        }

        /**
         * \brief
         * \return
         */
        unsigned int GetSize() const
        {
            return mComponents.GetSize();
        }

        /**
         * \brief
         * \param index
         * \return
         */
        T& At(unsigned int index)
        {
            return mComponents.At(index);
        }

        /**
         * \brief
         * \param index
         * \return
         */
        const T& At(unsigned int index) const
        {
            return mComponents.At(index);
        }

        /**
         * \brief
         */
        explicit operator T*()
        {
            return mComponents.GetData();
        }

        /**
         * \brief
         */
        explicit operator const T*() const
        {
            return mComponents.GetData();
        }

    private:

        StaticArray<T, S> mComponents;
    };

    /**
     * \brief
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    bool operator==(const Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        for (unsigned int i = 0; i < S; i++)
        {
            if (lhs.At(i) != rhs.At(i))
                return false;
        }

        return true;
    }

    /**
     * \brief
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    bool operator!=(const Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * \brief Addition operator
     * Compute the sum of two vectors
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator+(const Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        TVector<T> sum;
        for (unsigned int i = 0; i < S; i++)
            sum.At(i) = lhs.At(i) + rhs.At(i);

        return sum;
    }

    /**
     * \brief Addition operator
     * Compute the sum of two vectors
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T>& operator+=(Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        for (unsigned int i = 0; i < S; i++)
            lhs.At(i) += rhs.At(i);

        return static_cast<TVector<T>&>(lhs);
    }

    /**
     * \brief Negation operator
     * Compute the opposite vector of a given vector
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator-(const Vector<TVector, T, S>& rhs)
    {
        TVector<T> difference;
        for (unsigned int i = 0; i < S; i++)
            difference.At(i) = -1.0 * rhs.At(i);

        return difference;
    }

    /**
     * \brief Subtraction operator
     * Compute the difference between two vectors
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator-(const Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        TVector<T> difference;
        for (unsigned int i = 0; i < S; i++)
            difference.At(i) = lhs.At(i) - rhs.At(i);

        return difference;
    }

    /**
     * \brief Addition operator
     * Compute the sum of two vectors
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T>& operator-=(Vector<TVector, T, S>& lhs, const Vector<TVector, T, S>& rhs)
    {
        for (unsigned int i = 0; i < S; i++)
            lhs.At(i) -= rhs.At(i);

        return static_cast<TVector<T>&>(lhs);
    }

    /**
     * \brief Subtraction operator
     * Compute the multiplication between a scalar and a vector
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator*(T lhs, const Vector<TVector, T, S>& rhs)
    {
        TVector<T> product;
        for (unsigned int i = 0; i < S; i++)
            product.At(i) = lhs * rhs.At(i);

        return product;
    }

    /**
     * \brief Multiplication operator
     * Compute the multiplication between a scalar and a vector
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator*(const Vector<TVector, T, S>& lhs, T rhs)
    {
        TVector<T> product;
        for (unsigned int i = 0; i < S; i++)
            product.At(i) = lhs.At(i) * rhs;

        return product;
    }

    /**
     * \brief
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T>& operator*=(Vector<TVector, T, S>& lhs, T rhs)
    {
        for (unsigned int i = 0; i < S; i++)
            lhs.At(i) = lhs.At(i) * rhs;

        return static_cast<TVector<T>&>(lhs);
    }

    /**
     * \brief Subtraction operator
     * Compute the division between a scalar and a vector
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator/(T lhs, const Vector<TVector, T, S>& rhs)
    {
        TVector<T> result;
        for (unsigned int i = 0; i < S; i++)
            result.At(i) = lhs / rhs.At(i);

        return result;
    }

    /**
     * \brief Subtraction operator
     * Compute the multiplication between a scalar and a vector
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T> operator/(const Vector<TVector, T, S>& lhs, T rhs)
    {
        TVector<T> result;
        for (unsigned int i = 0; i < S; i++)
            result.At(i) = lhs.At(i) / rhs;

        return result;
    }

    /**
     * \brief
     * \tparam TVector
     * \tparam T
     * \tparam S
     * \param lhs
     * \param rhs
     * \return
     */
    template <template <class> class TVector, typename T, unsigned int S>
    TVector<T>& operator/=(Vector<TVector, T, S>& lhs, T rhs)
    {
        for (unsigned int i = 0; i < S; i++)
            lhs.At(i) = lhs.At(i) / rhs;

        return static_cast<TVector<T>&>(lhs);
    }
}

#endif // COCKTAIL_CORE_MATH_VECTOR_VECTOR_HPP
