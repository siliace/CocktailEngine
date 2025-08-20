#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

namespace Ck
{
	HeadlessSceneViewer::HeadlessSceneViewer(std::shared_ptr<Scene> scene, Extent2D<unsigned int> size, SceneViewerParameters parameters) :
		SceneViewer(std::move(scene)),
		mCurrentFramebuffer(0)
	{
		Renderer::RenderDevice* renderDevice = GetScene()->GetGraphicEngine()->GetRenderDevice();

		for (unsigned int i = 0; i < parameters.FrameCount; i++)
		{
			Renderer::TextureCreateInfo colorAttachmentCreateInfo;
			colorAttachmentCreateInfo.Type = Renderer::TextureType::e2D;
			colorAttachmentCreateInfo.Format = PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
			colorAttachmentCreateInfo.Size = MakeExtent(size, 1u);
			colorAttachmentCreateInfo.Usage = Renderer::TextureUsageFlagBits::Attachment;
			std::shared_ptr<Renderer::Texture> colorAttachment = renderDevice->CreateTexture(colorAttachmentCreateInfo);

			Renderer::TextureViewCreateInfo colorAttachmentViewCreateInfo;
			colorAttachmentViewCreateInfo.Texture = std::move(colorAttachment);
			colorAttachmentViewCreateInfo.Type = Renderer::TextureViewType::e2D;
			std::shared_ptr<Renderer::TextureView> colorAttachmentView = renderDevice->CreateTextureView(colorAttachmentViewCreateInfo);

			std::shared_ptr<Renderer::TextureView> depthStencilAttachmentView;
			if (parameters.DepthStencilFormat != PixelFormat::Undefined())
			{
				Renderer::TextureCreateInfo depthStencilAttachmentCreateInfo;
				depthStencilAttachmentCreateInfo.Type = Renderer::TextureType::e2D;
				depthStencilAttachmentCreateInfo.Format = PixelFormat::DepthStencil(24, 8);
				depthStencilAttachmentCreateInfo.Size = MakeExtent(size, 1u);
				depthStencilAttachmentCreateInfo.Usage = Renderer::TextureUsageFlagBits::Attachment;
				std::shared_ptr<Renderer::Texture> depthStencilAttachment = renderDevice->CreateTexture(colorAttachmentCreateInfo);

				Renderer::TextureViewCreateInfo depthStencilAttachmentViewCreateInfo;
				depthStencilAttachmentViewCreateInfo.Texture = std::move(depthStencilAttachment);
				depthStencilAttachmentViewCreateInfo.Type = Renderer::TextureViewType::e2D;
				depthStencilAttachmentView = renderDevice->CreateTextureView(colorAttachmentViewCreateInfo);
			}

			Renderer::FramebufferCreateInfo framebufferCreateInfo;
			framebufferCreateInfo.Samples = parameters.Samples;
			framebufferCreateInfo.ColorAttachmentCount = 1;
			framebufferCreateInfo.ColorAttachments[0] = std::move(colorAttachmentView);
			framebufferCreateInfo.DepthStencilAttachment = std::move(depthStencilAttachmentView);
			framebufferCreateInfo.DepthResolveMode = Renderer::ResolveMode::Average;
			framebufferCreateInfo.StencilResolveMode = Renderer::ResolveMode::SampleZero;

			mFramebuffers.Add(renderDevice->CreateFramebuffer(framebufferCreateInfo));
		}
	}

	Renderer::Framebuffer* HeadlessSceneViewer::AcquireNextFramebuffer(Renderer::RenderContext&) const
	{
		mCurrentFramebuffer = (mCurrentFramebuffer + 1) % mFramebuffers.GetSize();
		return mFramebuffers[mCurrentFramebuffer].get();
	}
}
