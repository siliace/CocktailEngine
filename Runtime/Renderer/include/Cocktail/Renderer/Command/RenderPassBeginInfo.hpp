#ifndef COCKTAIL_RENDERER_COMMAND_RENDERPASSBEGININFO_HPP
#define COCKTAIL_RENDERER_COMMAND_RENDERPASSBEGININFO_HPP

#include <Cocktail/Core/Color.hpp>

#include <Cocktail/Renderer/Framebuffer/Framebuffer.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Enumeration of possible modes to start a render pass
	 */
	enum class RenderPassMode
	{
		/**
		 * \brief 
		 */
		Initial,

		/**
		 * \brief Clear framebuffer attachments
		 */
		Clear,

		/**
		* \brief Load framebuffer attachments
		*/
		Load,
	};

	/**
	 * \brief Structure describing how to start a render pass
	 * \see CommandList::BeginRenderPass
	 */
	struct RenderPassBeginInfo
	{
		/**
		 * \brief Specifies the mode to use to begin the render pass
		 */
		RenderPassMode Mode = RenderPassMode::Clear;

		/**
		 * \brief Specifies the render target to write when issuing draw calls
		 */
		const Framebuffer* Framebuffer = nullptr;

		/**
		 * \brief Specifies the colors to use to clear colors attachments of \p Framebuffer
		 * If \p Mode is not Clear, this parameter is ignored
		 */
		LinearColor ColorClearValue[Framebuffer::MaxColorAttachmentCount];

		/**
		 * \brief Specifies the color to use to clear depth aspect of the depth stencil attachments of \p Framebuffer
		 * If \p Mode is not Clear, this parameter is ignored
		 * If \p Framebuffer does not have a depth capable attachment, this parameter is ignored
		 */
		float DepthClearValue = 1.f;

		/**
		 * \brief Specifies the color to use to clear stencil aspect of the depth stencil attachments of \p Framebuffer
		 * If \p Mode is not Clear, this parameter is ignored
		 * If \p Framebuffer does not have a stencil capable attachment, this parameter is ignored
		 */
		unsigned int StencilClearValue = 0;
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_RENDERPASSBEGININFO_HPP
