#ifndef COCKTAILENGINE_VULKAN_CONTEXT_RENDERSURFACE_HPP
#define COCKTAILENGINE_VULKAN_CONTEXT_RENDERSURFACE_HPP

#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/Utility/Time/Duration.hpp>

#include <CocktailEngine/Renderer/Context/RenderSurface.hpp>
#include <CocktailEngine/Renderer/Context/RenderSurfaceCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Command/Fence.hpp>
#include <CocktailEngine/Vulkan/Context/PresentationContext.hpp>
#include <CocktailEngine/Vulkan/Framebuffer/Framebuffer.hpp>
#include <CocktailEngine/Vulkan/Semaphore.hpp>

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

#endif // COCKTAILENGINE_VULKAN_CONTEXT_RENDERSURFACE_HPP
