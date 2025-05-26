#ifndef COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP
#define COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP

#include <memory>

namespace Ck::Renderer
{
    class RenderDevice;

	/**
     * \brief Base interface for every object instantiated from a RenderDevice
     */
    class RenderDeviceObject
    {
    public:

	    /**
         * \brief 
         */
        virtual ~RenderDeviceObject() = default;

	    /**
         * \brief 
         * \param name 
         */
        virtual void SetObjectName(const char* name) const = 0;

	    /**
         * \brief Get the RenderDevice used to created this object
         * \return The RenderDevice
         */
        virtual std::shared_ptr<RenderDevice> GetRenderDevice() const = 0;
    };  
}

#endif // COCKTAIL_RENDERER_RENDERDEVICEOBJECT_HPP
