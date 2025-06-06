#ifndef COCKTAIL_VULKAN_QUEUE_QUEUESUBMITTER_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUESUBMITTER_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>
#include <Cocktail/Vulkan/Queue/QueueSubmitDependency.hpp>
#include <Cocktail/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief Class managing submits on a single queue
	 */
	class QueueSubmitter
	{
	public:

		/**
		 * \brief Constructor
		 * \param renderDevice
		 * \param scheduler 
		 * \param queueType 
		 * \param queueIndex 
		 */
		QueueSubmitter(std::shared_ptr<RenderDevice> renderDevice, SubmitScheduler* scheduler, Renderer::CommandQueueType queueType, unsigned int queueIndex);

		/**
		 * \brief 
		 */
		void NextSubmit() const;

		/**
		 * \brief 
		 * \return 
		 */
		QueueSubmitDependency CreateDependency(VkPipelineStageFlags waitDstStages) const;
		
		/**
		 * \brief 
		 * \param fence 
		 */
		void SignalFence(std::shared_ptr<Fence> fence);
		
		/**
		 * \brief Add a Semaphore to be signaled by the current submit of the current batch 
		 * \param semaphore 
		 */
		void SignalSemaphore(std::shared_ptr<Semaphore> semaphore);
		
		/**
		 * \brief 
		 * \param semaphore 
		 * \param waitStages 
		 */
		void WaitExternalSemaphore(std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags waitStages);

		/**
		 * \brief 
		 * \param dependency 
		 * \param waitedQueue
		 * \param selfDependent 
		 */
		void WaitDependency(const QueueSubmitDependency& dependency, Renderer::CommandQueueType waitedQueue, bool selfDependent);

		/**
		 * \brief 
		 * \param commandListCount 
		 * \param commandLists 
		 * \param fence 
		 */
		void ExecuteCommandList(unsigned int commandListCount, CommandList** commandLists, Fence* fence);

		/**
		 * \brief 
		 * \param commandQueue 
		 * \return 
		 */
		bool HasDependencyTo(Renderer::CommandQueueType commandQueue) const;
		
		/**
		 * \brief 
		 */
		void TerminateBatch();

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		SubmitScheduler* mScheduler;
		Renderer::CommandQueueType mQueueType;
		unsigned int mQueueIndex;
		QueueSubmitBatch* mCurrentBatch;
		Flags<Renderer::CommandQueueType> mDependencies;
	};
}
#endif // COCKTAIL_VULKAN_QUEUE_QUEUESUBMITTER_HPP
