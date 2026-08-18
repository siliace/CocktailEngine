#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	SubmitScheduler::SubmitScheduler(RenderDevice* renderDevice) :
		mRenderDevice(renderDevice),
		mProcessingBatch(false)
	{
		/// Nothing
	}

    SubmitScheduler::~SubmitScheduler()
    {
		assert(mBatches.IsEmpty());
		assert(mPending.IsEmpty());
    }

    QueueSubmitBatch* SubmitScheduler::ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex)
    {
		ObjectPoolUniquePtr<QueueSubmitBatch> batch = mBatchPool.Allocate(mRenderDevice, queueType, queueIndex);

		QueueSubmitBatch* batchPtr = batch.Get();
		mBatches.Add(Move(batch));

		return batchPtr;
	}

	void SubmitScheduler::ConnectFence(QueueSubmitBatch* batch, SharedPtr<Fence> fence)
	{
		batch->AssignFence(Move(fence));
		batch->Connect(batch->OnCompleted(), [this, batch = batch]() {
			if (mProcessingBatch)
				return;

			mPending.FindIndexIf([batch](const ObjectPoolUniquePtr<QueueSubmitBatch>& existingBatch) {
				return existingBatch.Get() == batch;
			}).Then([&](unsigned int index) {
				mProcessingBatch = true;

				for (unsigned int i = 0; i < index + 1; i++)
				{
					mPending[i]->MarkCompleted();
					mTerminated.Add(Move(mPending[i]));
				}

				mPending.SpliceInPlace(0, index + 1);
			});

			mProcessingBatch = false;
		});
	}

	void SubmitScheduler::Submit()
	{
		mTerminated.Clear();

		while (!mBatches.IsEmpty())
		{
			ObjectPoolUniquePtr<QueueSubmitBatch> batch = mBatches.PopFirst();

			batch->Submit();
			mPending.Add(Move(batch));
		}
	}
}
