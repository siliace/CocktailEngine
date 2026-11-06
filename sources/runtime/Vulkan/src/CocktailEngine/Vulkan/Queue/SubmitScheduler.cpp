#include <CocktailEngine/Core/Utility/ValueGuard.hpp>

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>
#include <CocktailEngine/Renderer/Command/Synchronization/FenceCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Command/Fence.hpp>
#include <CocktailEngine/Vulkan/Queue/SubmitScheduler.hpp>
#include <CocktailEngine/Vulkan/RenderDevice.hpp>

namespace Ck::Vulkan
{
    namespace
    {
        /**
         * \brief Tells whether two batches are submitted to the same VkQueue
         *
         * The queue family alone is not enough: two batches of the same family submitted to
         * different queue indices run on different queues, and are therefore as unordered
         * against one another as batches of different families.
         */
        bool IsOnSameQueue(const QueueSubmitBatch* lhs, const QueueSubmitBatch* rhs)
        {
            return lhs->GetQueueType() == rhs->GetQueueType() && lhs->GetQueueIndex() == rhs->GetQueueIndex();
        }
    }

    SubmitScheduler::SubmitScheduler(RenderDevice* renderDevice) :
        mRenderDevice(renderDevice),
        mReclaiming(false)
    {
        assert(mRenderDevice);

        mBatches.Reserve(16);
        mPending.Reserve(16);
        mTerminated.Reserve(16);
    }

    SubmitScheduler::~SubmitScheduler()
    {
        assert(mBatches.IsEmpty());

        Synchronize();
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
        assert(batch);

        // Assigned first: it throws when the batch already carries a fence, and it has to do
        // so before a second callback is installed on the same batch.
        batch->AssignFence(Move(fence));

        batch->Connect(batch->OnCompleted(), [this, batch] {
            assert(batch);

            if (mReclaiming)
                return;

            // Counted before anything is touched. The reservation below is the only step of this
            // method able to fail, and it has to fail while the scheduler is still untouched.
            unsigned int reclaimedCount = 0;
            bool isPending = false;
            for (unsigned int i = 0; i < mPending.GetSize() && !isPending; i++)
            {
                if (!IsOnSameQueue(mPending[i].Get(), batch))
                    continue;

                ++reclaimedCount;
                isPending = mPending[i].Get() == batch;
            }

            if (!isPending)
                return;

            mTerminated.Reserve(mTerminated.GetSize() + reclaimedCount);

            ValueGuard guard(mReclaiming, true);

            // One pass doing both halves of the work: the batches up to and including the one
            // that completed leave for mTerminated, and the ones left behind are packed back
            // down. Packing rather than erasing matters because the batches kept are not a
            // suffix of the list, those belonging to other queues sit in between.
            unsigned int keptCount = 0;
            bool reclaimed = false;
            for (unsigned int i = 0; i < mPending.GetSize(); i++)
            {
                ObjectPoolUniquePtr<QueueSubmitBatch>& candidate = mPending[i];

                if (!reclaimed && IsOnSameQueue(candidate.Get(), batch))
                {
                    reclaimed = candidate.Get() == batch;

                    // Waits on the batch's own fence, which re-enters this method through the
                    // callback of the batch. The guard above is what sends that nested call
                    // straight back out, leaving the list below in the inconsistent state the
                    // loop is in the middle of creating.
                    candidate->MarkCompleted();
                    mTerminated.Add(Move(candidate));

                    continue;
                }

                if (keptCount != i)
                    mPending[keptCount] = Move(candidate);

                ++keptCount;
            }

            assert(reclaimed);
            mPending.SpliceInPlace(keptCount);
        });
    }

    void SubmitScheduler::Submit()
    {
        mTerminated.Clear();

        FenceQueueTails();

        while (!mBatches.IsEmpty())
        {
            ObjectPoolUniquePtr<QueueSubmitBatch> batch = mBatches.PopFirst();

            batch->Submit();
            mPending.Add(Move(batch));
        }
    }

    void SubmitScheduler::Synchronize()
    {
        if (mPending.IsEmpty())
            return;

        mTerminated.Reserve(mTerminated.GetSize() + mPending.GetSize());

        // Held for the whole drain. Reclamation is done by hand below, and the guard is what
        // stops the fences signalling along the way from doing it a second time, from under
        // the loops walking the very list they would rewrite.
        ValueGuard guard(mReclaiming, true);

        // Every wait before the first completion. Walking batch by batch instead would mark
        // a fenceless batch completed while the fence covering it, further down its queue,
        // has not been waited on yet.
        for (ObjectPoolUniquePtr<QueueSubmitBatch>& batch : mPending)
            batch->Wait();

        for (ObjectPoolUniquePtr<QueueSubmitBatch>& batch : mPending)
        {
            batch->MarkCompleted();
            mTerminated.Add(Move(batch));
        }

        mPending.Clear();
    }

    void SubmitScheduler::FenceQueueTails()
    {
        for (unsigned int i = 0; i < mBatches.GetSize(); i++)
        {
            QueueSubmitBatch* batch = mBatches[i].Get();

            // The tail of a queue is the batch no later batch shares a queue with. Testing
            // that directly spares the bookkeeping of a visited set, and the quadratic walk
            // it costs is on a list holding a handful of batches per frame.
            bool isQueueTail = true;
            for (unsigned int j = i + 1; j < mBatches.GetSize() && isQueueTail; j++)
                isQueueTail = !IsOnSameQueue(mBatches[j].Get(), batch);

            if (!isQueueTail || batch->HasFence())
                continue;

            Renderer::FenceCreateInfo fenceCreateInfo;
            fenceCreateInfo.Signaled = false;
            fenceCreateInfo.Name = "SubmitScheduler queue tail";

            ConnectFence(batch, mRenderDevice->CreateFence(fenceCreateInfo).StaticCast<Fence>());
        }
    }
}
