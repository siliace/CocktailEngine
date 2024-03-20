#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>

namespace Ck::Vulkan
{
	RenderContext::RenderContext(const Ref<RenderDevice>& renderDevice, const Renderer::RenderContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mCurrentFrameContext(0)
	{
		mRenderSurface = RenderSurface::Cast(createInfo.RenderSurface);
		mFrameContextCount = mRenderSurface->GetBufferCount();

		FrameContextCreateInfo frameContextCreateInfo;
		for (unsigned int i = 0; i < mFrameContextCount; i++)
			mFrameContexts[i] = FrameContext::New(mRenderDevice, frameContextCreateInfo);

		mScheduler = SubmitScheduler::New(mRenderDevice);
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType] = QueueSubmitter::New(mRenderDevice, mScheduler, queueType, 0);

		Connect(mRenderSurface->OnClose(), [&]() {
			for (unsigned int i = 0; i < mFrameContextCount; i++)
				mFrameContexts[i]->Synchronize();

			mScheduler->Synchronize();
			for (unsigned int i = 0; i < mFrameContextCount; i++)
				mFrameContexts[i] = nullptr;
		});

		Connect(mRenderSurface->OnResized(), [&](Extent2D<unsigned int> size) {
			// Ensure we are not rendering anything both on CPU and GPU so we can recreated our Framebuffers safely
			for (unsigned int i = 0; i < mFrameContextCount; i++)
				mFrameContexts[i]->Synchronize();
		});

		Connect(mRenderSurface->OnRedraw(), [&]() {
			// Don't try to render/present anything if we don't have a Swapchain
			// This can happen when the Window bound to the RenderSurface is minimized
			if (!mRenderSurface->GetSwapchain())
				return;

			Ref<FrameContext> currentFrameContext = mFrameContexts[mCurrentFrameContext];

			// Ensure resources held by the FrameContext are free to use
			currentFrameContext->Synchronize();

			// Get the framebuffer where render the content of the RenderSurface
			Ref<Framebuffer> currentFramebuffer = currentFrameContext->AcquireNextFramebuffer(mRenderSurface); 

			// Reset every resources held by the FrameContext
			currentFrameContext->Reset();

			// Setup per-frame resources required for the rending of the RenderSurface
			mOnBeforeRedraw.Emit(currentFrameContext.Get());

			// Wait on GPU for the acquired framebuffer to be acquired
			currentFrameContext->WaitQueue(*this, Renderer::CommandQueueType::Graphic);

			// Render the content of the RenderSurface
			mOnRedraw.Emit(currentFrameContext.Get(), currentFramebuffer.Get());

			// Present the rendered image on the RenderSurface
			currentFrameContext->Present(mRenderSurface, this);

			// Jump to the next FrameContext
			mCurrentFrameContext = (mCurrentFrameContext + 1) % mFrameContextCount;
		});
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

	void RenderContext::ExecuteCommandLists(Renderer::CommandQueueType commandQueue, unsigned int commandListCount, Ref<Renderer::CommandList>* commandLists, const Ref<Renderer::Fence>& fence)
	{
		std::vector<Ref<CommandList>> vkCommandLists(commandListCount);
		for (unsigned int i = 0; i < commandListCount; i++)
		{
			vkCommandLists[i] = CommandList::Cast(commandLists[i]);
			assert(vkCommandLists[i]->GetState() == Renderer::CommandListState::Executable);
		}

		mSubmitters[commandQueue]->ExecuteCommandList(commandListCount, vkCommandLists.data(), Fence::Cast(fence));
	}

	void RenderContext::Flush()
	{
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mSubmitters[queueType]->TerminateBatch();

		mScheduler->Flush();
	}

	void RenderContext::Synchronize() const
	{
		COCKTAIL_VK_CHECK(vkDeviceWaitIdle(mRenderDevice->GetHandle()));
	}

	Signal<Renderer::FrameContext*>& RenderContext::OnBeforeRedraw()
	{
		return mOnBeforeRedraw;
	}

	Signal<Renderer::FrameContext*, const Renderer::Framebuffer*>& RenderContext::OnRedraw()
	{
		return mOnRedraw;
	}
}
