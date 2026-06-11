#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderBuffer.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	AttachmentBuffer::AttachmentBuffer(RenderDevice& renderDevice, SharedPtr<TextureView> resolveAttachment, Renderer::RasterizationSamples samples) :
		mResolveAttachments(std::move(resolveAttachment))
	{
		if (samples == Renderer::RasterizationSamples::e1)
			return;
		
		SharedPtr<AbstractTexture> texture = mResolveAttachments->GetTexture().StaticCast<AbstractTexture>();
		Extent3D<unsigned int> textureSize = texture->GetSize();

		RenderBufferCreateInfo renderBufferCreateInfo;
		renderBufferCreateInfo.Format = texture->GetFormat();
		renderBufferCreateInfo.Size = MakeExtent(textureSize.Width, textureSize.Height);
		renderBufferCreateInfo.Samples = samples;
		SharedPtr<RenderBuffer> renderBuffer = renderDevice.CreateRenderBuffer(renderBufferCreateInfo);

		Renderer::TextureViewCreateInfo viewCreateInfo;
		viewCreateInfo.Texture = renderBuffer;
		viewCreateInfo.Type = Renderer::TextureViewType::e2D;
		mMultisampleAttachment = renderDevice.CreateTextureView(viewCreateInfo).StaticCast<TextureView>();
	}

	bool AttachmentBuffer::IsMultisample() const
	{
		return mMultisampleAttachment != nullptr;
	}

	SharedPtr<TextureView> AttachmentBuffer::GetMultisampleAttachment() const
	{
		return mMultisampleAttachment;
	}

	SharedPtr<TextureView> AttachmentBuffer::GetResolveAttachment() const
	{
		return mResolveAttachments;
	}

	Extent3D<unsigned int> AttachmentBuffer::GetSize() const
	{
		return mResolveAttachments->GetTexture()->GetSize();
	}
}
