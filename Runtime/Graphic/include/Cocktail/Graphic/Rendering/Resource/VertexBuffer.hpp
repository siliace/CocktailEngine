#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Geometry/Vertex/VertexArray.hpp>

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_GRAPHIC_API VertexBuffer : public BufferResource
    {
    public:

        /**
         * \brief
         * \param graphicEngine
         * \param vertices
         * \param name
         */
        VertexBuffer(std::shared_ptr<GraphicEngine> graphicEngine, std::shared_ptr<VertexArray> vertices, const AnsiChar* name = "");

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
        std::shared_ptr<VertexArray> GetVertexArray() const;

    private:

        std::shared_ptr<VertexArray> mVertices;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP
