#include <Cocktail/Renderer/Framebuffer/FramebufferLayout.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>

namespace Ck::Renderer
{
	FramebufferLayout GetRenderTargetLayout(const Framebuffer& framebuffer)
	{
		FramebufferLayout layout;
		layout.Samples = framebuffer.GetSamples();

		for (unsigned int i = 0; i < framebuffer.GetColorAttachmentCount(); i++)
			layout.ColorAttachmentFormats[i++] = framebuffer.GetColorAttachment(i)->GetTexture()->GetFormat();

		if (Ref<TextureView> depthStencilAttachment = framebuffer.GetDepthStencilAttachment(); depthStencilAttachment)
			layout.DepthStencilAttachmentFormat = depthStencilAttachment->GetTexture()->GetFormat();

		return layout;
	}
}
