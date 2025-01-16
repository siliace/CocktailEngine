#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Queue/QueueSubmitBatch.hpp>

namespace Ck::Vulkan
{
	QueueSubmitBatch::QueueSubmitBatch(Ref<RenderDevice> renderDevice, Renderer::CommandQueueType queue, unsigned int queueIndex) :
		mRenderDevice(std::move(renderDevice)),
		mQueue(queue),
		mQueueIndex(queueIndex),
		mSubmitCount(0),
		mCurrentSubmit(nullptr),
		mCompleted(false)
	{
		/// Nothing
	}

	QueueSubmitBatch::~QueueSubmitBatch()
	{
		DisconnectAll();
	}

	void QueueSubmitBatch::NextSubmit()
	{
		mCurrentSubmit = nullptr;
	}

	QueueSubmitDependency QueueSubmitBatch::CreateDependency(VkPipelineStageFlags waitDstStages)
	{
		if (mSubmitCount == 0)
			return { nullptr, 0 };

		SemaphoreCreateInfo semaphoreCreateInfo;
		semaphoreCreateInfo.Binary = true;

		Ref<Semaphore> semaphore = mRenderDevice->CreateSemaphore(semaphoreCreateInfo);
		mSubmits[mSubmitCount - 1].SignalSemaphore(semaphore);

		return { std::move(semaphore), waitDstStages };
	}

	void QueueSubmitBatch::WaitSemaphore(const Ref<Semaphore>& semaphore, VkPipelineStageFlags pipelineWaitStages)
	{
		if (!mCurrentSubmit || mCurrentSubmit->GetWaitSemaphoreCount() == QueueSubmit::MaxWaitSemaphore)
			PushSubmit();

		mCurrentSubmit->WaitSemaphore(semaphore, pipelineWaitStages);
	}

	void QueueSubmitBatch::ExecuteCommandLists(unsigned int commandListCount, Ref<CommandList>* commandLists)
	{
		/// TODO: improve batching ?
		for (unsigned int i = 0; i < commandListCount; i++)
		{
			if (!mCurrentSubmit || mCurrentSubmit->GetCommandListCount() == QueueSubmit::MaxCommandList)
				PushSubmit();

			mCurrentSubmit->ExecuteCommandLists(1, &commandLists[i]);
		}
	}

	void QueueSubmitBatch::SignalSemaphore(const Ref<Semaphore>& semaphore)
	{
		if (!mCurrentSubmit || mCurrentSubmit->GetWaitSemaphoreCount() == QueueSubmit::MaxWaitSemaphore)
			PushSubmit();

		mCurrentSubmit->SignalSemaphore(semaphore);
	}

	void QueueSubmitBatch::AssignFence(const Ref<Fence>& fence)
	{
		if (mFence != nullptr)
			throw std::runtime_error("Cannot assign two fences to the same submit batch");

		Connect(fence->OnSignaled(), [&] {
			if (!mCompleted)
				mOnCompleted.Emit();
		});

		mFence = fence;
	}

	bool QueueSubmitBatch::HasFence() const
	{
		return mFence != nullptr;
	}

	bool QueueSubmitBatch::IsFull() const
	{
		return mSubmitCount == MaxSubmit;
	}

	void QueueSubmitBatch::Submit() const
	{
		VkSubmitInfo submitInfos[MaxSubmit];
		VkSemaphore batchWaitSemaphores[MaxSubmit][QueueSubmit::MaxWaitSemaphore];
		VkPipelineStageFlags batchPipelineWaitStages[MaxSubmit][QueueSubmit::MaxWaitSemaphore];
		VkCommandBuffer batchCommandBuffers[MaxSubmit][QueueSubmit::MaxCommandList];
		VkSemaphore batchSignalSemaphores[MaxSubmit][QueueSubmit::MaxSignalSemaphore];

		for (unsigned int i = 0; i < mSubmitCount; i++)
		{
			VkSemaphore* submitWaitSemaphores = batchWaitSemaphores[i];
			VkPipelineStageFlags* submitPipelineWaitStages = batchPipelineWaitStages[i];
			VkCommandBuffer* submitCommandBuffers = batchCommandBuffers[i];
			VkSemaphore* submitSignalSemaphores = batchSignalSemaphores[i];

			mSubmits[i].Bake(submitWaitSemaphores, submitPipelineWaitStages, submitCommandBuffers, submitSignalSemaphores);

			VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO, nullptr };
			submitInfo.waitSemaphoreCount = mSubmits[i].GetWaitSemaphoreCount();
			submitInfo.pWaitSemaphores = submitWaitSemaphores;
			submitInfo.pWaitDstStageMask = submitPipelineWaitStages;
			submitInfo.commandBufferCount = mSubmits[i].GetCommandListCount();
			submitInfo.pCommandBuffers = submitCommandBuffers;
			submitInfo.signalSemaphoreCount = mSubmits[i].GetSignalSemaphoreCount();
			submitInfo.pSignalSemaphores = submitSignalSemaphores;

			submitInfos[i] = submitInfo;
		}

		const QueueFamily& queueFamily = mRenderDevice->GetQueueFamilyContext().GetFamily(mQueue);

		VkQueue queue;
		vkGetDeviceQueue(mRenderDevice->GetHandle(), queueFamily.GetIndex(), mQueueIndex, &queue);

		VkFence fenceHandle = mFence ? mFence->GetHandle() : VK_NULL_HANDLE;
		vkQueueSubmit(queue, mSubmitCount, submitInfos, fenceHandle);
	}

	void QueueSubmitBatch::MarkCompleted()
	{
		if (!mCompleted)
		{
			for (unsigned int i = 0; i < mSubmitCount; i++)
				mSubmits[i].MarkCompleted();

			mCompleted = true;
			if (mFence)
				mFence->Wait();
		}
	}

	Signal<>& QueueSubmitBatch::OnCompleted()
	{
		return mOnCompleted;
	}

	void QueueSubmitBatch::PushSubmit()
	{
		mCurrentSubmit = &mSubmits[mSubmitCount];
		++mSubmitCount;
	}
}
