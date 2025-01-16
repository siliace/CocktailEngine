#ifndef COCKTAIL_RENDERER_MEMORY_MEMORYRESOURCE_HPP
#define COCKTAIL_RENDERER_MEMORY_MEMORYRESOURCE_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>
#include <Cocktail/Renderer/Memory/MemoryType.hpp>
#include <Cocktail/Renderer/Memory/ResourceType.hpp>

namespace Ck::Renderer
{
    /**
     * \brief 
     */
    class MemoryResource : public Implements<MemoryResource, RenderDeviceObject>
    {
    public:

	    /**
         * \brief 
         * \return 
         */
        virtual bool IsExclusive() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual MemoryType GetMemoryType() const = 0;

	    /**
         * \brief 
         * \return 
         */
        virtual ResourceType GetResourceType() const = 0;
    }; 
}

#endif // COCKTAIL_RENDERER_MEMORY_MEMORYRESOURCE_HPP
