#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	AttachmentBuffer::AttachmentBuffer(RenderDevice& renderDevice, std::shared_ptr<TextureView> resolveAttachment, Renderer::RasterizationSamples samples) :
		mResolveAttachments(std::move(resolveAttachment))
	{
		if (samples == Renderer::RasterizationSamples::e1)
			return;
		
		std::shared_ptr<AbstractTexture> texture = std::static_pointer_cast<AbstractTexture>(mResolveAttachments->GetTexture());
		Extent3D<unsigned int> textureSize = texture->GetSize();

		RenderBufferCreateInfo renderBufferCreateInfo;
		renderBufferCreateInfo.Format = texture->GetFormat();
		renderBufferCreateInfo.Size = MakeExtent(textureSize.Width, textureSize.Height);
		renderBufferCreateInfo.Samples = samples;
		std::shared_ptr<RenderBuffer> renderBuffer = renderDevice.CreateRenderBuffer(renderBufferCreateInfo);

		Renderer::TextureViewCreateInfo viewCreateInfo;
		viewCreateInfo.Texture = renderBuffer;
		viewCreateInfo.Type = Renderer::TextureViewType::e2D;
		mMultisampleAttachment = std::static_pointer_cast<TextureView>(renderDevice.CreateTextureView(viewCreateInfo));
	}

	bool AttachmentBuffer::IsMultisample() const
	{
		return mMultisampleAttachment != nullptr;
	}

	std::shared_ptr<TextureView> AttachmentBuffer::GetMultisampleAttachment() const
	{
		return mMultisampleAttachment;
	}

	std::shared_ptr<TextureView> AttachmentBuffer::GetResolveAttachment() const
	{
		return mResolveAttachments;
	}

	Extent3D<unsigned int> AttachmentBuffer::GetSize() const
	{
		return mResolveAttachments->GetTexture()->GetSize();
	}
}
