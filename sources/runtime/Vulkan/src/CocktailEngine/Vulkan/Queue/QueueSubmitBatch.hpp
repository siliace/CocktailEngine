#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/Queue/QueueSubmit.hpp>
#include <CocktailEngine/Vulkan/Queue/QueueSubmitDependency.hpp>

namespace Ck::Vulkan
{
	class Fence;

	/**
     * \brief
     */
	class QueueSubmitBatch : public Observable
	{
	public:

		/**
		 * \brief
		 */
		static constexpr unsigned int MaxSubmit = 16;

		/**
		 * \brief
		 */
		QueueSubmitBatch(RenderDevice* renderDevice, Renderer::CommandQueueType queue, unsigned int queueIndex);

		/**
		 * \brief 
		 */
		~QueueSubmitBatch() override;

		/**
		 * \brief 
		 */
		void NextSubmit();

		/**
		 * \brief Create a dependency to the lastly closed submit
		 * \param waitDstStages 
		 * \return 
		 */
		QueueSubmitDependency CreateDependency(VkPipelineStageFlags waitDstStages);

		/**
		 * \brief
		 * \param semaphore
		 * \param pipelineWaitStages
		 */
		void WaitSemaphore(SharedPtr<Semaphore> semaphore, VkPipelineStageFlags pipelineWaitStages);

		/**
		 * \brief
		 * \param commandListCount
		 * \param commandLists
		 */
		void ExecuteCommandLists(unsigned int commandListCount, CommandList** commandLists);

		/**
		 * \brief
		 * \param semaphore
		 */
		void SignalSemaphore(SharedPtr<Semaphore> semaphore);

		/**
		 * \brief
		 * \param fence
		 */
		void AssignFence(SharedPtr<Fence> fence);

		/**
		 * \brief
		 * \return
		 */
		bool HasFence() const;

		/**
		 * \brief
		 * \return
		 */
		bool IsFull() const;

		/**
		 * \brief
		 */
		void Submit() const;

		/**
		 * \brief 
		 */
		void MarkCompleted();

		/**
		 * \brief 
		 * \return 
		 */
		Signal<>& OnCompleted();

	private:

		/**
		 * \brief 
		 */
		void PushSubmit();

		RenderDevice* mRenderDevice;
		Renderer::CommandQueueType mQueue;
		unsigned int mQueueIndex;
		SharedPtr<Fence> mFence;
		unsigned int mSubmitCount;
		QueueSubmit mSubmits[MaxSubmit];
		QueueSubmit* mCurrentSubmit;
		bool mCompleted;
		Signal<> mOnCompleted;
	};
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
