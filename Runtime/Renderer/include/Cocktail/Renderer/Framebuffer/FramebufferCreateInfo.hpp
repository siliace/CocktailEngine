#ifndef COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFERCREATEINFO_HPP
#define COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFERCREATEINFO_HPP

#include <Cocktail/Core/Flags.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferLayout.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>

namespace Ck::Renderer
{
    /**
     * \brief 
     */
    enum class FramebufferCreateFlagBits
    {
    };

    using FramebufferCreateFlags = Flags<FramebufferCreateFlagBits>;

    /**
     * \brief Framebuffer descriptor structure
     * Describe the set of parameters to create a Framebuffer
     * \see RenderDevice::CreateFramebuffer
     */
    struct FramebufferCreateInfo
    {
        /**
         * \brief Specifies flags to apply to the Framebuffer to create
         */
        FramebufferCreateFlags Flags;

	    /**
         * \brief Specifies the number of rasterization samples of the Framebuffer to create
         */
        RasterizationSamples Samples = RasterizationSamples::e1;

    	/**
         * \brief Specifies the color attachments of the Framebuffer to create
         */
        Ref<TextureView> ColorAttachments[Framebuffer::MaxColorAttachmentCount];

    	/**
         * \brief Specifies the number of color attachments of the Framebuffer to create
         */
        unsigned int ColorAttachmentCount = 0;

    	/**
         * \brief Specifies the depth stencil attachment of the Framebuffer to create
         */
        Ref<TextureView> DepthStencilAttachment = nullptr;

        /**
         * \brief Specifies the name of the Framebuffer to create
         * If the extension Debug is not supported by the RenderDevice creating the Framebuffer, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
    };
}

#endif // COCKTAIL_RENDERER_RENDERTARGET_FRAMEBUFFERCREATEINFO_HPP
