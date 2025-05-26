#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Renderer/Context/FrameContext.hpp>

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Buffer/BufferAllocator.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>

namespace Ck::Vulkan
{
	class RenderContext;

	class FrameContext : public Renderer::FrameContext
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
		Renderer::Framebuffer* AcquireNextFramebuffer(Renderer::RenderSurface* renderSurface) override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo) override;

		/**
		 * \brief
		 * \param usage
		 * \param memoryType
		 * \return
		 */
		Renderer::BufferAllocator* GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType) override;

		/**
		 * \brief
		 * \param queue
		 */
		void Present(VkQueue queue);

		/**
		 * \brief
		 * \param name
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief
		 * \return
		 */
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::FrameToken GetToken() const override;

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
		std::unordered_map<BufferAllocatorKey, std::shared_ptr<BufferAllocator>> mBufferAllocators;
		bool mSubmitted;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
