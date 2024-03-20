#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan
{
	RenderSurface::RenderSurface(const Ref<RenderDevice>& renderDevice, const Renderer::RenderSurfaceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mSurface(VK_NULL_HANDLE),
		mFramebufferCount(0),
	    mVSyncEnable(false)
	{
		Window& window = *createInfo.Window;

		mDepthStencilFormat = createInfo.DepthStencilFormat;

		// Create the Surface we will render to
		// TODO: Handle Headless surfaces ? This extension implemented nowhere but it might one day change
		mSurface = WSI::CreateWindowSurface(renderDevice->GetInstanceHandle(), window, mAllocationCallbacks);

		// Create the PresentationContext used to manage swapchains creation
		PresentationContextCreateInfo presentationContextCreateInfo;
		presentationContextCreateInfo.Surface = mSurface;
		presentationContextCreateInfo.BufferCount = createInfo.BufferCount;
		presentationContextCreateInfo.ColorDepth = createInfo.ColorDepth;
		presentationContextCreateInfo.AlphaDepth = createInfo.AlphaDepth;
		presentationContextCreateInfo.ColorSpace = createInfo.ColorSpace;

		mPresentationContext = PresentationContext::New(mRenderDevice, presentationContextCreateInfo, mAllocationCallbacks);

		CreateRenderPass(createInfo.Samples, createInfo.DepthStencilFormat);

		// Create the initial swapchain
		RecreateSwapchain(window.GetSize(), createInfo.EnableVSync);

		// Destroy the surface when the window is closed
		Connect(window.OnCloseEvent(), [&](const WindowCloseEvent& event) {
			mOnClose.Emit();

			mSwapchain = nullptr;
		});

		// Recreate or destroy the swapchain where the window is resized
		Connect(window.OnResizedEvent(), [&](const WindowResizedEvent& event) {
			mOnResized.Emit(event.Size);

			bool minimized = false;
			minimized |= event.DisplayMode == WindowDisplayMode::Minimized;
			minimized |= event.Size.Width == 0;
			minimized |= event.Size.Height == 0;

			if (!minimized)
			{
				RecreateSwapchain(event.Size, mVSyncEnable);
			}
			else
			{
				mSwapchain = nullptr;
			}
		});

		// Redraw the surface when the window must be redrawn
		Connect(window.OnRedrawEvent(), [&](const WindowRedrawEvent&) {
			mOnRedraw.Emit();
		});
	}

	RenderSurface::~RenderSurface()
	{
		mSwapchain = nullptr;
		vkDestroySurfaceKHR(mRenderDevice->GetInstanceHandle(), mSurface, mAllocationCallbacks);
	}

	void RenderSurface::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SURFACE_KHR;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mSurface);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Ref<Renderer::RenderDevice> RenderSurface::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Extent2D<unsigned int> RenderSurface::GetSize() const
	{
		return mSize;
	}

	PixelFormat RenderSurface::GetColorFormat() const
	{
		return mPresentationContext->GetSurfaceFormat();
	}

	PixelFormat RenderSurface::DepthStencilFormat() const
	{
		return mDepthStencilFormat;
	}

	Renderer::RasterizationSamples RenderSurface::GetSamples() const
	{
		return mRenderPass->GetSamples();
	}

	bool RenderSurface::IsVsyncEnabled() const
	{
		return mVSyncEnable;
	}

	void RenderSurface::EnableVSync(bool enable)
	{
		RecreateSwapchain(mSize, enable);
	}

	unsigned RenderSurface::GetBufferCount() const
	{
		return mPresentationContext->GetBufferCount();
	}

	unsigned int RenderSurface::AcquireNextFramebuffer(Uint64 timeout, const Ref<Semaphore>& semaphore, const Ref<Fence>& fence)
	{
		unsigned int imageIndex;

		VkSemaphore semaphoreHandle = semaphore ? semaphore->GetHandle() : VK_NULL_HANDLE;
		VkFence fenceHandle = fence ? fence->GetHandle() : VK_NULL_HANDLE;
		COCKTAIL_VK_CHECK(vkAcquireNextImageKHR(mRenderDevice->GetHandle(), mSwapchain->GetHandle(), timeout, semaphoreHandle, fenceHandle, &imageIndex));

		return imageIndex;
	}

	Ref<Framebuffer> RenderSurface::GetFramebuffer(unsigned int framebufferIndex) const
	{
		if (framebufferIndex >= mFramebufferCount)
			return nullptr;

		return mFramebuffers[framebufferIndex];
	}

	Ref<Swapchain> RenderSurface::GetSwapchain() const
	{
		return mSwapchain;
	}

	Signal<>& RenderSurface::OnClose()
	{
		return mOnClose;
	}

	Signal<Extent2D<unsigned int>>& RenderSurface::OnResized()
	{
		return mOnResized;
	}

	Signal<>& RenderSurface::OnRedraw()
	{
		return mOnRedraw;
	}

	void RenderSurface::CreateRenderPass(Renderer::RasterizationSamples samples, PixelFormat depthStencilFormat)
	{
		Renderer::FramebufferLayout framebufferLayout;
		framebufferLayout.Samples = samples;
		framebufferLayout.ColorAttachmentCount = 1;
		framebufferLayout.ColorAttachmentFormats[0] = mPresentationContext->GetSurfaceFormat();
		framebufferLayout.DepthStencilAttachmentFormat = depthStencilFormat;

		RenderPassCreateInfo createInfo;
		createInfo.FramebufferLayout = framebufferLayout;
		createInfo.Presentable = true;

		mRenderPass = mRenderDevice->CreateRenderPass(createInfo);
	}

	void RenderSurface::RecreateSwapchain(const Extent2D<unsigned int>& size, bool enableVSync)
	{
		mSize = size;

		VkPresentModeKHR presentMode;
		if (enableVSync)
		{
			if (mPresentationContext->IsPresentationModeSupported(VK_PRESENT_MODE_MAILBOX_KHR))
			{
				mVSyncEnable = true;
				presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			}
			else if (mPresentationContext->IsPresentationModeSupported(VK_PRESENT_MODE_IMMEDIATE_KHR))
			{
				mVSyncEnable = true;
				presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
			else
			{
				mVSyncEnable = false;
				presentMode = VK_PRESENT_MODE_FIFO_KHR;
			}
		}
		else
		{
			mVSyncEnable = false;
			presentMode = VK_PRESENT_MODE_FIFO_KHR;
		}

		Ref<TextureView> depthStencilTextureView;
		if (mDepthStencilFormat != PixelFormat::Undefined())
		{
			RenderBufferCreateInfo renderBufferCreateInfo;
			renderBufferCreateInfo.Format = mDepthStencilFormat;
			renderBufferCreateInfo.Size = mSize;
			renderBufferCreateInfo.Samples = Renderer::RasterizationSamples::e1;

			Ref<RenderBuffer> depthStencilTexture = mRenderDevice->CreateRenderBuffer(renderBufferCreateInfo);

			Renderer::TextureViewCreateInfo viewCreateInfo;
			viewCreateInfo.Texture = depthStencilTexture;
			viewCreateInfo.Type = Renderer::TextureViewType::e2D;

			depthStencilTextureView = TextureView::Cast(mRenderDevice->CreateTextureView(viewCreateInfo));
		}

		mSwapchain = mPresentationContext->CreateSwapchain(mSize, presentMode, mSwapchain.Get());

		mFramebufferCount = mSwapchain->GetTextureCount();
		for (unsigned int i = 0; i < mFramebufferCount; i++)
		{
			Renderer::TextureViewCreateInfo viewCreateInfo;
			viewCreateInfo.Texture = mSwapchain->GetTexture(i);
			viewCreateInfo.Type = Renderer::TextureViewType::e2D;
			viewCreateInfo.Format = mPresentationContext->GetSurfaceFormat();

			Ref<TextureView> swapchainTextureView = TextureView::Cast(mRenderDevice->CreateTextureView(viewCreateInfo));

			Renderer::FramebufferCreateInfo framebufferCreateInfo;
			framebufferCreateInfo.Samples = mRenderPass->GetSamples();
			framebufferCreateInfo.ColorAttachments[0] = swapchainTextureView;
			framebufferCreateInfo.ColorAttachmentCount = 1;
			framebufferCreateInfo.DepthStencilAttachment = depthStencilTextureView;

			Ref<Renderer::Framebuffer> fb = mRenderDevice->CreateFramebuffer(mRenderPass, framebufferCreateInfo);
			mFramebuffers[i] = Framebuffer::Cast(fb);
		}
	}
}
