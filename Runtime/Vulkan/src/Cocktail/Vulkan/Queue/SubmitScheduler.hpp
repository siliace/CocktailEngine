#ifndef COCKTAIL_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
#define COCKTAIL_VULKAN_QUEUE_SUBMITSCHEDULER_HPP

#include <queue>

#include <Cocktail/Core/Utility/ObjectPool.hpp>

#include <Cocktail/Vulkan/Queue/QueueSubmitBatch.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Class managing batches submission and life-cycle in a deterministic order
	 */
	class SubmitScheduler : public Observable
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit SubmitScheduler(std::shared_ptr<RenderDevice> renderDevice);

		/**
		 * \brief Destructor
		 */
		~SubmitScheduler() override;
		
		/**
		 * \brief 
		 * \param queueType 
		 * \param queueIndex 
		 * \return 
		 */
		QueueSubmitBatch* ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex);

		/**
		 * \brief 
		 * \param batch 
		 * \param fence 
		 */
		void ConnectFence(QueueSubmitBatch* batch, std::shared_ptr<Fence> fence);

		/**
		 * \brief 
		 */
		void Submit();

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		bool mProcessingBatch;
		std::queue<QueueSubmitBatch*> mBatches;
		std::vector<QueueSubmitBatch*> mPending;
		std::vector<QueueSubmitBatch*> mTerminated;
		TlsObjectPool<QueueSubmitBatch> mBatchPool;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
