#ifndef COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
#define COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP

#include <CocktailEngine/Core/Utility/ByteArray.hpp>

#include <CocktailEngine/Graphic/Geometry/Vertex/VertexRef.hpp>

namespace Ck
{
    /**
     * \brief Container class for attributes of vertices sharing a same format
     */
    class COCKTAILENGINE_GRAPHIC_API VertexArray
    {
    public:

        /**
         * \brief Constructor
         *
         * \param layout The format of vertices stored
         * \param vertexCount The number of vertices stored
         * \param interlaced True if vertex attributes must be stored interlaced, false otherwise
         */
        VertexArray(SharedPtr<VertexLayout> layout, std::size_t vertexCount, bool interlaced = true);

        /**
         * \brief Merge another vertex array into this one
         *
         * \param other The vertex array to merge
         */
        void Merge(const VertexArray& other);

        /**
         * \brief Get a reference to a vertex
         *
         * \param index The index of the vertex
         *
         * \return A reference to the vertex at the specified index
         */
        VertexRef At(std::size_t index);

        /**
         * \brief Get a constant reference to a vertex
         *
         * \param index The index of the vertex
         *
         * \return A constant reference to the vertex at the specified index
         */
        VertexConstRef At(std::size_t index) const;

        /**
         * \brief Get the format of vertices stored
         *
         * \return The format
         */
        const SharedPtr<VertexLayout>& GetVertexLayout() const;

        /**
         * \brief Get the number of vertices stored
         *
         * \return The number of vertices
         */
        std::size_t GetVertexCount() const;

        /**
         * \brief Get the raw vertex data
         *
         * \return The vertex data as a byte array
         */
        const ByteArray& GetVertices() const;

    private:

        SharedPtr<VertexLayout> mVertexLayout;
        std::size_t mVertexCount;
        bool mInterlaced;
        ByteArray mVertices;
    };
}

#endif // COCKTAIL_GRAPHIC_GEOMETRY_VERTEX_VERTEXARRAY_HPP
