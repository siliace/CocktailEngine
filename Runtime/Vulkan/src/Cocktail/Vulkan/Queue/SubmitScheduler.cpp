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
		mBatches = {};
		mPending.clear();
		mTerminated.clear();
    }

    QueueSubmitBatch *SubmitScheduler::ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex)
    {
		std::shared_ptr<QueueSubmitBatch> batch = mBatchPool.Allocate(mRenderDevice, queueType, queueIndex);
		mBatches.push(batch);

		return batch.get();
	}

	void SubmitScheduler::ConnectFence(QueueSubmitBatch* batch, std::shared_ptr<Fence> fence)
	{
		batch->AssignFence(fence);
		batch->Connect(batch->OnCompleted(), [this, batch = batch]() {
			auto itBatch = std::find_if(mPending.begin(), mPending.end(), [&](std::shared_ptr<QueueSubmitBatch> pendingBatch) {
				return pendingBatch.get() == batch;
			});

			if (itBatch != mPending.end())
			{
				std::for_each(mPending.begin(), itBatch, [](std::shared_ptr<QueueSubmitBatch> pendingBatch) {
					pendingBatch->MarkCompleted();
				});

				mTerminated.insert(mTerminated.end(), 
					std::make_move_iterator(mPending.begin()), 
					std::make_move_iterator(std::next(itBatch))
				);

				mPending.erase(mPending.begin(), std::next(itBatch));
			}
		});
	}

	void SubmitScheduler::Flush()
	{
		mTerminated.clear();

		while (!mBatches.empty())
		{
			std::shared_ptr<QueueSubmitBatch> batch = std::move(mBatches.front());
			mBatches.pop();

			batch->Submit();
			mPending.emplace_back(std::move(batch));
		}
	}
}
