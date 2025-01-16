#include <Cocktail/Vulkan/Framebuffer/RenderPassBuilder.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>

namespace Ck::Vulkan
{
	RenderPassBuilder::RenderPassBuilder(Renderer::RasterizationSamples samples):
		mSamples(samples)
	{
		mDepthStencilAttachmentReference.attachment = VK_ATTACHMENT_UNUSED;
		mDepthStencilAttachmentReference.layout = VK_IMAGE_LAYOUT_UNDEFINED;
	}

	void RenderPassBuilder::CreateAttachment(const PixelFormat& format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, bool presentable)
	{
		if (mSamples != Renderer::RasterizationSamples::e1)
		{
			VkAttachmentDescription attachmentDescription = CreateAttachmentDescription(format, mSamples, loadOp, storeOp, false);
			mAttachmentDescriptions.push_back(attachmentDescription);

			VkAttachmentReference attachmentReference;
			attachmentReference.attachment = static_cast<unsigned int>(mAttachmentDescriptions.size() - 1);

			if (format.IsColor())
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				mColorAttachmentReference.push_back(attachmentReference);
			}
			else
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				mDepthStencilAttachmentReference = attachmentReference;
			}
		}

		VkAttachmentDescription attachmentDescription = CreateAttachmentDescription(format, Renderer::RasterizationSamples::e1, loadOp, storeOp, presentable);
		mAttachmentDescriptions.push_back(attachmentDescription);

		VkAttachmentReference attachmentReference;
		attachmentReference.attachment = static_cast<unsigned int>(mAttachmentDescriptions.size() - 1);
		if (format.IsColor())
		{
			attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			mResolveAttachmentReference.push_back(attachmentReference);
		}
		else if (!format.IsColor() && mSamples == Renderer::RasterizationSamples::e1) /// In multisample case, the depth stencil attachment is the multisampmled one
		{
			attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			mDepthStencilAttachmentReference = attachmentReference;
		}
	}

	void RenderPassBuilder::CreateSubpass(VkPipelineBindPoint bindPoint)
	{
		VkSubpassDescription subpassDescription;
		subpassDescription.flags = 0;
		subpassDescription.pipelineBindPoint = bindPoint;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;

		if (mSamples != Renderer::RasterizationSamples::e1)
		{
			subpassDescription.colorAttachmentCount = static_cast<unsigned int>(mColorAttachmentReference.size());
			subpassDescription.pColorAttachments = mColorAttachmentReference.data();
			subpassDescription.pResolveAttachments = mResolveAttachmentReference.data();

		}
		else
		{
			subpassDescription.colorAttachmentCount = static_cast<unsigned int>(mResolveAttachmentReference.size());
			subpassDescription.pColorAttachments = mResolveAttachmentReference.data();
			subpassDescription.pResolveAttachments = nullptr;
		}

		subpassDescription.pDepthStencilAttachment = &mDepthStencilAttachmentReference;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;

		mSubpassDescriptions.push_back(subpassDescription);
	}

	void RenderPassBuilder::CreateDependency()
	{
	}

	VkRenderPassCreateInfo RenderPassBuilder::ToCreateInfo() const
	{
		VkRenderPassCreateInfo renderPassCreateInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr };
		{
			renderPassCreateInfo.flags = 0;
			renderPassCreateInfo.attachmentCount = static_cast<unsigned int>(mAttachmentDescriptions.size());
			renderPassCreateInfo.pAttachments = mAttachmentDescriptions.data();
			renderPassCreateInfo.subpassCount = static_cast<unsigned int>(mSubpassDescriptions.size());
			renderPassCreateInfo.pSubpasses = mSubpassDescriptions.data();
			renderPassCreateInfo.dependencyCount = static_cast<unsigned int>(mSubpassDependencies.size());
			renderPassCreateInfo.pDependencies = mSubpassDependencies.data();
		}

		return renderPassCreateInfo;
	}

	VkAttachmentDescription RenderPassBuilder::CreateAttachmentDescription(const PixelFormat& format, Renderer::RasterizationSamples samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, bool presentable)
	{
		bool isMultisample = samples != Renderer::RasterizationSamples::e1;

		VkAttachmentDescription attachmentDescription;
		attachmentDescription.flags = 0;
		attachmentDescription.format = ToVkType(format);
		attachmentDescription.samples = ToVkType(samples);
		attachmentDescription.loadOp = loadOp;
		attachmentDescription.storeOp = storeOp;

		if (format.IsDepthStencil())
		{
			attachmentDescription.stencilLoadOp = isMultisample ? VK_ATTACHMENT_LOAD_OP_DONT_CARE : loadOp;
			attachmentDescription.stencilStoreOp = isMultisample ? VK_ATTACHMENT_STORE_OP_DONT_CARE : storeOp;
		}
		else
		{
			attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}

		switch (loadOp)
		{
		case VK_ATTACHMENT_LOAD_OP_LOAD:
			attachmentDescription.initialLayout = presentable ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			break;

		case VK_ATTACHMENT_LOAD_OP_CLEAR: 
		case VK_ATTACHMENT_LOAD_OP_DONT_CARE:
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			break;
		}

		if (format.IsColor())
		{
			attachmentDescription.finalLayout = presentable ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}
		else
		{
			attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}

		return attachmentDescription;
	}
}
