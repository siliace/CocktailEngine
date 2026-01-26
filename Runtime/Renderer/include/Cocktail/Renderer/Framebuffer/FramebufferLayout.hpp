#ifndef COCKTAIL_RENDERER_FRAMEBUFFER_FRAMEBUFFERLAYOUT_HPP
#define COCKTAIL_RENDERER_FRAMEBUFFER_FRAMEBUFFERLAYOUT_HPP

#include <Cocktail/Renderer/PixelFormat.hpp>

#include <Cocktail/Renderer/Export.hpp>
#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/Framebuffer/Framebuffer.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Describe the layout of a RenderTarget
     */
    struct FramebufferLayout
    {
	    /**
         * \brief Specifies the number of rasterization samples of the RenderTarget
         */
        RasterizationSamples Samples = RasterizationSamples::e1;

    	/**
         * \brief Specifies the formats of the color attachments of the RenderTarget
         */
        PixelFormat ColorAttachmentFormats[Framebuffer::MaxColorAttachmentCount];

	    /**
         * \brief Specifies the number of color attachment of the RenderTarget
         */
        unsigned int ColorAttachmentCount = 0;

	    /**
         * \brief Specifies the Format of the depth buffer of the RenderTarget
         * If the RenderTarget does not have a depth buffer, the Format will be Undefined
         */
        PixelFormat DepthStencilAttachmentFormat;
    };

    /**
     * \brief Get the RenderTargetLayout of a RenderTarget
     * \param renderTarget
     * \return 
     */
	COCKTAIL_RENDERER_API FramebufferLayout GetFramebufferLayout(const Framebuffer& renderTarget);
};

#endif // COCKTAIL_RENDERER_RENDERTARGET_RENDERTARGETLAYOUT_HPP
