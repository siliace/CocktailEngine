#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	SubmitScheduler::SubmitScheduler(std::shared_ptr<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice))
	{
		/// Nothing
	}

    SubmitScheduler::~SubmitScheduler()
    {
		assert(mBatches.empty());
		assert(mPending.empty());

		for (QueueSubmitBatch* terminated : mTerminated)
			mBatchPool.Recycle(terminated);
    }

    QueueSubmitBatch* SubmitScheduler::ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex)
    {
		QueueSubmitBatch* batch = mBatchPool.AllocateUnsafe(mRenderDevice, queueType, queueIndex);
		mBatches.push(batch);

		return batch;
	}

	void SubmitScheduler::ConnectFence(QueueSubmitBatch* batch, std::shared_ptr<Fence> fence)
	{
		batch->AssignFence(std::move(fence));
		batch->Connect(batch->OnCompleted(), [this, batch = batch]() {
			auto itBatch = std::find(mPending.begin(), mPending.end(), batch);

			if (itBatch != mPending.end())
			{
				std::for_each(mPending.begin(), itBatch, [](QueueSubmitBatch* pendingBatch) {
					pendingBatch->MarkCompleted();
				});

				mTerminated.insert(mTerminated.end(), mPending.begin(), std::next(itBatch));
				mPending.erase(mPending.begin(), std::next(itBatch));
			}
		});
	}

	void SubmitScheduler::Flush()
	{
		for (QueueSubmitBatch* terminated : mTerminated)
			mBatchPool.Recycle(terminated);

		mTerminated.clear();

		while (!mBatches.empty())
		{
			QueueSubmitBatch* batch = mBatches.front();
			mBatches.pop();

			batch->Submit();
			mPending.emplace_back(batch);
		}
	}
}
