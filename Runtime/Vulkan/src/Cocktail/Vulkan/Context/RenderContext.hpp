#ifndef COCKTAIL_VULKAN_CONTEXT_RENDERCONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_RENDERCONTEXT_HPP

#include <Cocktail/Renderer/Context/RenderContext.hpp>

#include <Cocktail/Vulkan/Context/FrameContext.hpp>
#include <Cocktail/Vulkan/Queue/QueueSubmitter.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class RenderContext : public Inherit<RenderContext, Object, Renderer::RenderContext>, public Observable
	{
	public:

		/**
		 * \brief
		 * \param renderDevice 
		 * \param createInfo 
		 */
		RenderContext(Ref<RenderDevice> renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~RenderContext() override;

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
		 * \param createInfo 
		 * \return 
		 */
		Ref<Renderer::CommandListPool> CreateCommandListPool(const Renderer::CommandListPoolCreateInfo& createInfo) override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::FrameContext* BeginFrame() override;

		/**
		 * \brief 
		 * \param queue 
		 */
		void SignalQueue(Renderer::CommandQueueType queue) override;

		/**
		 * \brief Add a Fence to be signaled by the current submit batch
		 * \param commandQueue The command queue where signal the Fence
		 * \param fence The fence to signal
		 */
		void SignalFence(Renderer::CommandQueueType commandQueue, const Ref<Fence>& fence);

		/**
		 * \brief Add a Semaphore to be signaled by the current submit
		 * \param commandQueue The command queue where signal the Semaphore
		 * \param semaphore The Semaphore to signal
		 */
		void SignalSemaphore(Renderer::CommandQueueType commandQueue, const Ref<Semaphore>& semaphore);

		/**
		 * \brief 
		 * \param waitingQueue 
		 * \param waitedQueue 
		 */
		void WaitQueue(Renderer::CommandQueueType waitingQueue, Renderer::CommandQueueType waitedQueue) override;

		/**
		 * \brief Add a Semaphore to be waited by the current submit
		 * \param commandQueue The command queue where wait the Semaphore
		 * \param semaphore The Semaphore to wait
		 * \param waitStages The pipeline stages blocked by the wait operation
		 */
		void WaitSemaphore(Renderer::CommandQueueType commandQueue, const Ref<Semaphore>& semaphore, VkPipelineStageFlags waitStages);

		/**
		 * \brief 
		 * \param commandQueue
		 * \param commandListCount 
		 * \param commandLists 
		 * \param fence 
		 */
		void ExecuteCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Ref<Renderer::CommandList>* commandLists, Ref<Renderer::Fence> fence) override;

		/**
		 * \brief 
		 */
		void EndFrame() override;

		/**
		 * \brief 
		 */
		void Flush() override;

		/**
		 * \brief 
		 */
		void Synchronize() override;

	private:

		Ref<RenderDevice> mRenderDevice;
		VkQueue mPresentationQueue;
		Ref<SubmitScheduler> mScheduler;
		EnumMap<Renderer::CommandQueueType, Ref<QueueSubmitter>> mSubmitters;
		unsigned int mFrameContextCount;
		unsigned int mCurrentFrameContext;
		Ref<FrameContext> mFrameContexts[Swapchain::MaxSwapchainTexture];
		Signal<Renderer::FrameContext*> mOnBeforeRedraw;
		Signal<Renderer::FrameContext*, const Renderer::Framebuffer*> mOnRedraw;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_RENDERCONTEXT_HPP
