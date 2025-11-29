#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Context/Swapchain.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderBuffer.hpp>
#include <Cocktail/Vulkan/Framebuffer/RenderPass.hpp>
#include <Cocktail/Vulkan/Texture/TextureView.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan
{
	RenderSurface::RenderSurface(RenderDevice* renderDevice, const Renderer::RenderSurfaceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
	    mVSyncEnable(false)
	{
		mDepthStencilFormat = createInfo.DepthStencilFormat;

		// Create the Surface we will render to
		Window& window = *createInfo.Window;
		mHandle = WSI::CreateWindowSurface(mRenderDevice->GetInstanceHandle(), window, mAllocationCallbacks);

		// Create the PresentationContext used to manage swapchains creation
		mPresentationContext = MakeUnique<PresentationContext>(mRenderDevice, this, createInfo.BufferCount, createInfo.ColorDepth, createInfo.AlphaDepth, createInfo.ColorSpace);

		CreateRenderPass(createInfo.Samples, createInfo.DepthStencilFormat);

		// Create the initial swapchain
		RecreateSwapchain(window.GetSize(), createInfo.EnableVSync);

		// Destroy the surface when the window is closed
		Connect(window.OnCloseEvent(), [&](const WindowCloseEvent& event) {
			vkDeviceWaitIdle(mRenderDevice->GetHandle());
			mSwapchain = nullptr;
		});

		// Recreate or destroy the swapchain where the window is resized
		Connect(window.OnResizedEvent(), [&](const WindowResizedEvent& event) {
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
	}

	RenderSurface::~RenderSurface()
	{
		mSwapchain = nullptr;
		vkDestroySurfaceKHR(mRenderDevice->GetInstanceHandle(), mHandle, mAllocationCallbacks);
	}

	Optional<unsigned int> RenderSurface::AcquireNextFramebuffer(Duration timeout, std::shared_ptr<Semaphore> semaphore, std::shared_ptr<Fence> fence) const
	{
		if (!mSwapchain)
			return Optional<unsigned int>::Empty();

		unsigned int imageIndex;
		VkSemaphore semaphoreHandle = semaphore ? semaphore->GetHandle() : VK_NULL_HANDLE;
		VkFence fenceHandle = fence ? fence->GetHandle() : VK_NULL_HANDLE;
		COCKTAIL_VK_CHECK(vkAcquireNextImageKHR(mRenderDevice->GetHandle(), mSwapchain->GetHandle(), timeout.GetCount(TimeUnit::Nanoseconds()), semaphoreHandle, fenceHandle, &imageIndex));

		return Optional<unsigned int>::Of(imageIndex);
	}

	void RenderSurface::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SURFACE_KHR;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* RenderSurface::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Extent2D<unsigned int> RenderSurface::GetSize() const
	{
		return mSwapchain->GetSize();
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
		RecreateSwapchain(GetSize(), enable);
	}

	unsigned int RenderSurface::GetBufferCount() const
	{
		return mPresentationContext->GetBufferCount();
	}

	std::shared_ptr<Framebuffer> RenderSurface::GetFramebuffer(unsigned int framebufferIndex) const
	{
		if (framebufferIndex >= GetBufferCount())
			return nullptr;

		return mFramebuffers[framebufferIndex];
	}

	std::shared_ptr<Swapchain> RenderSurface::GetSwapchain() const
	{
		return mSwapchain;
	}

	VkSurfaceKHR RenderSurface::GetHandle() const
	{
		return mHandle;
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
		createInfo.DepthResolveMode = Renderer::ResolveMode::Average;
		createInfo.StencilResolveMode = Renderer::ResolveMode::SampleZero;
		createInfo.Presentable = true;

		mRenderPass = mRenderDevice->CreateRenderPass(createInfo);
	}

	void RenderSurface::RecreateSwapchain(const Extent2D<unsigned int>& size, bool enableVSync)
 	{
		VkPresentModeKHR presentMode;
		if (enableVSync)
		{
			mVSyncEnable = true;
			presentMode = VK_PRESENT_MODE_FIFO_KHR;
		}
		else 
		{
			if (mPresentationContext->IsPresentationModeSupported(VK_PRESENT_MODE_IMMEDIATE_KHR))
			{
				mVSyncEnable = false;
				presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
			else
			{
				CK_LOG(VulkanLogCategory, LogLevel::Warning, CK_TEXT("RenderSurface does not support present mode immediate, vsync cannot be disabled"));

				mVSyncEnable = true;
				presentMode = VK_PRESENT_MODE_FIFO_KHR;
			}
		}

		mSwapchain = mPresentationContext->CreateSwapchain(size, presentMode, mSwapchain.get());

		std::shared_ptr<TextureView> depthStencilTextureView;
		if (mDepthStencilFormat != PixelFormat::Undefined())
		{
			RenderBufferCreateInfo renderBufferCreateInfo;
			renderBufferCreateInfo.Format = mDepthStencilFormat;
			renderBufferCreateInfo.Size = mSwapchain->GetSize();
			renderBufferCreateInfo.Samples = Renderer::RasterizationSamples::e1;

			std::shared_ptr<RenderBuffer> depthStencilTexture = mRenderDevice->CreateRenderBuffer(renderBufferCreateInfo);

			Renderer::TextureViewCreateInfo viewCreateInfo;
			viewCreateInfo.Texture = depthStencilTexture;
			viewCreateInfo.Type = Renderer::TextureViewType::e2D;

			depthStencilTextureView = std::static_pointer_cast<TextureView>(
				mRenderDevice->CreateTextureView(viewCreateInfo)
			);
		}

		for (unsigned int i = 0; i < mSwapchain->GetTextureCount(); i++)
		{
			Renderer::TextureViewCreateInfo viewCreateInfo;
			viewCreateInfo.Texture = mSwapchain->GetTexture(i);
			viewCreateInfo.Type = Renderer::TextureViewType::e2D;
			viewCreateInfo.Format = mPresentationContext->GetSurfaceFormat();

			std::shared_ptr<TextureView> swapchainTextureView = std::static_pointer_cast<TextureView>(
				mRenderDevice->CreateTextureView(viewCreateInfo)
			);

			Renderer::FramebufferCreateInfo framebufferCreateInfo;
			framebufferCreateInfo.Samples = mRenderPass->GetSamples();
			framebufferCreateInfo.ColorAttachments[0] = swapchainTextureView;
			framebufferCreateInfo.ColorAttachmentCount = 1;
			framebufferCreateInfo.DepthStencilAttachment = depthStencilTextureView;

			mFramebuffers[i] = std::static_pointer_cast<Framebuffer>(
				mRenderDevice->CreateFramebuffer(mRenderPass, framebufferCreateInfo)
			);
		}
	}
}
