#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>

namespace Ck::Vulkan
{
	RenderContext::RenderContext(std::shared_ptr<RenderDevice> renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mPresentationQueue(VK_NULL_HANDLE),
		mCurrentFrameContext(0)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		const QueueFamily& queueFamily = queueFamilyContext.GetPresentationQueueFamily();
		vkGetDeviceQueue(mRenderDevice->GetHandle(), queueFamily.GetIndex(), 0, &mPresentationQueue);

		for (unsigned int i = 0; i < createInfo.ConcurrentFrameCount; i++)
			mFrameContexts.push_back(std::make_unique<FrameContext>(this, createInfo.RenderSurfaceCount, allocationCallbacks));

		mScheduler = std::make_unique<SubmitScheduler>(mRenderDevice);
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType] = std::make_unique<QueueSubmitter>(mRenderDevice, mScheduler.get(), queueType, 0);
	}

	RenderContext::~RenderContext()
	{
		for (std::unique_ptr<FrameContext>& frameContext : mFrameContexts) 
			frameContext->Synchronize();
	}

	void RenderContext::SetObjectName(const char* name) const
	{
	}

	std::shared_ptr<Renderer::RenderDevice> RenderContext::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	std::shared_ptr<Renderer::CommandListPool> RenderContext::CreateCommandListPool(const Renderer::CommandListPoolCreateInfo& createInfo)
	{
		return std::make_shared<CommandListPool>(mRenderDevice, createInfo, nullptr);
	}

	Renderer::FrameContext* RenderContext::BeginFrame()
	{
		FrameContext* currentFrameContext = mFrameContexts[mCurrentFrameContext].get();
		currentFrameContext->Synchronize();
		currentFrameContext->Reset();

		return currentFrameContext;
	}

	void RenderContext::SignalQueue(Renderer::CommandQueueType queue)
	{
		mSubmitters[queue]->NextSubmit();
	}

	void RenderContext::SignalFence(Renderer::CommandQueueType commandQueue, std::shared_ptr<Fence> fence)
	{
		mSubmitters[commandQueue]->SignalFence(fence);
	}

	void RenderContext::SignalSemaphore(Renderer::CommandQueueType commandQueue, std::shared_ptr<Semaphore> semaphore)
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

	void RenderContext::WaitSemaphore(Renderer::CommandQueueType commandQueue, std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags waitStages)
	{
		mSubmitters[commandQueue]->WaitExternalSemaphore(semaphore, waitStages);
	}

	void RenderContext::ExecuteCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Renderer::CommandList** commandLists, Renderer::Fence* fence)
	{
		unsigned int submitCommandListCount = 0;
		CommandList** submitCommandLists = COCKTAIL_STACK_ALLOC(CommandList*, commandListCount);
		for (unsigned int i = 0; i < commandListCount; i++)
		{
			if (commandLists[i]->GetState() == Renderer::CommandListState::Initial)
				continue;

			if (commandLists[i]->GetState() == Renderer::CommandListState::Recording || commandLists[i]->GetState() == Renderer::CommandListState::RecordingRenderPass)
				commandLists[i]->End();

			assert(commandLists[i]->GetState() == Renderer::CommandListState::Executable);
			submitCommandLists[submitCommandListCount] = static_cast<CommandList*>(commandLists[i]);
			++submitCommandListCount;
		}

		mSubmitters[commandQueue]->ExecuteCommandList(submitCommandListCount, submitCommandLists, static_cast<Fence*>(fence));
	}

	void RenderContext::EndFrame()
	{
		mFrameContexts[mCurrentFrameContext]->Present(mPresentationQueue);

		mCurrentFrameContext = (mCurrentFrameContext + 1) % mFrameContexts.size();
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
