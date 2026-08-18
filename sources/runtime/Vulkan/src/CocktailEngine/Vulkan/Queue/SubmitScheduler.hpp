#ifndef COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP

#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

#include <CocktailEngine/Vulkan/Queue/QueueSubmitBatch.hpp>

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
		explicit SubmitScheduler(RenderDevice* renderDevice);

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
		void ConnectFence(QueueSubmitBatch* batch, SharedPtr<Fence> fence);

		/**
		 * \brief 
		 */
		void Submit();

	private:

		RenderDevice* mRenderDevice;
		bool mProcessingBatch;
		Array<QueueSubmitBatch*> mBatches;
		Array<QueueSubmitBatch*> mPending;
		Array<QueueSubmitBatch*> mTerminated;
		TlsObjectPool<QueueSubmitBatch> mBatchPool;
	};
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
