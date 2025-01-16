#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>

namespace Ck::Vulkan
{
	RenderContext::RenderContext(Ref<RenderDevice> renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mPresentationQueue(VK_NULL_HANDLE),
		mFrameContextCount(createInfo.ConcurrentFrameCount),
		mCurrentFrameContext(0)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		const QueueFamily& queueFamily = queueFamilyContext.GetPresentationQueueFamily();
		vkGetDeviceQueue(mRenderDevice->GetHandle(), queueFamily.GetIndex(), 0, &mPresentationQueue);

		for (unsigned int i = 0; i < mFrameContextCount; i++)
			mFrameContexts[i] = FrameContext::New(this, createInfo.RenderSurfaceCount, allocationCallbacks);

		mScheduler = SubmitScheduler::New(mRenderDevice);
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType] = QueueSubmitter::New(mRenderDevice, mScheduler, queueType, 0);
	}

	RenderContext::~RenderContext()
	{
		for (unsigned int i = 0; i < mFrameContextCount; i++)
			mFrameContexts[i]->Synchronize();
	}

	void RenderContext::SetObjectName(const char* name) const
	{
	}

	Ref<Renderer::RenderDevice> RenderContext::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<Renderer::CommandListPool> RenderContext::CreateCommandListPool(const Renderer::CommandListPoolCreateInfo& createInfo)
	{
		return CommandListPool::New(mRenderDevice, createInfo, nullptr);
	}

	Renderer::FrameContext* RenderContext::BeginFrame()
	{
		FrameContext* currentFrameContext = mFrameContexts[mCurrentFrameContext].Get();
		currentFrameContext->Synchronize();
		currentFrameContext->Reset();

		return currentFrameContext;
	}

	void RenderContext::SignalQueue(Renderer::CommandQueueType queue)
	{
		mSubmitters[queue]->NextSubmit();
	}

	void RenderContext::SignalFence(Renderer::CommandQueueType commandQueue, const Ref<Fence>& fence)
	{
		mSubmitters[commandQueue]->SignalFence(fence);
	}

	void RenderContext::SignalSemaphore(Renderer::CommandQueueType commandQueue, const Ref<Semaphore>& semaphore)
	{
		mSubmitters[commandQueue]->SignalSemaphore(semaphore);
	}

	void RenderContext::WaitQueue(Renderer::CommandQueueType waitingQueue, Renderer::CommandQueueType waitedQueue)
	{
		if (waitingQueue == waitedQueue)
			return;

		VkPipelineStageFlags waitDstStage = 0;
		switch (waitingQueue)
		{
		case Renderer::CommandQueueType::Graphic: 
			waitDstStage = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
			break;

		case Renderer::CommandQueueType::Transfer: 
			waitDstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			break;

		case Renderer::CommandQueueType::Compute:
			waitDstStage = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			break;
		}

		QueueSubmitDependency dependency = mSubmitters[waitedQueue]->CreateDependency(waitDstStage);
		if (dependency.WaitSemaphore)
		{
			bool selfDependent = mSubmitters[waitedQueue]->HasDependencyTo(waitingQueue);
			mSubmitters[waitingQueue]->WaitDependency(dependency, waitedQueue, selfDependent);
		}
	}

	void RenderContext::WaitSemaphore(Renderer::CommandQueueType commandQueue, const Ref<Semaphore>& semaphore, VkPipelineStageFlags waitStages)
	{
		mSubmitters[commandQueue]->WaitExternalSemaphore(semaphore, waitStages);
	}

	void RenderContext::ExecuteCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Ref<Renderer::CommandList>* commandLists, Ref<Renderer::Fence> fence)
	{
		std::vector<Ref<CommandList>> vkCommandLists(commandListCount);
		for (unsigned int i = 0; i < commandListCount; i++)
		{
			vkCommandLists[i] = CommandList::Cast(commandLists[i]);
			assert(vkCommandLists[i]->GetState() == Renderer::CommandListState::Executable);
		}

		mSubmitters[commandQueue]->ExecuteCommandList(commandListCount, vkCommandLists.data(), Fence::Cast(fence));
	}

	void RenderContext::EndFrame()
	{
		mFrameContexts[mCurrentFrameContext]->Present(mPresentationQueue);

		mCurrentFrameContext = (mCurrentFrameContext + 1) % mFrameContextCount;
	}

	void RenderContext::Flush()
	{
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType]->TerminateBatch();

		mScheduler->Flush();
	}

	void RenderContext::Synchronize()
	{
		Flush();
		COCKTAIL_VK_CHECK(vkDeviceWaitIdle(mRenderDevice->GetHandle()));
	}
}
