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
         * \param buffer
         * \param vertices
         */
        VertexBuffer(Ref<GraphicEngine> graphicEngine, Ref<Renderer::Buffer> buffer, Ref<VertexArray> vertices);

        /**
         * \brief 
         */
        void Upload() override;

        /**
         * \brief
         * \return
         */
        Ref<GraphicEngine> GetGraphicEngine() const override;

        /**
         * \brief
         * \return
         */
        Renderer::Buffer* GetUnderlyingResource() const override;

        /**
         * \brief 
         * \return 
         */
        Ref<VertexArray> GetVertexArray() const;

    private:

        Ref<GraphicEngine> mGraphicEngine;
        Ref<Renderer::Buffer> mBuffer;
        Ref<VertexArray> mVertices;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_VERTEXBUFFER_HPP
