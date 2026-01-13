#ifndef COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP
#define COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP

namespace Ck::Renderer
{
    /**
     * \brief Enumeration of optional extensions supported by a RenderDevice
     *
     * Extensions expose additional or non-core features that may or may not be
     * available depending on the underlying graphics backend or hardware.
     *
     * \see RenderDevice::IsSupportedExtension
     */
    enum class RenderDeviceExtension
    {
        /**
         * \brief Allows creation of RenderSurface objects
         *
         * This extension is required to create surfaces used for presentation
         * (e.g. window or swapchain surfaces).
         *
         * \see RenderSurfaceCreateInfo
         * \see RenderDevice::CreateRenderSurface
         */
        RenderSurface,

        /**
         * \brief Allows creation of a RenderSurface using a color space other than ColorSpace::Srgb
         *
         * Enables support for alternative color spaces such as HDR formats.
         *
         * \see RenderSurfaceCreateInfo::ColorSpace
         * \see RenderDevice::CreateRenderSurface
         */
        RenderSurfaceColorSpace,

        /**
         * \brief Enables reporting of debug and validation messages from the driver
         *
         * Useful for development and debugging purposes.
         *
         * \see RenderDevice::OnDebugMessage
         */
        Debug,

        /**
         * \brief Enables per-instance attribute divisors in vertex input
         *
         * Allows vertex attributes to advance at a rate different from per-vertex,
         * which is commonly used for hardware instancing.
         */
        InstanceDivisor,

        /**
         * \brief Allows creation of TextureView objects with a format different from the source Texture
         *
         * Requires the source texture to be created with compatible formats.
         *
         * \see TextureViewCreateInfo::Format
         * \see TextureCreateFlagBits::MutableFormat
         * \see TextureCreateInfo::CompatibleFormats
         * \see TextureCreateInfo::CompatibleFormatCount
         */
        MutableTextureFormat,

        /**
         * \brief Allows creation of a 2D array TextureView from a single mip level
         *        of a 3D texture
         *
         * This is useful for techniques that reinterpret 3D textures as layered 2D arrays.
         */
        TextureView2DArrayCompatible,

        /**
         * \brief Allows index buffers using 1-byte (8-bit) indices
         *
         * Useful for very small meshes to reduce memory usage.
         *
         * \see IndexType::Byte
         */
        ByteIndexType,

        /**
         * \brief Enables timeline-based synchronization primitives
         *
         * Provides synchronization mechanisms based on monotonically increasing
         * counters instead of binary fences or semaphores.
         */
        TimelineSynchronization,

        /**
         * \brief Enables variable shading rate rendering
         *
         * Allows the GPU to vary fragment shading frequency across the framebuffer
         * to improve performance.
         */
        VariableShadingRate,
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICEEXTENSION_HPP
