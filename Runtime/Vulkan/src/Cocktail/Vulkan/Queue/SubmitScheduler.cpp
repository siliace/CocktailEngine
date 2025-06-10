#include <Cocktail/Renderer/Command/CommandQueueType.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Queue/SubmitScheduler.hpp>

namespace Ck::Vulkan
{
	SubmitScheduler::SubmitScheduler(std::shared_ptr<RenderDevice> renderDevice) :
		mRenderDevice(std::move(renderDevice)),
		mProcessingBatch(false)
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
			if (mProcessingBatch)
				return;

			if (auto itBatch = std::find(mPending.begin(), mPending.end(), batch); itBatch != mPending.end())
			{
				mProcessingBatch = true;
				std::for_each(mPending.begin(), itBatch, [](QueueSubmitBatch* pendingBatch) {
					pendingBatch->MarkCompleted();
				});

				auto itNextBatch = std::next(itBatch);
				mTerminated.insert(mTerminated.end(), mPending.begin(), itNextBatch);
				mPending.erase(mPending.begin(), itNextBatch);
			}

			mProcessingBatch = false;
		});
	}

	void SubmitScheduler::Submit()
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
