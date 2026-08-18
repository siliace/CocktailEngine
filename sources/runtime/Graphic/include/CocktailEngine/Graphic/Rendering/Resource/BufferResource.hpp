#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP

#include <CocktailEngine/Core/Memory/SharedFromThis.hpp>
 
#include <CocktailEngine/Graphic/Export.hpp>
#include <CocktailEngine/Graphic/Rendering/Resource/GraphicResource.hpp>

#include <CocktailEngine/Renderer/Buffer/Buffer.hpp>

namespace Ck
{
	/**
     * \brief 
     */
    class COCKTAILENGINE_GRAPHIC_API BufferResource : public GraphicResource, public SharedFromThis<BufferResource>
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
