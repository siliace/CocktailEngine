#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_GRAPHIC_API VertexBuffer : public Extends<VertexBuffer, BufferResource>
    {
    public:

        /**
         * \brief
         * \param graphicEngine
         * \param vertices
         * \param name
         */
        VertexBuffer(Ref<GraphicEngine> graphicEngine, Ref<VertexArray> vertices, std::string_view name = "");

        /**
         * \brief
         */
        void Upload();

        /**
         * \brief 
         */
        void Upload(std::size_t firstVertex, std::size_t vertexCount);

        /**
         * \brief 
         * \return 
         */
        Ref<VertexArray> GetVertexArray() const;

    private:

        Ref<VertexArray> mVertices;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP
