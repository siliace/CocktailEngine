#include <Cocktail/Renderer/Context/RenderContextCreateInfo.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>

namespace Ck::Vulkan
{
	RenderContext::RenderContext(RenderDevice* renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mPresentationQueue(VK_NULL_HANDLE),
		mCurrentFrameContext(0),
		mFrameContexts(createInfo.ConcurrentFrameCount)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		const QueueFamily& queueFamily = queueFamilyContext.GetPresentationQueueFamily();
		vkGetDeviceQueue(mRenderDevice->GetHandle(), queueFamily.GetIndex(), 0, &mPresentationQueue);

		for (unsigned int i = 0; i < createInfo.ConcurrentFrameCount; i++)
			mFrameContexts[i] = std::make_unique<FrameContext>(this, allocationCallbacks);

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

	Renderer::RenderDevice* RenderContext::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Renderer::BufferAllocator* RenderContext::GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType)
	{
		return GetCurrentFrameContext()->GetBufferAllocator(usage, memoryType);
	}

	Renderer::Framebuffer* RenderContext::AcquireFramebuffer(Renderer::RenderSurface* renderSurface)
	{
		return GetCurrentFrameContext()->AcquireNextFramebuffer(static_cast<const RenderSurface*>(renderSurface));
	}

	Renderer::CommandList* RenderContext::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		return GetCurrentFrameContext()->CreateCommandList(createInfo);
	}

	void RenderContext::SignalQueue(Renderer::CommandQueueType commandQueue)
	{
		mSubmitters[commandQueue]->NextSubmit();
	}

	void RenderContext::SignalFence(Renderer::CommandQueueType commandQueue, std::shared_ptr<Fence> fence)
	{
		mSubmitters[commandQueue]->SignalFence(std::move(fence));
	}

	void RenderContext::SignalSemaphore(Renderer::CommandQueueType commandQueue, std::shared_ptr<Semaphore> semaphore)
	{
		mSubmitters[commandQueue]->SignalSemaphore(std::move(semaphore));
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
		mSubmitters[commandQueue]->WaitExternalSemaphore(std::move(semaphore), waitStages);
	}

	void RenderContext::SubmitCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Renderer::CommandList** commandLists, Renderer::Fence* fence)
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

	void RenderContext::Submit()
	{
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType]->TerminateBatch();

		mScheduler->Submit();
	}

	void RenderContext::Flush()
	{
		GetCurrentFrameContext()->Present(mPresentationQueue);

		mCurrentFrameContext = (mCurrentFrameContext + 1) % mFrameContexts.GetSize();

		FrameContext* currentFrameContext = GetCurrentFrameContext();
		currentFrameContext->Synchronize();
		currentFrameContext->Reset();
	}

	void RenderContext::Synchronize()
	{
		Flush();
		COCKTAIL_VK_CHECK(vkDeviceWaitIdle(mRenderDevice->GetHandle()));
	}

	FrameContext* RenderContext::GetCurrentFrameContext() const
	{
		return mFrameContexts[mCurrentFrameContext].get();
	}
}
