#include <CocktailEngine/Renderer/Framebuffer/FramebufferLayout.hpp>
#include <CocktailEngine/Renderer/Texture/Texture.hpp>
#include <CocktailEngine/Renderer/Texture/TextureView.hpp>

namespace Ck::Renderer
{
	FramebufferLayout GetRenderTargetLayout(const Framebuffer& framebuffer)
	{
		FramebufferLayout layout;
		layout.Samples = framebuffer.GetSamples();

		for (unsigned int i = 0; i < framebuffer.GetColorAttachmentCount(); i++)
			layout.ColorAttachmentFormats[i++] = framebuffer.GetColorAttachment(i)->GetTexture()->GetFormat();

		if (SharedPtr<TextureView> depthStencilAttachment = framebuffer.GetDepthStencilAttachment())
			layout.DepthStencilAttachmentFormat = depthStencilAttachment->GetTexture()->GetFormat();

		return layout;
	}
}
