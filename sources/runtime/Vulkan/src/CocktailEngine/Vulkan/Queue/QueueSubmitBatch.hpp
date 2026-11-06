#ifndef COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
#define COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP

#include <CocktailEngine/Core/Signal/Signal.hpp>

#include <CocktailEngine/Renderer/Command/CommandQueueType.hpp>

#include <CocktailEngine/Vulkan/Queue/QueueSubmit.hpp>
#include <CocktailEngine/Vulkan/Queue/QueueSubmitDependency.hpp>

namespace Ck::Vulkan
{
    class Fence;

    /**
     * \brief
     */
    class QueueSubmitBatch : public Observable
    {
    public:

        /**
         * \brief
         */
        static constexpr unsigned int MaxSubmit = 16;

        /**
         * \brief
         */
        QueueSubmitBatch(RenderDevice* renderDevice, Renderer::CommandQueueType queue, unsigned int queueIndex);

        /**
         * \brief
         */
        ~QueueSubmitBatch() override;

        /**
         * \brief
         */
        void NextSubmit();

        /**
         * \brief Create a dependency to the lastly closed submit
         * \param waitDstStages
         * \return
         */
        QueueSubmitDependency CreateDependency(VkPipelineStageFlags waitDstStages);

        /**
         * \brief
         * \param semaphore
         * \param pipelineWaitStages
         */
        void WaitSemaphore(SharedPtr<Semaphore> semaphore, VkPipelineStageFlags pipelineWaitStages);

        /**
         * \brief
         * \param commandListCount
         * \param commandLists
         */
        void ExecuteCommandLists(unsigned int commandListCount, CommandList** commandLists);

        /**
         * \brief
         * \param semaphore
         */
        void SignalSemaphore(SharedPtr<Semaphore> semaphore);

        /**
         * \brief
         * \param fence
         */
        void AssignFence(SharedPtr<Fence> fence);

        /**
         * \brief Returns the queue family this batch is submitted to
         *
         * \return Queue family of the batch
         *
         * \remark Together with GetQueueIndex, identifies the VkQueue whose progress orders
         *         this batch. Two batches reporting the same pair complete in submission
         *         order, two reporting different pairs complete independently.
         */
        Renderer::CommandQueueType GetQueueType() const;

        /**
         * \brief Returns the index of the queue this batch is submitted to, within its family
         *
         * \return Queue index of the batch
         */
        unsigned int GetQueueIndex() const;

        /**
         * \brief
         * \return
         */
        bool HasFence() const;

        /**
         * \brief
         * \return
         */
        bool IsFull() const;

        /**
         * \brief
         */
        void Submit() const;

        /**
         * \brief Blocks until the work submitted by this batch is done
         *
         * \remark Returns at once for a batch carrying no fence, which is not a claim that
         *         its work is done. A fenceless batch offers nothing to wait on, and its
         *         completion is only known through the fenced batch following it on its
         *         queue.
         *
         * \remark Emits OnCompleted on the way out when the wait is what signals the fence,
         *         so waiting is enough to drive reclamation. It emits nothing once
         *         MarkCompleted has run.
         */
        void Wait();

        /**
         * \brief
         */
        void MarkCompleted();

        /**
         * \brief
         * \return
         */
        Signal<>& OnCompleted();

    private:

        /**
         * \brief
         */
        void PushSubmit();

        RenderDevice* mRenderDevice;
        Renderer::CommandQueueType mQueue;
        unsigned int mQueueIndex;
        SharedPtr<Fence> mFence;
        unsigned int mSubmitCount;
        QueueSubmit mSubmits[MaxSubmit];
        QueueSubmit* mCurrentSubmit;
        bool mCompleted;
        Signal<> mOnCompleted;
    };
}

#endif // COCKTAILENGINE_VULKAN_QUEUE_QUEUESUBMITBATCH_HPP
