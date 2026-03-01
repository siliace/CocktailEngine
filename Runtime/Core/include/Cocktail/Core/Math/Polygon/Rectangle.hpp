#ifndef COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP
#define COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP

#include <cassert>

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Extent2D.hpp>
#include <Cocktail/Core/Math/Polygon/Polygon.hpp>

namespace Ck
{
    template <typename T, typename Enable = void>
    struct SafeMakeUnsigned
    {
    };

    template <typename T>
    struct SafeMakeUnsigned<T, std::enable_if_t<std::is_floating_point_v<T>>>
    {
        using Type = T;
    };

    template <typename T>
    struct SafeMakeUnsigned<T, std::enable_if_t<!std::is_floating_point_v<T>>>
    {
        using Type = std::make_unsigned_t<T>;
    };

    template <typename T>
    using SafeMakeUnsignedType = typename SafeMakeUnsigned<T>::Type;

    /**
     * \brief
     *
     * \tparam T
     */
    template <typename T>
    class Rectangle : public Polygon<T>
    {
    public:

        static_assert(std::is_signed_v<T>);

        using SizeType = SafeMakeUnsignedType<T>;

        using PointType = Vector2<T>;
        using ExtentType = Extent2D<SizeType>;

        /**
         * \brief
         *
         * \param size
         *
         * \return
         */
        static Rectangle Square(SizeType size)
        {
            return Rectangle(MakeExtent(size, size));
        }

        /**
         * \brief
         *
         * \param position
         * \param size
         *
         * \return
         */
        static Rectangle Square(PointType position, SizeType size)
        {
            return Rectangle(position, MakeExtent(size, size));
        }

        /**
         * \brief
         *
         * \param extent
         */
        explicit Rectangle(ExtentType extent) :
            Rectangle(PointType::Zero(), extent)
        {
            /// Nothing
        }

        /**
         * \brief
         */
        Rectangle() = default;

        /**
         * \brief
         *
         * \param position
         * \param extent
         */
        Rectangle(PointType position, ExtentType extent) :
            Position(position),
            Extent(extent)
        {
            /// Nothing
        }

        /**
         * \brief
         *
         * \param x
         * \param y
         * \param width
         * \param height
         */
        Rectangle(T x, T y, SizeType width, SizeType height) :
            Rectangle(Vector2(x, y), MakeExtent(width, height))
        {
            /// Nothing
        }

        /**
         * \brief
         *
         * \param vertex
         *
         * \return
         */
        bool Contains(const PointType& vertex) const override
        {
            if (vertex.X() < Position.X() || vertex.X() > Position.X() + Extent.Width)
                return false;

            if (vertex.Y() < Position.Y() || vertex.Y() > Position.Y() + Extent.Height)
                return false;

            return true;
        }

        /**
         * \brief
         * \return
         */
        std::size_t GetVertexCount() const override
        {
            return 4;
        }

        /**
         * \brief
         * \param index
         * \return
         */
        PointType GetVertex(std::size_t index) const override
        {
            assert(index < 4);

            switch (index)
            {
                case 0: return { Position.X(), Position.Y() };
                case 1: return { Position.X() + Extent.Width, Position.Y() };
                case 2: return { Position.X() + Extent.Width, Position.Y() + Extent.Height };
                case 3: return { Position.X(), Position.Y() + Extent.Height };
            }

            COCKTAIL_UNREACHABLE();
            return {};
        }

        /**
         * \brief
         * \return
         */
        PointType GetCenter() const override
        {
            return (Position + Extent) / static_cast<T>(2);
        }

        PointType Position;
        ExtentType Extent;
    };
}

#endif // COCKTAIL_CORE_MATH_POLYGON_RECTANGLE_HPP
