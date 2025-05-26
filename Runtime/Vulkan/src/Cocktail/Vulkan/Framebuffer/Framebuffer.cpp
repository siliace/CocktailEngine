#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>

namespace Ck::Vulkan
{
	Framebuffer::Framebuffer(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<RenderPass> renderPass, const Renderer::FramebufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mRenderPass(renderPass),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		Renderer::RasterizationSamples samples = createInfo.Samples;

		mColorBufferCount = createInfo.ColorAttachmentCount;
		for (unsigned int i = 0; i < mColorBufferCount; i++)
		{
			std::unique_ptr<AttachmentBuffer> buffer = std::make_unique<AttachmentBuffer>(
				*mRenderDevice, 
				std::static_pointer_cast<TextureView>(createInfo.ColorAttachments[i]), 
				samples
			);

			Extent3D<unsigned int> bufferSize = buffer->GetSize();
			mSize.Width = std::max(mSize.Width, bufferSize.Width);
			mSize.Height = std::max(mSize.Height, bufferSize.Height);
			mSize.Depth = std::max(mSize.Depth, bufferSize.Depth);

			mColorBuffers[i] = std::move(buffer);
		}

		if (createInfo.DepthStencilAttachment)
		{
			mDepthStencilBuffer = std::make_unique<AttachmentBuffer>(
				*mRenderDevice, 
				std::static_pointer_cast<TextureView>(createInfo.DepthStencilAttachment),
				samples
			);

			Extent3D<unsigned int> bufferSize = mDepthStencilBuffer->GetSize();
			mSize.Width = std::max(mSize.Width, bufferSize.Width);
			mSize.Height = std::max(mSize.Height, bufferSize.Height);
			mSize.Depth = std::max(mSize.Depth, bufferSize.Depth);
		}

		unsigned int attachmentCount = 0;
		VkImageView attachments[(Renderer::Framebuffer::MaxColorAttachmentCount + 1) * 2];
		for (unsigned int i = 0; i < mColorBufferCount; i++)
		{
			AttachmentBuffer* colorBuffer = mColorBuffers[i].get();

			if (colorBuffer->IsMultisample())
				attachments[attachmentCount++] = colorBuffer->GetMultisampleAttachment()->GetHandle();

			attachments[attachmentCount++] = colorBuffer->GetResolveAttachment()->GetHandle();
		}

		if (mDepthStencilBuffer)
		{
			if (mDepthStencilBuffer->IsMultisample())
				attachments[attachmentCount++] = mDepthStencilBuffer->GetMultisampleAttachment()->GetHandle();

			attachments[attachmentCount++] = mDepthStencilBuffer->GetResolveAttachment()->GetHandle();
		}

		VkFramebufferCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.renderPass = mRenderPass->GetHandle(Renderer::RenderPassMode::Initial);
			vkCreateInfo.attachmentCount = attachmentCount;
			vkCreateInfo.pAttachments = attachments;
			vkCreateInfo.width = mSize.Width;
			vkCreateInfo.height = mSize.Height;
			vkCreateInfo.layers = mSize.Depth;
		}

		COCKTAIL_VK_CHECK(vkCreateFramebuffer(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));
	}

	Framebuffer::~Framebuffer()
	{
		vkDestroyFramebuffer(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}
	
	void Framebuffer::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_FRAMEBUFFER;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> Framebuffer::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Extent3D<unsigned int> Framebuffer::GetSize() const
	{
		return mSize;
	}

	Renderer::RasterizationSamples Framebuffer::GetSamples() const
	{
		return mRenderPass->GetSamples();
	}

	std::shared_ptr<Renderer::TextureView> Framebuffer::GetColorMultisampleAttachment(unsigned index) const
	{
		if (index >= mColorBufferCount)
			return nullptr;

		return mColorBuffers[index]->GetMultisampleAttachment();
	}

	std::shared_ptr<Renderer::TextureView> Framebuffer::GetColorAttachment(unsigned int index) const
	{
		if (index >= mColorBufferCount)
			return nullptr;

		return mColorBuffers[index]->GetResolveAttachment();
	}
	
	unsigned int Framebuffer::GetColorAttachmentCount() const
	{
		return mColorBufferCount;
	}

	std::shared_ptr<Renderer::TextureView> Framebuffer::GetDepthStencilMultisampleAttachment() const
	{
		if (!mDepthStencilBuffer)
			return nullptr;

		return mDepthStencilBuffer->GetMultisampleAttachment();
	}

	std::shared_ptr<Renderer::TextureView> Framebuffer::GetDepthStencilAttachment() const
	{
		if (!mDepthStencilBuffer)
			return nullptr;

		return mDepthStencilBuffer->GetResolveAttachment();
	}

	std::shared_ptr<RenderPass> Framebuffer::GetRenderPass() const
	{
		return mRenderPass;
	}

	VkFramebuffer Framebuffer::GetHandle() const
	{
		return mHandle;
	}
}
