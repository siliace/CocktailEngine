#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class BufferResource : public Inherit<BufferResource, Object, GraphicResource>
    {
    public:

	    /**
         * \brief Constructor
         * \param graphicEngine
         * \param usage 
         * \param size 
         * \param name 
         */
        BufferResource(Ref<GraphicEngine> graphicEngine, Renderer::BufferUsageFlags usage, std::size_t size, std::string_view name = "");

        /**
         * \brief
         * \return
         */
        Renderer::Buffer* GetUnderlyingResource() const override;

	    /**
	     * \brief
	     * \return 
	     */
	    Ref<GraphicEngine> GetGraphicEngine() const override;

    private:

        Ref<GraphicEngine> mGraphicEngine;
    	Ref<Renderer::Buffer> mBuffer;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
