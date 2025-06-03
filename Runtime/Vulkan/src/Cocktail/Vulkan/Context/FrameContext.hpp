#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>

namespace Ck::Vulkan
{
	class RenderContext;

	class FrameContext
	{
	public:

		/**
		 * \brief
		 * \param renderContext
		 * \param maxRenderSurfaceCount
		 * \param allocationCallbacks
		 */
		FrameContext(RenderContext* renderContext, unsigned int maxRenderSurfaceCount, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 */
		void Synchronize() const;

		/**
		 * \brief Clear any remaining resources from a previous frame
		 */
		void Reset();

		/**
		 * \brief
		 * \param renderSurface
		 * \return
		 */
		Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::RenderSurface* renderSurface);

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo);

		/**
		 * \brief
		 * \param queue
		 */
		void Present(VkQueue queue);

	private:

		using BufferAllocatorKey = CompositeKey<Renderer::BufferUsageFlags, Renderer::MemoryType>;

		/**
		 * \brief 
		 */
		struct AcquiredRenderSurface
		{
			/**
			 * \brief 
			 */
			std::shared_ptr<Swapchain> Swapchain;

			/**
			 * \brief 
			 */
			unsigned int SwapchainImageIndex;

			/**
			 * \brief 
			 */
			std::shared_ptr<Semaphore> ImageAvailable;

			/**
			 * \brief 
			 */
			std::shared_ptr<Semaphore> ImagePresentable;
		};

		RenderContext* mRenderContext;
		unsigned int mMaxRenderSurfaceCount;
		unsigned int mRenderSurfaceCount;
		std::shared_ptr<CommandListPool> mCommandListPool;
		std::vector<std::shared_ptr<Renderer::CommandList>> mCommandLists;
		std::shared_ptr<Fence> mFrameFence;
		std::unique_ptr<AcquiredRenderSurface[]> mAcquiredRenderSurfaces;
		bool mSubmitted;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
