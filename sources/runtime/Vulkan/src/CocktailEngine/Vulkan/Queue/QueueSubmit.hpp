#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMIT_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMIT_HPP

#include <CocktailEngine/Vulkan/Semaphore.hpp>
#include <CocktailEngine/Vulkan/Command/CommandList.hpp>

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
		void WaitSemaphore(SharedPtr<Semaphore> semaphore, VkPipelineStageFlags pipelineWaitStages);

		/**
		 * \brief Add a command lists to be executed when this submit will be sent to a queue
		 * \param commandList
		 */
		void ExecuteCommandList(SharedPtr<CommandList> commandList);

		/**
		 * \brief Add a Semaphore to be waited when this submit will be sent to a queue
		 * \param semaphore
		 */
		void SignalSemaphore(SharedPtr<Semaphore> semaphore);

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
		SharedPtr<Semaphore> mWaitSemaphores[MaxWaitSemaphore];
		VkPipelineStageFlags mPipelineWaitStages[MaxWaitSemaphore];

		unsigned int mCommandListCount;
		SharedPtr<CommandList> mCommandLists[MaxCommandList];

		unsigned int mSignalSemaphoreCount;
		SharedPtr<Semaphore> mSignalSemaphores[MaxWaitSemaphore];
	};
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMIT_HPP
