#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITTER_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITTER_HPP

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/Command/CommandList.hpp>
#include <CocktailEngine/Vulkan/Command/Fence.hpp>
#include <CocktailEngine/Vulkan/Queue/QueueSubmitDependency.hpp>
#include <CocktailEngine/Vulkan/Queue/SubmitScheduler.hpp>

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
		QueueSubmitter(RenderDevice* renderDevice, SubmitScheduler* scheduler, Renderer::CommandQueueType queueType, unsigned int queueIndex);

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
		void SignalFence(SharedPtr<Fence> fence);
		
		/**
		 * \brief Add a Semaphore to be signaled by the current submit of the current batch 
		 * \param semaphore 
		 */
		void SignalSemaphore(SharedPtr<Semaphore> semaphore);
		
		/**
		 * \brief 
		 * \param semaphore 
		 * \param waitStages 
		 */
		void WaitExternalSemaphore(SharedPtr<Semaphore> semaphore, VkPipelineStageFlags waitStages);

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

		RenderDevice* mRenderDevice;
		SubmitScheduler* mScheduler;
		Renderer::CommandQueueType mQueueType;
		unsigned int mQueueIndex;
		QueueSubmitBatch* mCurrentBatch;
		Flags<Renderer::CommandQueueType> mDependencies;
	};
}
#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITTER_HPP
