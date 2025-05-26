#ifndef COCKTAIL_VULKAN_QUEUE_QUEUESUBMIT_HPP
#define COCKTAIL_VULKAN_QUEUE_QUEUESUBMIT_HPP

#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Describe a submit to a VkQueue
	 * This structure contains command lists to be executed and semaphores to be waited and signaled
	 */
	class QueueSubmit
	{
	public:

		static constexpr unsigned int MaxCommandList = 128;
		static constexpr unsigned int MaxWaitSemaphore = 8;
		static constexpr unsigned int MaxSignalSemaphore = 8;

		/**
		 * \brief
		 */
		QueueSubmit();

		/**
		 * \brief Add a Semaphore to be waited when this submit will be sent to a queue
		 * \param semaphore
		 * \param pipelineWaitStages
		 */
		void WaitSemaphore(std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags pipelineWaitStages);

		/**
		 * \brief Add a command lists to be executed when this submit will be sent to a queue
		 * \param commandList
		 */
		void ExecuteCommandList(std::shared_ptr<CommandList> commandList);

		/**
		 * \brief Add a Semaphore to be waited when this submit will be sent to a queue
		 * \param semaphore
		 */
		void SignalSemaphore(std::shared_ptr<Semaphore> semaphore);

		/**
		 * \brief
		 * \param waitSemaphores
		 * \param pipelineWaitStages
		 * \param commandBuffers
		 * \param signalSemaphores
		 */
		void Bake(VkSemaphore* waitSemaphores, VkPipelineStageFlags* pipelineWaitStages, VkCommandBuffer* commandBuffers, VkSemaphore* signalSemaphores) const;

		/**
		 * \brief 
		 */
		void MarkCompleted();

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetWaitSemaphoreCount() const;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetCommandListCount() const;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetSignalSemaphoreCount() const;

	private:

		unsigned int mWaitSemaphoreCount;
		std::shared_ptr<Semaphore> mWaitSemaphores[MaxWaitSemaphore];
		VkPipelineStageFlags mPipelineWaitStages[MaxWaitSemaphore];

		unsigned int mCommandListCount;
		std::shared_ptr<CommandList> mCommandLists[MaxCommandList];

		unsigned int mSignalSemaphoreCount;
		std::shared_ptr<Semaphore> mSignalSemaphores[MaxWaitSemaphore];
	};
}

#endif // COCKTAIL_VULKAN_QUEUE_QUEUESUBMIT_HPP
