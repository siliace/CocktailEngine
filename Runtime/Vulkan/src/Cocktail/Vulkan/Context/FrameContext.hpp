#ifndef COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Buffer/BufferAllocator.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Framebuffer/Framebuffer.hpp>

namespace Ck::Vulkan
{
	class RenderContext;

	class FrameContext : public Inherit<FrameContext, Object, Renderer::FrameContext>
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
		Ref<Renderer::CommandList> CreateCommandList(const Renderer::CommandListCreateInfo& createInfo) override;

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

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
			Ref<Swapchain> Swapchain;

			/**
			 * \brief 
			 */
			unsigned int SwapchainImageIndex;

			/**
			 * \brief 
			 */
			Ref<Semaphore> ImageAvailable;

			/**
			 * \brief 
			 */
			Ref<Semaphore> ImagePresentable;
		};

		RenderContext* mRenderContext;
		unsigned int mMaxRenderSurfaceCount;
		unsigned int mRenderSurfaceCount;
		Ref<CommandListPool> mCommandListPool;
		std::vector<Ref<Renderer::CommandList>> mCommandLists;
		Ref<Fence> mFrameFence;
		std::unique_ptr<AcquiredRenderSurface[]> mAcquiredRenderSurfaces;
		std::unordered_map<BufferAllocatorKey, Ref<BufferAllocator>> mBufferAllocators;
		bool mSubmitted;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_FRAMECONTEXT_HPP