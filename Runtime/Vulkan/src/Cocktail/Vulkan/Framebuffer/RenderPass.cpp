#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPassBuilder.hpp>

namespace Ck::Vulkan
{
	namespace
	{
		VkAttachmentReference2KHR CreateAttachmentDescription2(Array<VkAttachmentDescription2KHR>& attachmentDescriptions, const PixelFormat& format, Renderer::RasterizationSamples samples, VkAttachmentLoadOp loadOp, bool presentable)
		{
			const bool isMultisample = samples != Renderer::RasterizationSamples::e1;

			VkAttachmentDescription2KHR attachmentDescription{ VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR, nullptr };
			{
				attachmentDescription.format = ToVkType(format);
				attachmentDescription.samples = ToVkType(samples);
				attachmentDescription.loadOp = loadOp;
				attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

				if (format.IsStencil() || format.IsDepthStencil())
				{
					attachmentDescription.stencilLoadOp = isMultisample ? VK_ATTACHMENT_LOAD_OP_DONT_CARE : loadOp;
					attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
				}
				else
				{
					attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				}

				switch (loadOp)
				{
				case VK_ATTACHMENT_LOAD_OP_LOAD:
					{
						if (format.IsColor())
						{
							if (isMultisample)
							{
								attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
							}
							else
							{
								attachmentDescription.initialLayout = presentable ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
							}
						}
						else
						{
							attachmentDescription.initialLayout = isMultisample ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						}
					}
					break;

				case VK_ATTACHMENT_LOAD_OP_CLEAR:
				case VK_ATTACHMENT_LOAD_OP_DONT_CARE:
					attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					break;
				}

				if (format.IsColor())
				{
					if (isMultisample)
					{
						attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					}
					else
					{
						attachmentDescription.finalLayout = presentable ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					}
				}
				else
				{
					attachmentDescription.finalLayout = isMultisample ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				}
			}

			VkAttachmentReference2KHR attachmentReference{ VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR, nullptr };
			attachmentReference.attachment = attachmentDescriptions.GetSize();
			if (format.IsColor())
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachmentReference.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}
			else
			{
				attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachmentReference.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
			}

			attachmentDescriptions.Add(attachmentDescription);

			return attachmentReference;
		}
	}

	RenderPass::RenderPass(std::shared_ptr<RenderDevice> renderDevice, const RenderPassCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks)
	{
		mColorAttachmentCount = createInfo.FramebufferLayout.ColorAttachmentCount;
		mSamples = createInfo.FramebufferLayout.Samples;
		mDepthResolveMode = createInfo.DepthResolveMode;
		mStencilResolveMode = createInfo.StencilResolveMode;

		for (Renderer::RenderPassMode renderPassMode : Enum<Renderer::RenderPassMode>::Values)
		{
			VkAttachmentLoadOp loadOp = {};
			switch (renderPassMode)
			{
			case Renderer::RenderPassMode::Initial:
				loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				break;

			case Renderer::RenderPassMode::Clear:
				loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				break;

			case Renderer::RenderPassMode::Load:
				loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
				break;
			}

			if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::RenderPass2))
			{
				Array<VkAttachmentDescription2KHR> attachmentDescriptions;
				Array<VkAttachmentReference2KHR> multisampleColorAttachmentReferences;
				Array<VkAttachmentReference2KHR> colorAttachmentReferences;
				VkAttachmentReference2KHR multiSampleDephtStencilAttachmentReference;
				VkAttachmentReference2KHR dephtStencilAttachmentReference;

				mResolveDepthStencil = mRenderDevice->IsFeatureSupported(RenderDeviceFeature::RenderPassDepthStencilResolve);
				if (mResolveDepthStencil)
				{
					VkPhysicalDeviceProperties2KHR physicalDeviceProperties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR, nullptr };
					VkPhysicalDeviceDepthStencilResolvePropertiesKHR depthStencilResolveProperties{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DEPTH_STENCIL_RESOLVE_PROPERTIES_KHR, nullptr };

					Chain(physicalDeviceProperties, depthStencilResolveProperties);
					vkGetPhysicalDeviceProperties2KHR(mRenderDevice->GetPhysicalDeviceHandle(), &physicalDeviceProperties);

					mResolveDepthStencil &= depthStencilResolveProperties.independentResolve == VK_TRUE || mDepthResolveMode == mStencilResolveMode;
					mResolveDepthStencil &= (depthStencilResolveProperties.supportedDepthResolveModes & ToVkType(mDepthResolveMode)) > 0;
					mResolveDepthStencil &= (depthStencilResolveProperties.supportedStencilResolveModes & ToVkType(mStencilResolveMode)) > 0;
				}

				for (unsigned int i = 0; i < mColorAttachmentCount; i++)
				{
					const PixelFormat& depthStencilFormat = createInfo.FramebufferLayout.ColorAttachmentFormats[i];
					assert(depthStencilFormat != PixelFormat::Undefined());

					if (mSamples != Renderer::RasterizationSamples::e1)
					{
						multisampleColorAttachmentReferences.Add(
							CreateAttachmentDescription2(attachmentDescriptions, depthStencilFormat, mSamples, loadOp, false)
						);
					}

					colorAttachmentReferences.Add(
						CreateAttachmentDescription2(attachmentDescriptions, depthStencilFormat, Renderer::RasterizationSamples::e1, loadOp, createInfo.Presentable)
					);
				}

				const PixelFormat& depthStencilFormat = createInfo.FramebufferLayout.DepthStencilAttachmentFormat;
				if (depthStencilFormat != PixelFormat::Undefined())
				{
					if (mSamples != Renderer::RasterizationSamples::e1)
						multiSampleDephtStencilAttachmentReference = CreateAttachmentDescription2(attachmentDescriptions, depthStencilFormat, mSamples, loadOp, false);

					dephtStencilAttachmentReference = CreateAttachmentDescription2(attachmentDescriptions, depthStencilFormat, Renderer::RasterizationSamples::e1, loadOp, false);
				}

				VkSubpassDescriptionDepthStencilResolveKHR resolveDepthStencil{ VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_DEPTH_STENCIL_RESOLVE_KHR, nullptr };
				{
					resolveDepthStencil.depthResolveMode = depthStencilFormat.IsDepth() || depthStencilFormat.IsDepthStencil() ? ToVkType(createInfo.DepthResolveMode) : VK_RESOLVE_MODE_NONE_KHR;
					resolveDepthStencil.stencilResolveMode = depthStencilFormat.IsStencil() || depthStencilFormat.IsDepthStencil() ? ToVkType(createInfo.StencilResolveMode) : VK_RESOLVE_MODE_NONE_KHR;
					resolveDepthStencil.pDepthStencilResolveAttachment = &dephtStencilAttachmentReference;
				}

				VkSubpassDescription2KHR subpassDescription{ VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2_KHR, nullptr };
				{
					subpassDescription.flags = 0;
					subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
					subpassDescription.inputAttachmentCount = 0;
					subpassDescription.pInputAttachments = nullptr;
					subpassDescription.colorAttachmentCount = colorAttachmentReferences.GetSize();
					if (mSamples != Renderer::RasterizationSamples::e1)
					{
						if (mResolveDepthStencil)
							Chain(subpassDescription, resolveDepthStencil);

						subpassDescription.pColorAttachments = multisampleColorAttachmentReferences.GetData();
						subpassDescription.pResolveAttachments = colorAttachmentReferences.GetData();
						subpassDescription.pDepthStencilAttachment = &multiSampleDephtStencilAttachmentReference;
					}
					else
					{
						subpassDescription.pColorAttachments = colorAttachmentReferences.GetData();
						subpassDescription.pResolveAttachments = nullptr;
						subpassDescription.pDepthStencilAttachment = &dephtStencilAttachmentReference;
					}

					subpassDescription.preserveAttachmentCount = 0;
					subpassDescription.pPreserveAttachments = nullptr;
				}

				VkRenderPassCreateInfo2KHR vkCreateInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2_KHR, nullptr };
				{
					vkCreateInfo.flags = 0;
					vkCreateInfo.attachmentCount = attachmentDescriptions.GetSize();
					vkCreateInfo.pAttachments = attachmentDescriptions.GetData();
					vkCreateInfo.subpassCount = 1;
					vkCreateInfo.pSubpasses = &subpassDescription;
					vkCreateInfo.dependencyCount = 0;
					vkCreateInfo.pDependencies = nullptr;
					vkCreateInfo.correlatedViewMaskCount = 0;
					vkCreateInfo.pCorrelatedViewMasks = nullptr;
				}

				COCKTAIL_VK_CHECK(vkCreateRenderPass2KHR(mRenderDevice->GetHandle(), &vkCreateInfo, allocationCallbacks, &mHandle[renderPassMode]));
			}
			else
			{
				mResolveDepthStencil = false;

				RenderPassBuilder builder(mSamples);

				for (unsigned int i = 0; i < createInfo.FramebufferLayout.ColorAttachmentCount; i++)
				{
					PixelFormat colorFormat = createInfo.FramebufferLayout.ColorAttachmentFormats[i];
					builder.CreateAttachment(colorFormat, loadOp, VK_ATTACHMENT_STORE_OP_STORE, createInfo.Presentable);
				}

				PixelFormat depthStencilFormat = createInfo.FramebufferLayout.DepthStencilAttachmentFormat;
				if (depthStencilFormat != PixelFormat::Undefined())
					builder.CreateAttachment(depthStencilFormat, loadOp, VK_ATTACHMENT_STORE_OP_STORE, false);

				builder.CreateSubpass(VK_PIPELINE_BIND_POINT_GRAPHICS);
				builder.CreateDependency();

				VkRenderPassCreateInfo renderPassCreateInfo = builder.ToCreateInfo();
				COCKTAIL_VK_CHECK(vkCreateRenderPass(mRenderDevice->GetHandle(), &renderPassCreateInfo, mAllocationCallbacks, &mHandle[renderPassMode]));

			}
		}
	}

	RenderPass::~RenderPass()
	{
		for (Renderer::RenderPassMode mode : Enum<Renderer::RenderPassMode>::Values)
			vkDestroyRenderPass(mRenderDevice->GetHandle(), mHandle[mode], mAllocationCallbacks);
	}
	
	void RenderPass::SetObjectName(const char* name) const
	{
		for (Renderer::RenderPassMode mode : Enum<Renderer::RenderPassMode>::Values)
		{
			VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
			{
				objectNameInfo.objectType = VK_OBJECT_TYPE_RENDER_PASS;
				objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle[mode]);
				objectNameInfo.pObjectName = name;
			}

			COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
		}
	}

	std::shared_ptr<Renderer::RenderDevice> RenderPass::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	bool RenderPass::ResolveDepthStencil() const
	{
		return mResolveDepthStencil;
	}

	unsigned int RenderPass::GetColorAttachmentCount() const
	{
		return mColorAttachmentCount;
	}

	Renderer::RasterizationSamples RenderPass::GetSamples() const
	{
		return mSamples;
	}

	Renderer::ResolveMode RenderPass::GetDepthResolveMode() const
	{
		return mDepthResolveMode;
	}

	Renderer::ResolveMode RenderPass::GetStencilResolveMode() const
	{
		return mStencilResolveMode;
	}

	VkRenderPass RenderPass::GetHandle(Renderer::RenderPassMode mode) const
	{
		return mHandle[mode];
	}
}
