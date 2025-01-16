#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

namespace Ck
{
	HeadlessSceneViewer::HeadlessSceneViewer(Ref<Scene> scene, Extent2D<unsigned int> size, SceneViewerParameters parameters) :
		Super(std::move(scene)),
		mCurrentFramebuffer(0)
	{
		Ref<Renderer::RenderDevice> renderDevice = GetScene()->GetGraphicEngine()->GetRenderDevice();

		for (unsigned int i = 0; i < parameters.FrameCount; i++)
		{
			Renderer::TextureCreateInfo colorAttachmentCreateInfo;
			colorAttachmentCreateInfo.Flags = Renderer::TextureCreateFlagBits::FramebufferAttachment;
			colorAttachmentCreateInfo.Type = Renderer::TextureType::e2D;
			colorAttachmentCreateInfo.Format = PixelFormat::Color(PixelFormat::Layout::RGBA, DataType::UnsignedInt8);
			colorAttachmentCreateInfo.Size = MakeExtent(size, 1u);
			Ref<Renderer::Texture> colorAttachment = renderDevice->CreateTexture(colorAttachmentCreateInfo);

			Renderer::TextureViewCreateInfo colorAttachmentViewCreateInfo;
			colorAttachmentViewCreateInfo.Texture = std::move(colorAttachment);
			colorAttachmentViewCreateInfo.Type = Renderer::TextureViewType::e2D;
			Ref<Renderer::TextureView> colorAttachmentView = renderDevice->CreateTextureView(colorAttachmentViewCreateInfo);

			Ref<Renderer::TextureView> depthStencilAttachmentView;
			if (parameters.DepthStencilFormat != PixelFormat::Undefined())
			{
				Renderer::TextureCreateInfo depthStencilAttachmentCreateInfo;
				depthStencilAttachmentCreateInfo.Flags = Renderer::TextureCreateFlagBits::FramebufferAttachment;
				depthStencilAttachmentCreateInfo.Type = Renderer::TextureType::e2D;
				depthStencilAttachmentCreateInfo.Format = PixelFormat::DepthStencil(24, 8);
				depthStencilAttachmentCreateInfo.Size = MakeExtent(size, 1u);
				Ref<Renderer::Texture> depthStencilAttachment = renderDevice->CreateTexture(colorAttachmentCreateInfo);

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

			mFramebuffers.push_back(renderDevice->CreateFramebuffer(framebufferCreateInfo));
		}
	}

	Renderer::Framebuffer* HeadlessSceneViewer::AcquireNextFramebuffer(Renderer::FrameContext& frameContext) const
	{
		mCurrentFramebuffer = (mCurrentFramebuffer + 1) % mFramebuffers.size();
		return mFramebuffers[mCurrentFramebuffer].Get();
	}
}
