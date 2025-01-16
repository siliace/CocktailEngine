#ifndef COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP
#define COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP

namespace Ck::Renderer
{
	/**
     * \brief Enumeration of possible extensions supported by a RenderDevice
     * \see RenderDevice::IsSupportedExtension
     */
    enum class RenderDeviceExtension
    {
	    /**
	     * \brief Extension allowing to create RenderSurface
         * \see RenderSurfaceCreateInfo
         * \see RenderDevice::CreateRenderSurface
	     */
	    RenderSurface,

	    /**
         * \brief Extension allowing to create a RenderSurface with another ColorSpace than ColorSpace::Srgb
         * \see RenderSurfaceCreateInfo::ColorSpace
         * \see RenderDevice::CreateRenderSurface
         */
        RenderSurfaceColorSpace,

	    /**
         * \brief 
         */
        Debug,

	    /**
         * \brief 
         */
        InstanceDivisor,

	    /**
         * \brief Extension allowing to create TextureView with a Format that differ from the source Texture
         * \see TextureViewCreateInfo::Format
         * \see TextureCreateFlagBits::MutableFormat
         * \see TextureCreateInfo::CompatibleFormats
         * \see TextureCreateInfo::CompatibleFormatCount
         */
        MutableTextureFormat,

	    /**
         * \brief Extension allowing to create a TextureView with the type e2DArray form a level of a Texture with type e3D
         */
        TextureView2DArrayCompatible,

        /**
         * \brief Extension allowing to create index buffers with an index size of 1 byte
         * \see IndexType::Byte
         */
        ByteIndexType,

	    /**
         * \brief 
         */
        TimelineSynchronization,
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP
