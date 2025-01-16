#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP

#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>
#include <Cocktail/Graphic/Geometry/Index/IndexArray.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API IndexBuffer : public Extends<IndexBuffer, BufferResource>
	{
	public:

		/**
		 * \brief 
		 * \param graphicEngine 
		 * \param buffer 
		 * \param indices 
		 */
		IndexBuffer(Ref<GraphicEngine> graphicEngine, Ref<Renderer::Buffer> buffer, Ref<IndexArray> indices);

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
		Ref<IndexArray> GetIndexArray() const;

    private:

        Ref<GraphicEngine> mGraphicEngine;
        Ref<Renderer::Buffer> mBuffer;
        Ref<IndexArray> mIndices;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_INDEXBUFFER_HPP
