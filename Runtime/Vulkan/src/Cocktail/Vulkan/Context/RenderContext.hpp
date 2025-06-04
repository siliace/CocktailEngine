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
	class RenderContext : public Renderer::RenderContext, public Observable
	{
	public:

		/**
		 * \brief
		 * \param renderDevice 
		 * \param createInfo 
		 */
		RenderContext(std::shared_ptr<RenderDevice> renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		std::shared_ptr<Renderer::CommandListPool> CreateCommandListPool(const Renderer::CommandListPoolCreateInfo& createInfo) override;

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
		void SignalFence(Renderer::CommandQueueType commandQueue, std::shared_ptr<Fence> fence);

		/**
		 * \brief Add a Semaphore to be signaled by the current submit
		 * \param commandQueue The command queue where signal the Semaphore
		 * \param semaphore The Semaphore to signal
		 */
		void SignalSemaphore(Renderer::CommandQueueType commandQueue, std::shared_ptr<Semaphore> semaphore);

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
		void WaitSemaphore(Renderer::CommandQueueType commandQueue, std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags waitStages);

		/**
		 * \brief 
		 * \param commandQueue
		 * \param commandListCount 
		 * \param commandLists 
		 * \param fence 
		 */
		void ExecuteCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Renderer::CommandList** commandLists, Renderer::Fence* fence) override;

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

		std::shared_ptr<RenderDevice> mRenderDevice;
		VkQueue mPresentationQueue;
		std::unique_ptr<SubmitScheduler> mScheduler;
		EnumMap<Renderer::CommandQueueType, std::unique_ptr<QueueSubmitter>> mSubmitters;
		unsigned int mCurrentFrameContext;
		std::vector<std::unique_ptr<FrameContext>> mFrameContexts;
		Signal<Renderer::FrameContext*> mOnBeforeRedraw;
		Signal<Renderer::FrameContext*, const Renderer::Framebuffer*> mOnRedraw;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_RENDERCONTEXT_HPP
