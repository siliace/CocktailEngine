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
         * \brief 
         */
        virtual void Upload() = 0;

        /**
         * \brief
         * \return
         */
        Renderer::Buffer* GetUnderlyingResource() const override = 0;
    };
}

#endif // COCKTAIL_GRAPHIC_RENDERING_RESOURCE_BUFFERRESOURCE_HPP
