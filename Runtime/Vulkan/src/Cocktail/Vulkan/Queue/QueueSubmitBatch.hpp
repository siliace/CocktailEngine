#ifndef COCKTAIL_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP

#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/Queue/QueueSubmit.hpp>
#include <Cocktail/Vulkan/Queue/QueueSubmitDependency.hpp>

namespace Ck::Vulkan
{
	class Fence;

	/**
     * \brief
     */
	class QueueSubmitBatch : public Extends<QueueSubmitBatch, Object>, public Observable
	{
	public:

		/**
		 * \brief
		 */
		static constexpr unsigned int MaxSubmit = 16;

		/**
		 * \brief
		 */
		QueueSubmitBatch(Ref<RenderDevice> renderDevice, Renderer::CommandQueueType queue, unsigned int queueIndex);

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
		void WaitSemaphore(const Ref<Semaphore>& semaphore, VkPipelineStageFlags pipelineWaitStages);

		/**
		 * \brief
		 * \param commandListCount
		 * \param commandLists
		 */
		void ExecuteCommandLists(unsigned int commandListCount, Ref<CommandList>* commandLists);

		/**
		 * \brief
		 * \param semaphore
		 */
		void SignalSemaphore(const Ref<Semaphore>& semaphore);

		/**
		 * \brief
		 * \param fence
		 */
		void AssignFence(const Ref<Fence>& fence);

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

		Ref<RenderDevice> mRenderDevice;
		Renderer::CommandQueueType mQueue;
		unsigned int mQueueIndex;
		Ref<Fence> mFence;
		unsigned int mSubmitCount;
		QueueSubmit mSubmits[MaxSubmit];
		QueueSubmit* mCurrentSubmit;
		bool mCompleted;
		Signal<> mOnCompleted;
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
