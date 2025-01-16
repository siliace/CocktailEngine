#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	AttachmentBuffer::AttachmentBuffer(RenderDevice& renderDevice, Ref<TextureView> resolveAttachment, Renderer::RasterizationSamples samples) :
		mResolveAttachments(std::move(resolveAttachment))
	{
		if (samples == Renderer::RasterizationSamples::e1)
			return;
		
		Ref<AbstractTexture> texture = AbstractTexture::Cast(mResolveAttachments->GetTexture());
		Extent3D<unsigned int> textureSize = texture->GetSize();

		RenderBufferCreateInfo renderBufferCreateInfo;
		renderBufferCreateInfo.Format = texture->GetFormat();
		renderBufferCreateInfo.Size = MakeExtent(textureSize.Width, textureSize.Height);
		renderBufferCreateInfo.Samples = samples;
		Ref<RenderBuffer> renderBuffer = renderDevice.CreateRenderBuffer(renderBufferCreateInfo);

		Renderer::TextureViewCreateInfo viewCreateInfo;
		viewCreateInfo.Texture = renderBuffer;
		viewCreateInfo.Type = Renderer::TextureViewType::e2D;
		mMultisampleAttachment = TextureView::Cast(renderDevice.CreateTextureView(viewCreateInfo));
	}

	bool AttachmentBuffer::IsMultisample() const
	{
		return mMultisampleAttachment != nullptr;
	}

	const Ref<TextureView>& AttachmentBuffer::GetMultisampleAttachment() const
	{
		return mMultisampleAttachment;
	}

	const Ref<TextureView>& AttachmentBuffer::GetResolveAttachment() const
	{
		return mResolveAttachments;
	}

	Extent3D<unsigned int> AttachmentBuffer::GetSize() const
	{
		return mResolveAttachments->GetTexture()->GetSize();
	}
}
