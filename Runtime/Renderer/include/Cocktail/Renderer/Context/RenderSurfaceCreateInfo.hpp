#ifndef COCKTAIL_RENDERER_RENDERTARGET_RENDERSURFACECREATEINFO_HPP
#define COCKTAIL_RENDERER_RENDERTARGET_RENDERSURFACECREATEINFO_HPP

#include <Cocktail/Core/System/Monitor/VideoMode.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>

#include <Cocktail/Renderer/ColorSpace.hpp>
#include <Cocktail/Renderer/Format.hpp>
#include <Cocktail/Renderer/RasterizationSamples.hpp>

namespace Ck::Renderer
{
    /**
     * \brief RenderSurface descriptor structure
     * Describe the set of parameters to create a RenderSurface
     * \see RenderDevice::CreateRenderSurface
     */
    struct RenderSurfaceCreateInfo
    {
        /**
         * \brief Specifies the Window bound to the RenderSurface to create
         */
        Ref<Window> Window;

        /**
         * \brief Specifies the ColorSpace of the RenderSurface to create
         * If this value is not ColorSpace::Srgb, the RenderDevice creating the RenderSurface must support the extension RenderSurfaceColorSpace
         */
        ColorSpace ColorSpace = ColorSpace::Srgb;

        /**
         * \brief Specifies the ColorDepth of the RenderSurface to create
         * On modern operating systems, only ColorDepth::e32 and ColorDepth::e64 are supported
         */
        DisplayColorDepth ColorDepth = DisplayColorDepth::e32;

        /**
         * \brief Specifies the Alpha of the RenderSurface to create
         * This parameter is already counted in the \p ColorDepth parameter
         * so if \p AlphaDepth is AlphaDepth::e8 and \p ColorDepth is ColorDepth::e32,
         * the Format of the created render target will likely be Format::BGRA8Srgb or Format::RGBA8Srgb
         */
        DisplayAlphaDepth AlphaDepth = DisplayAlphaDepth::e8;

        /**
         * \brief Specifies the format of the depth buffer of the RenderSurface to create
         * If this format is not a depth/stencil format, an exception will be thrown
         * If this format is Format::Undefined, no depth buffer will be created
         */
        PixelFormat DepthStencilFormat = PixelFormat::Undefined();

        /**
         * \brief Specifies the number of rasterization samples of the RenderSurface to create
         */
        RasterizationSamples Samples = RasterizationSamples::e1;

        /**
         * \brief Specifies if the RenderSurface to create is created with vertical synchronization enabled
         * Enable it will avoid image tearing but might reduce the framerate
         * \see RenderSurface::EnableVsync
         */
        bool EnableVSync = true;

        /**
         * \brief Specifies the number of back buffer in the RenderSurface to create
         * This value may not be respected by the implementation to create the actual RenderSurface
         */
        unsigned int BufferCount = 3;

        /**
         * \brief Specifies the name of the RenderSurface to create
         * If the extension Debug is not supported by the RenderDevice creating the RenderSurface, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_RENDERTARGET_RENDERSURFACECREATEINFO_HPP
