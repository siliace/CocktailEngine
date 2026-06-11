#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP

#include <Cocktail/Core/Memory/SharedFromThis.hpp>
 
#include <Cocktail/Graphic/Export.hpp>
#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAIL_GRAPHIC_API BufferResource : public GraphicResource, public SharedFromThis<BufferResource>
    {
    public:

	    /**
         * \brief Constructor
         * \param graphicEngine
         * \param usage 
         * \param size 
         * \param name 
         */
        BufferResource(SharedPtr<GraphicEngine> graphicEngine, Renderer::BufferUsageFlags usage, std::size_t size, const AnsiChar* name = "");

        /**
         * \brief
         * \return
         */
        Renderer::Buffer* GetUnderlyingResource() const override;

	    /**
	     * \brief
	     * \return 
	     */
	    SharedPtr<GraphicEngine> GetGraphicEngine() const override;

    private:

        SharedPtr<GraphicEngine> mGraphicEngine;
        SharedPtr<Renderer::Buffer> mBuffer;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
