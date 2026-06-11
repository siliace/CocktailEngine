#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Queue/QueueSubmitter.hpp>
#include <Cocktail/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	QueueSubmitter::QueueSubmitter(RenderDevice* renderDevice, SubmitScheduler* scheduler, Renderer::CommandQueueType queueType, unsigned int queueIndex) :
		mRenderDevice(renderDevice),
		mScheduler(scheduler),
		mQueueType(queueType),
		mQueueIndex(queueIndex),
		mCurrentBatch(nullptr)
	{
		/// Nothing
	}

	void QueueSubmitter::NextSubmit() const
	{
		if (mCurrentBatch)
			mCurrentBatch->NextSubmit();
	}

	QueueSubmitDependency QueueSubmitter::CreateDependency(VkPipelineStageFlags waitDstStages) const
	{
		if (!mCurrentBatch)
			return { nullptr, 0 };

		return mCurrentBatch->CreateDependency(waitDstStages);
	}

	void QueueSubmitter::SignalFence(SharedPtr<Fence> fence)
	{
		if (!mCurrentBatch || mCurrentBatch && mCurrentBatch->HasFence())
			mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);

		mScheduler->ConnectFence(mCurrentBatch, fence);
	}

	void QueueSubmitter::SignalSemaphore(SharedPtr<Semaphore> semaphore)
	{
		if (!mCurrentBatch)
			mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);

		mCurrentBatch->SignalSemaphore(semaphore);
	}

	void QueueSubmitter::WaitExternalSemaphore(SharedPtr<Semaphore> semaphore, VkPipelineStageFlags waitStages)
	{
		if (!mCurrentBatch)
			mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);

		mCurrentBatch->WaitSemaphore(semaphore, waitStages);
	}

	void QueueSubmitter::WaitDependency(const QueueSubmitDependency& dependency, Renderer::CommandQueueType waitedQueue, bool selfDependent)
	{
		if (!mCurrentBatch || selfDependent)
			mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);

		mCurrentBatch->WaitSemaphore(dependency.WaitSemaphore, dependency.WaitDstStages);		
		mDependencies |= waitedQueue;
	}

	void QueueSubmitter::ExecuteCommandList(unsigned int commandListCount, CommandList** commandLists, Fence* fence)
	{
		if (mCurrentBatch && fence)
		{
			if (mCurrentBatch->HasFence())
				mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);
		}
		else if (!mCurrentBatch)
		{
			mCurrentBatch = mScheduler->ScheduleBatch(mQueueType, mQueueIndex);
		}

		if (fence)
			mScheduler->ConnectFence(mCurrentBatch, fence->AsShared());

		mCurrentBatch->ExecuteCommandLists(commandListCount, commandLists);
	}

	bool QueueSubmitter::HasDependencyTo(Renderer::CommandQueueType commandQueue) const
	{
		return mDependencies & commandQueue;
	}

	void QueueSubmitter::TerminateBatch()
	{
		mCurrentBatch = nullptr;
	}
}
