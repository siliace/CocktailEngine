#ifndef COCKTAIL_GRAPHIC_RENDERING_RESOURCE_GRAPHICRESOURCE_HPP
#define COCKTAIL_GRAPHIC_RENDERING_RESOURCE_GRAPHICRESOURCE_HPP

#include <Cocktail/Renderer/Memory/MemoryResource.hpp>

namespace Ck
{
    class GraphicEngine;

	/**
     * \brief 
     */
    class GraphicResource
    {
    public:

	    virtual ~GraphicResource() = default;

	    /**
         * \brief 
         * \return 
         */
        virtual Renderer::MemoryResource* GetUnderlyingResource() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual std::shared_ptr<GraphicEngine> GetGraphicEngine() const = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_GRAPHICRESOURCE_HPP
