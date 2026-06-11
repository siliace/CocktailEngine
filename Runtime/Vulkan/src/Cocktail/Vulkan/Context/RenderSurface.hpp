#ifndef COCKTAIL_VULKAN_CONTEXT_RENDERSURFACE_HPP
#define COCKTAIL_VULKAN_CONTEXT_RENDERSURFACE_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include <Cocktail/Renderer/Context/RenderSurface.hpp>
#include <Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp>

#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/PresentationContext.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>
#include <Cocktail/Vulkan/Semaphore.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class RenderSurface : public Renderer::RenderSurface, public Observable
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo
		 * \param allocationCallbacks 
		 */
		RenderSurface(RenderDevice* renderDevice, const Renderer::RenderSurfaceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~RenderSurface() override;

		/**
		 * \brief
		 * \param timeout
		 * \param semaphore
		 * \param fence
		 * \return
		 */
		Optional<unsigned int> AcquireNextFramebuffer(Duration timeout, SharedPtr<Semaphore> semaphore, SharedPtr<Fence> fence) const;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief
		 * \return
		 */
		Extent2D<unsigned int> GetSize() const override;

		/**
		 * \brief
		 * \return
		 */
		PixelFormat GetColorFormat() const override;

		/**
		 * \brief
		 * \return
		 */
		PixelFormat DepthStencilFormat() const override;

		/**
		 * \brief
		 * \return
		 */
		Renderer::RasterizationSamples GetSamples() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsVsyncEnabled() const override;

		/**
		 * \brief 
		 * \param enable 
		 */
		void EnableVSync(bool enable) override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetBufferCount() const override;

		/**
		 * \brief
		 * \param framebufferIndex
		 * \return
		 */
		SharedPtr<Framebuffer> GetFramebuffer(unsigned int framebufferIndex) const;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<Swapchain> GetSwapchain() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkSurfaceKHR GetHandle() const;

	private:

		/**
		 * \brief 
		 * \param samples 
		 * \param depthStencilFormat 
		 */
		void CreateRenderPass(Renderer::RasterizationSamples samples, PixelFormat depthStencilFormat);


		/**
		 * \brief 
		 * \param size 
		 * \param enableVSync 
		 */
		void RecreateSwapchain(const Extent2D<unsigned int>& size, bool enableVSync);

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSurfaceKHR mHandle;
		PixelFormat mDepthStencilFormat;
		UniquePtr<PresentationContext> mPresentationContext;
		SharedPtr<RenderPass> mRenderPass;
		SharedPtr<Swapchain> mSwapchain;
		SharedPtr<Framebuffer> mFramebuffers[Framebuffer::MaxColorAttachmentCount];
		bool mVSyncEnable;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_RENDERSURFACE_HPP
