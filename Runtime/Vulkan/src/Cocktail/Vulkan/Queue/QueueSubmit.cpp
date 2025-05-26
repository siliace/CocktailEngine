#include <Cocktail/Vulkan/Queue/QueueSubmit.hpp>

namespace Ck::Vulkan
{
	QueueSubmit::QueueSubmit() :
		mWaitSemaphoreCount(0),
		mWaitSemaphores{ nullptr },
		mPipelineWaitStages{ 0 },
		mCommandListCount(0),
		mCommandLists{ nullptr },
		mSignalSemaphoreCount(0),
		mSignalSemaphores{ nullptr }
	{
		/// Nothing
	}

	void QueueSubmit::WaitSemaphore(std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags pipelineWaitStages)
	{
		mWaitSemaphores[mWaitSemaphoreCount] = semaphore;
		mPipelineWaitStages[mWaitSemaphoreCount] = pipelineWaitStages;

		++mWaitSemaphoreCount;
	}

	void QueueSubmit::ExecuteCommandList(std::shared_ptr<CommandList> commandList)
	{
		mCommandLists[mCommandListCount] = std::move(commandList);
		mCommandListCount += 1;
	}

	void QueueSubmit::SignalSemaphore(std::shared_ptr<Semaphore> semaphore)
	{
		mSignalSemaphores[mSignalSemaphoreCount] = semaphore;

		++mSignalSemaphoreCount;
	}

	void QueueSubmit::Bake(VkSemaphore* waitSemaphores, VkPipelineStageFlags* pipelineWaitStages, VkCommandBuffer* commandBuffers, VkSemaphore* signalSemaphores) const
	{
		for (unsigned int i = 0; i < mWaitSemaphoreCount; i++)
		{
			waitSemaphores[i] = mWaitSemaphores[i]->GetHandle();
			pipelineWaitStages[i] = mPipelineWaitStages[i];
		}

		for (unsigned int i = 0; i < mCommandListCount; i++)
			commandBuffers[i] = mCommandLists[i]->GetHandle();

		for (unsigned int i = 0; i < mSignalSemaphoreCount; i++)
			signalSemaphores[i] = mSignalSemaphores[i]->GetHandle();
	}

	void QueueSubmit::MarkCompleted()
	{
		for (unsigned int i = 0; i < mWaitSemaphoreCount; i++)
		{
			mWaitSemaphores[i] = nullptr;
			mPipelineWaitStages[i] = 0;
		}
		mWaitSemaphoreCount = 0;

		for (unsigned int i = 0; i < mCommandListCount; i++)
		{
			mCommandLists[i]->MarkCompleted();
			mCommandLists[i] = nullptr;
		}
		mCommandListCount = 0;

		for (unsigned int i = 0; i < mSignalSemaphoreCount; i++)
			mSignalSemaphores[i] = nullptr;

		mSignalSemaphoreCount = 0;
	}

	unsigned int QueueSubmit::GetWaitSemaphoreCount() const
	{
		return mWaitSemaphoreCount;
	}

	unsigned int QueueSubmit::GetCommandListCount() const
	{
		return mCommandListCount;
	}

	unsigned int QueueSubmit::GetSignalSemaphoreCount() const
	{
		return mSignalSemaphoreCount;
	}
}
