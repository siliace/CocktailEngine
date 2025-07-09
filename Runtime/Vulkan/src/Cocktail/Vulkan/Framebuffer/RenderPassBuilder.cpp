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
			mAttachmentDescriptions.Add(attachmentDescription);

			VkAttachmentReference attachmentReference;
			attachmentReference.attachment = mAttachmentDescriptions.GetSize() - 1;

			if (format.IsColor())
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				mColorAttachmentReference.Add(attachmentReference);
			}
			else
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				mDepthStencilAttachmentReference = attachmentReference;
			}
		}

		VkAttachmentDescription attachmentDescription = CreateAttachmentDescription(format, Renderer::RasterizationSamples::e1, loadOp, storeOp, presentable);
		mAttachmentDescriptions.Add(attachmentDescription);

		VkAttachmentReference attachmentReference;
		attachmentReference.attachment = mAttachmentDescriptions.GetSize() - 1;
		if (format.IsColor())
		{
			attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			mResolveAttachmentReference.Add(attachmentReference);
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
			subpassDescription.colorAttachmentCount = mColorAttachmentReference.GetSize();
			subpassDescription.pColorAttachments = mColorAttachmentReference.GetData();
			subpassDescription.pResolveAttachments = mResolveAttachmentReference.GetData();

		}
		else
		{
			subpassDescription.colorAttachmentCount = mResolveAttachmentReference.GetSize();
			subpassDescription.pColorAttachments = mResolveAttachmentReference.GetData();
			subpassDescription.pResolveAttachments = nullptr;
		}

		subpassDescription.pDepthStencilAttachment = &mDepthStencilAttachmentReference;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;

		mSubpassDescriptions.Add(subpassDescription);
	}

	void RenderPassBuilder::CreateDependency()
	{
	}

	VkRenderPassCreateInfo RenderPassBuilder::ToCreateInfo() const
	{
		VkRenderPassCreateInfo renderPassCreateInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO, nullptr };
		{
			renderPassCreateInfo.flags = 0;
			renderPassCreateInfo.attachmentCount = mAttachmentDescriptions.GetSize();
			renderPassCreateInfo.pAttachments = mAttachmentDescriptions.GetData();
			renderPassCreateInfo.subpassCount = mSubpassDescriptions.GetSize();
			renderPassCreateInfo.pSubpasses = mSubpassDescriptions.GetData();
			renderPassCreateInfo.dependencyCount = mSubpassDependencies.GetSize();
			renderPassCreateInfo.pDependencies = mSubpassDependencies.GetData();
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
