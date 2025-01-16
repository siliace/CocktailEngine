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

	void QueueSubmit::WaitSemaphore(const Ref<Semaphore>& semaphore, VkPipelineStageFlags pipelineWaitStages)
	{
		mWaitSemaphores[mWaitSemaphoreCount] = semaphore;
		mPipelineWaitStages[mWaitSemaphoreCount] = pipelineWaitStages;

		++mWaitSemaphoreCount;
	}

	void QueueSubmit::ExecuteCommandLists(unsigned int commandListCount, Ref<CommandList>* commandLists)
	{
		for (unsigned int i = 0; i < commandListCount; i++)
			mCommandLists[mCommandListCount + i] = commandLists[i];

		mCommandListCount += commandListCount;
	}

	void QueueSubmit::SignalSemaphore(const Ref<Semaphore>& semaphore)
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
