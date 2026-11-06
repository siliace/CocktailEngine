#ifndef COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP

#include <CocktailEngine/Core/Memory/Allocator/SizedLinearAllocator.hpp>
#include <CocktailEngine/Core/Utility/ObjectPool.hpp>

#include <CocktailEngine/Vulkan/Queue/QueueSubmitBatch.hpp>

namespace Ck::Vulkan
{
    class Fence;
    class RenderDevice;

    /**
     * \class SubmitScheduler
     *
     * \brief Submits queue batches and reclaims them in per-queue completion order
     *
     * Batches are scheduled over the course of a frame, submitted together by Submit, then
     * reclaimed as their fences signal. Reclamation is where the scheduler earns its name:
     * a batch whose fence signals also reclaims every batch submitted before it on the same
     * queue. A batch therefore does not need a fence of its own to have its command lists
     * and semaphores released, the next fenced batch behind it on its queue releases it.
     * That is what lets a frame carry one fence per queue rather than one per batch.
     *
     * One fence per queue is not merely allowed, it is required, and Submit is what makes it
     * true: a queue whose last batch has no fence is given one there. Without it the chain
     * has no end and the batches behind it are never reclaimed, which is a leak rather than
     * a crash, and a quiet one. The fence a caller supplies is used whenever there is one,
     * so a frame already fencing a queue costs nothing extra.
     *
     * The ordering this relies on is the one a VkQueue gives, and nothing more. Batches are
     * ordered against the batches sharing their (queue family, queue index) pair, and are
     * unordered against every other batch: queues progress independently, so a transfer
     * fence signalling says nothing about a graphics batch submitted earlier. Reclamation
     * walks the pending list filtering on that pair for exactly this reason, and getting it
     * wrong is not a stall but a use-after-free, since reclaiming a fenceless batch drops
     * the references keeping its command lists alive while the GPU may still be reading
     * them. Ordering across queues is the business of the dependency semaphores
     * QueueSubmitBatch::CreateDependency hands out, not of this class.
     *
     * Completed batches are not destroyed where they complete. They are parked until the
     * following Submit, see mTerminated.
     *
     * \warning Confined to a single thread, and the confinement reaches further than the
     *          methods below. Reclamation is driven by Fence::OnSignaled, so the thread
     *          that waits on a fence given to ConnectFence is the thread that mutates the
     *          scheduler. Neither the batch pool nor the three lists are synchronized, so
     *          every batch handed to ConnectFence must carry a fence waited on by the
     *          owning thread.
     */
    class SubmitScheduler
    {
    public:

        /**
         * \brief Constructor
         *
         * Creates a scheduler holding nothing. Batch storage is allocated on first use.
         *
         * \param renderDevice Device the batches are submitted to, must outlive the scheduler
         */
        explicit SubmitScheduler(RenderDevice* renderDevice);

        /**
         * \brief Deleted copy constructor
         *
         * The scheduler's identity is shared with its batches: each fenced batch owns a
         * callback holding this pointer, see ConnectFence.
         */
        SubmitScheduler(const SubmitScheduler& other) = delete;

        /**
         * \brief Deleted move constructor
         *
         * Moving would leave those callbacks pointing at the moved-from scheduler, which is
         * the object whose lists no longer hold the batches.
         */
        SubmitScheduler(SubmitScheduler&& other) noexcept = delete;

        /**
         * \brief Destructor
         *
         * Drains whatever is still in flight, see Synchronize.
         *
         * \remark Asserts that nothing is scheduled. Batches left scheduled were recorded
         *         and then dropped without a Submit, which the caller is not expected to do
         *         and which this cannot repair: submitting them here would send work to a
         *         device that is being torn down.
         */
        ~SubmitScheduler();

        /**
         * \brief Deleted copy assignment operator
         */
        SubmitScheduler& operator=(const SubmitScheduler& other) = delete;

        /**
         * \brief Deleted move assignment operator
         */
        SubmitScheduler& operator=(SubmitScheduler&& other) noexcept = delete;

        /**
         * \brief Creates an empty batch targeting a queue, to be submitted by the next Submit
         *
         * \param queueType Queue family the batch is submitted to
         * \param queueIndex Index of the queue within its family
         *
         * \return Batch owned by the scheduler, never null
         *
         * \throw OutOfMemory If the batch pool had to grow and could not allocate
         *
         * \warning Non-owning. The batch lives until it completes and the Submit following
         *          its completion releases it, so the pointer must not be held across a
         *          Submit. Callers keeping a current batch between frames are expected to
         *          drop it beforehand, as QueueSubmitter::TerminateBatch does.
         */
        QueueSubmitBatch* ScheduleBatch(Renderer::CommandQueueType queueType, unsigned int queueIndex);

        /**
         * \brief Gives a batch its fence and makes that fence drive reclamation
         *
         * The fence is assigned to the batch, and the batch's completion is wired to the
         * scheduler so that signalling it reclaims the batch along with everything submitted
         * before it on the same queue.
         *
         * \param batch Batch to fence, must come from ScheduleBatch and not have been
         *              submitted yet
         * \param fence Fence signalled when the batch's work is done
         *
         * \throw std::runtime_error If \p batch already carries a fence
         *
         * \remark The callback is owned by \p batch rather than by the scheduler, so it goes
         *         away when the batch is released. It holds this scheduler by pointer, which
         *         is why the scheduler is neither copyable nor movable.
         */
        void ConnectFence(QueueSubmitBatch* batch, SharedPtr<Fence> fence);

        /**
         * \brief Submits every scheduled batch and releases the ones reclaimed since last call
         *
         * Batches are submitted in scheduling order and move to the pending list, where they
         * stay until their queue's fence reclaims them. The last batch of each queue is
         * given a fence beforehand when the caller did not supply one, which is what bounds
         * the pending list.
         *
         * \throw OutOfMemory If a fence had to be created for a queue tail and could not be
         *
         * \remark Releasing the previously reclaimed batches is the first thing done, not the
         *         last: a batch reclaimed during this very call must survive until the next
         *         one, for the reason given on mTerminated.
         */
        void Submit();

        /**
         * \brief Blocks until every submitted batch is done, then reclaims them all
         *
         * Leaves the pending list empty. Meant for tearing down, and for the points where
         * the caller needs the device idle.
         *
         * \remark Waits on every fence before marking anything completed, rather than
         *         reclaiming each batch as its own wait returns. A fenceless batch is only
         *         known to be done once the fenced tail of its queue is, so releasing it
         *         while a wait is still outstanding would be the very use-after-free the
         *         per-queue ordering exists to avoid.
         *
         * \remark Reclaims by hand instead of letting the fences drive it. A fence waited on
         *         earlier is already signalled and emits nothing the second time, so a drain
         *         that relied on the signals would quietly leave those batches pending.
         */
        void Synchronize();

    private:

        /**
         * \brief Gives a fence to the last scheduled batch of every queue that lacks one
         *
         * Called by Submit before anything leaves for the pending list. Reclamation runs
         * backwards from a fence along its own queue, so a queue whose last batch carries
         * none has no starting point and keeps every batch of that queue pending for good.
         * Fencing the tail is the smallest thing that closes the chain: one fence per queue
         * per frame, and none at all for a queue the caller already fenced.
         *
         * \throw OutOfMemory If a fence could not be created
         *
         * \remark The fences created here are held by the batches alone and go away with
         *         them, so each one costs a vkCreateFence and a vkDestroyFence per frame and
         *         per unfenced queue. Worth pooling if a profile ever says so, which is also
         *         true of the semaphores QueueSubmitBatch::CreateDependency creates.
         */
        void FenceQueueTails();

        /**
         * \brief Storage of every batch the scheduler hands out
         *
         * Declared first so that it is destroyed last, after the three lists holding the
         * batches carved out of it. Pooled objects may outlive their pool, so the reverse
         * order would work too, but only by relying on something stated elsewhere.
         */
        TlsObjectPool<QueueSubmitBatch> mBatchPool;

        RenderDevice* mRenderDevice; /*!< Device the batches are submitted to */

        /**
         * \brief Whether a reclamation is under way
         *
         * Reclamation is re-entrant by nature. QueueSubmitBatch::MarkCompleted waits on the
         * batch's fence, Fence::Wait emits OnSignaled on the way out, and that lands back in
         * the callback of the very batch being reclaimed. This is what makes the nested call
         * return instead of walking a pending list halfway through being compacted, and it
         * is also what lets Synchronize reclaim by hand without the fences interfering.
         */
        bool mReclaiming;
        Array<ObjectPoolUniquePtr<QueueSubmitBatch>> mBatches; /*!< Scheduled, not submitted yet, in scheduling order */
        Array<ObjectPoolUniquePtr<QueueSubmitBatch>> mPending; /*!< Submitted, awaiting completion, in submission order */

        /**
         * \brief Reclaimed batches, released by the next Submit
         *
         * The delay is not a convenience. A batch is reclaimed from inside its own
         * completion callback, and that callback is owned by the batch, so destroying it
         * there would free the lambda that is running. Parking the batch moves its
         * destruction to a point where nothing of it is on the stack.
         */
        Array<ObjectPoolUniquePtr<QueueSubmitBatch>> mTerminated;
    };
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_SUBMITSCHEDULER_HPP
