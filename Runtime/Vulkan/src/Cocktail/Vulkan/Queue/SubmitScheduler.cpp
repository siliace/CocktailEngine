#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	SubmitScheduler::SubmitScheduler(RenderDevice* renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mProcessingBatch(false)
	{
		/// Nothing
	}

    SubmitScheduler::~SubmitScheduler()
    {
		assert(mBatches.IsEmpty());
		assert(mPending.IsEmpty());

		for (QueueSubmitBatch* terminated : mTerminated)
			mBatchPool.Recycle(terminated);
    }

    QueueSubmitBatch* SubmitScheduler::ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex)
    {
		QueueSubmitBatch* batch = mBatchPool.AllocateUnsafe(mRenderDevice, queueType, queueIndex);
		mBatches.Add(batch);

		return batch;
	}

	void SubmitScheduler::ConnectFence(QueueSubmitBatch* batch, std::shared_ptr<Fence> fence)
	{
		batch->AssignFence(std::move(fence));
		batch->Connect(batch->OnCompleted(), [this, batch = batch]() {
			if (mProcessingBatch)
				return;

			mPending.FindIndex(batch).Then([&](unsigned int index) {
				mProcessingBatch = true;

				for (unsigned int i = 0; i < index + 1; i++)
				{
					mPending[i]->MarkCompleted();
					mTerminated.Add(mPending[i]);
				}

				mPending.SliceInPlace(0, index + 1);
			});

			mProcessingBatch = false;
		});
	}

	void SubmitScheduler::Submit()
	{
		for (QueueSubmitBatch* terminated : mTerminated)
			mBatchPool.Recycle(terminated);

		mTerminated.Clear();

		while (!mBatches.IsEmpty())
		{
			QueueSubmitBatch* batch = mBatches.PopFirst();

			batch->Submit();
			mPending.Add(batch);
		}
	}
}
