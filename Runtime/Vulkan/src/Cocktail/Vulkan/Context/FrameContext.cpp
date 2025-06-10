#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Buffer/BufferAllocator.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/FrameContext.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>
#include <Cocktail/Vulkan/Context/RenderSurface.hpp>
#include <Cocktail/Vulkan/Context/Swapchain.hpp>

namespace Ck::Vulkan
{
	FrameContext::FrameContext(RenderContext* renderContext, unsigned int maxRenderSurfaceCount, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderContext(renderContext),
		mRenderSurfaceCount(0),
		mSubmitted(false)
	{
		std::shared_ptr<RenderDevice> renderDevice = std::static_pointer_cast<RenderDevice>(mRenderContext->GetRenderDevice());

		CommandListPoolCreateInfo commandListPoolCreateInfo;
		mCommandListPool = std::make_shared<CommandListPool>(renderDevice, commandListPoolCreateInfo, allocationCallbacks);

		Renderer::FenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.Signaled = false;
		mFrameFence = std::static_pointer_cast<Fence>(renderDevice->CreateFence(fenceCreateInfo));

		SemaphoreCreateInfo semaphoreCreateInfo;
		mAcquiredRenderSurfaces = FixedArray<AcquiredRenderSurface>(maxRenderSurfaceCount);
		for (unsigned int i = 0; i < maxRenderSurfaceCount; i++)
		{
			mAcquiredRenderSurfaces[i].ImageAvailable = renderDevice->CreateSemaphore(semaphoreCreateInfo);
			mAcquiredRenderSurfaces[i].ImagePresentable = renderDevice->CreateSemaphore(semaphoreCreateInfo);
		}
	}

	void FrameContext::Synchronize() const
	{
		if (mSubmitted)
			mFrameFence->Wait();
	}

	void FrameContext::Reset()
	{
		mSubmitted = false;
		for (const auto& [bufferAllocatorKey, bufferAllocator] : mBufferAllocators)
			bufferAllocator->Reset(false);

		mCommandListPool->Reset(false);
		mFrameFence->Reset();
	}

	Framebuffer* FrameContext::AcquireNextFramebuffer(const RenderSurface* renderSurface)
	{
		for (unsigned int i = 0; i < mRenderSurfaceCount; i++)
		{
			if (mAcquiredRenderSurfaces[i].Swapchain == renderSurface->GetSwapchain())
				return renderSurface->GetFramebuffer(mAcquiredRenderSurfaces[i].SwapchainImageIndex).get();
		}

		assert(mRenderSurfaceCount < mAcquiredRenderSurfaces.GetSize());
		AcquiredRenderSurface& acquiredRenderSurface = mAcquiredRenderSurfaces[mRenderSurfaceCount];
		Optional<unsigned int> imageIndex = renderSurface->AcquireNextFramebuffer(Duration::Infinite(), acquiredRenderSurface.ImageAvailable, nullptr);
		if (imageIndex.IsEmpty())
			return nullptr;

		mRenderContext->WaitSemaphore(Renderer::CommandQueueType::Graphic, acquiredRenderSurface.ImageAvailable, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		acquiredRenderSurface.Swapchain = renderSurface->GetSwapchain();
		acquiredRenderSurface.SwapchainImageIndex = imageIndex.Get();

		++mRenderSurfaceCount;

		return renderSurface->GetFramebuffer(imageIndex.Get()).get();
	}

	CommandList* FrameContext::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		for (std::shared_ptr<CommandList> commandList : mCommandLists)
		{
			if (commandList->GetState() != Renderer::CommandListState::Initial)
				continue;

			if (commandList->GetUsage() != createInfo.Usage)
				continue;

			if (commandList->GetDynamicState() != createInfo.DynamicState)
				continue;

			if (commandList->IsSecondary() != createInfo.Secondary)
				continue;

			commandList->SetObjectName(createInfo.Name);
			return commandList.get();
		}

		std::shared_ptr<CommandList> commandList = mCommandListPool->CreateCommandList(createInfo);
		mCommandLists.push_back(commandList);

		return commandList.get();
	}

	Renderer::BufferAllocator* FrameContext::GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType)
	{
		for (const auto& [bufferAllocatorKey, bufferAllocator] : mBufferAllocators)
		{
			if (std::get<0>(bufferAllocatorKey) & usage && std::get<1>(bufferAllocatorKey) == memoryType)
				return bufferAllocator.get();
		}

		std::shared_ptr<RenderDevice> renderDevice = std::static_pointer_cast<RenderDevice>(mRenderContext->GetRenderDevice());
		std::shared_ptr<BufferAllocator> allocator = std::make_shared<BufferAllocator>(renderDevice, usage, 1024 * 1024, memoryType);
		mBufferAllocators[BufferAllocatorKey(usage, memoryType)] = allocator;

		return allocator.get();
	}

	void FrameContext::Present(VkQueue queue)
	{
		// Add a new fence to be signaled by the graphic queue.
		// This will ensure a future frame won't use resources from the current frame and cause errors.
		mRenderContext->SignalFence(Renderer::CommandQueueType::Graphic, mFrameFence);

		// The GPU also must wait until the swapchain's image is not anymore involved in any rendering operations.
		for (unsigned int i = 0; i < mRenderSurfaceCount; i++)
			mRenderContext->SignalSemaphore(Renderer::CommandQueueType::Graphic, mAcquiredRenderSurfaces[i].ImagePresentable);

		// Effectively submit everything on GPU
		mRenderContext->Submit();

		if (mRenderSurfaceCount)
		{
			VkSemaphore* waitSemaphoreHandles = COCKTAIL_STACK_ALLOC(VkSemaphore, mRenderSurfaceCount);
			VkSwapchainKHR* swapchainHandles = COCKTAIL_STACK_ALLOC(VkSwapchainKHR, mRenderSurfaceCount);
			unsigned int* imageIndexes = COCKTAIL_STACK_ALLOC(unsigned int, mRenderSurfaceCount);
			VkResult* result = COCKTAIL_STACK_ALLOC(VkResult, mRenderSurfaceCount);

			for (unsigned int i = 0; i < mRenderSurfaceCount; i++)
			{
				waitSemaphoreHandles[i] = mAcquiredRenderSurfaces[i].ImagePresentable->GetHandle();
				swapchainHandles[i] = mAcquiredRenderSurfaces[i].Swapchain->GetHandle();
				imageIndexes[i] = mAcquiredRenderSurfaces[i].SwapchainImageIndex;
			}

			VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, nullptr };
			{
				presentInfo.waitSemaphoreCount = mRenderSurfaceCount;
				presentInfo.pWaitSemaphores = waitSemaphoreHandles;
				presentInfo.swapchainCount = mRenderSurfaceCount;
				presentInfo.pSwapchains = swapchainHandles;
				presentInfo.pImageIndices = imageIndexes;
				presentInfo.pResults = result;
			}

			vkQueuePresentKHR(queue, &presentInfo);

			for (unsigned int i = 0; i < mRenderSurfaceCount; i++)
				mAcquiredRenderSurfaces[i].Swapchain = nullptr;

			mRenderSurfaceCount = 0;
		}

		mSubmitted = true;
	}
}
