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
		void Flush();

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		TlsObjectPool<QueueSubmitBatch> mBatchPool;
		std::queue<std::shared_ptr<QueueSubmitBatch>> mBatches;
		std::vector<std::shared_ptr<QueueSubmitBatch>> mPending;
		std::vector<std::shared_ptr<QueueSubmitBatch>> mTerminated;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
