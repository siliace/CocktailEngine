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
	class SubmitScheduler : public Extends<SubmitScheduler, Object>, public Observable
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit SubmitScheduler(Ref<RenderDevice> renderDevice);

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
		void ConnectFence(QueueSubmitBatch* batch, const Ref<Fence>& fence);

		/**
		 * \brief 
		 */
		void Flush();

	private:

		Ref<RenderDevice> mRenderDevice;
		TlsObjectPool<QueueSubmitBatch> mBatchPool;
		std::queue<Ref<QueueSubmitBatch>> mBatches;
		std::vector<Ref<QueueSubmitBatch>> mPending;
		std::vector<Ref<QueueSubmitBatch>> mTerminated;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
