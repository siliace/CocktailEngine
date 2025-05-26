#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
 
#include <Cocktail/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class BufferResource : public GraphicResource, public std::enable_shared_from_this<BufferResource>
    {
    public:

	    /**
         * \brief Constructor
         * \param graphicEngine
         * \param usage 
         * \param size 
         * \param name 
         */
        BufferResource(std::shared_ptr<GraphicEngine> graphicEngine, Renderer::BufferUsageFlags usage, std::size_t size, std::string_view name = "");

        /**
         * \brief
         * \return
         */
        Renderer::Buffer* GetUnderlyingResource() const override;

	    /**
	     * \brief
	     * \return 
	     */
	    std::shared_ptr<GraphicEngine> GetGraphicEngine() const override;

    private:

        std::shared_ptr<GraphicEngine> mGraphicEngine;
        std::shared_ptr<Renderer::Buffer> mBuffer;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
