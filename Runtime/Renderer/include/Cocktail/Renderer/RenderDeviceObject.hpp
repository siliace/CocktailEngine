#ifndef COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP
#define COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP

#include <Cocktail/Core/Memory/Ref.hpp>
#include <Cocktail/Core/Meta/Implements.hpp>

namespace Ck::Renderer
{
    class RenderDevice;

	/**
     * \brief Base interface for every object instantiated from a RenderDevice
     */
    class RenderDeviceObject : public Implements<RenderDeviceObject, Interface>
    {
    public:

	    /**
         * \brief 
         * \param name 
         */
        virtual void SetObjectName(const char* name) const = 0;

	    /**
         * \brief Get the RenderDevice used to created this object
         * \return The RenderDevice
         */
        virtual Ref<RenderDevice> GetRenderDevice() const = 0;
    };  
}

#endif // COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP
