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
	FrameContext::FrameContext(RenderContext* renderContext, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderContext(renderContext),
		mSubmitted(false)
	{
		RenderDevice* renderDevice = static_cast<RenderDevice*>(mRenderContext->GetRenderDevice());

		CommandListPoolCreateInfo commandListPoolCreateInfo;
		mCommandListPool = MakeShared<CommandListPool>(renderDevice, commandListPoolCreateInfo, allocationCallbacks);

		Renderer::FenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.Signaled = false;
		mFrameFence = renderDevice->CreateFence(fenceCreateInfo).StaticCast<Fence>();
	}

    FrameContext::~FrameContext()
    {
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
	    AcquiredImage& acquiredImage = mAcquiredImages.ComputeIfMissing(renderSurface, [this](const RenderSurface*) {
	        RenderDevice* renderDevice = static_cast<RenderDevice*>(mRenderContext->GetRenderDevice());

            AcquiredImage acquiredImage;
            acquiredImage.ImageAvailable = renderDevice->CreateSemaphore({});
            acquiredImage.ImagePresentable = renderDevice->CreateSemaphore({});
            return acquiredImage;
	    });

		if (acquiredImage.ImageIndex.IsEmpty())
		{
			acquiredImage.ImageIndex = renderSurface->AcquireNextFramebuffer(Duration::Infinite(), acquiredImage.ImageAvailable, nullptr);
			mRenderContext->WaitSemaphore(Renderer::CommandQueueType::Graphic, acquiredImage.ImageAvailable, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		}

		return renderSurface->GetFramebuffer(acquiredImage.ImageIndex.Get()).Get();
	}

	CommandList* FrameContext::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		return mCommandLists.FindIf([&](SharedPtr<CommandList>& commandList) {
			if (commandList->GetState() != Renderer::CommandListState::Initial)
				return false;

			if (commandList->GetUsage() != createInfo.Usage)
				return false;

			if (commandList->IsSecondary() != createInfo.Secondary)
				return false;

			return true;
		}).Then([&](SharedPtr<CommandList>& commandList) {
			commandList->SetObjectName(createInfo.Name);
		}).GetOrElse([&]() {
			return mCommandLists.Emplace(
				mCommandListPool->CreateCommandList(createInfo)
			);
		}).Get();
	}

	Renderer::BufferAllocator* FrameContext::GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType)
	{
		for (const auto& [bufferAllocatorKey, bufferAllocator] : mBufferAllocators)
		{
			if (std::get<0>(bufferAllocatorKey) & usage && std::get<1>(bufferAllocatorKey) == memoryType)
				return bufferAllocator.Get();
		}

		RenderDevice* renderDevice = static_cast<RenderDevice*>(mRenderContext->GetRenderDevice());
		SharedPtr<BufferAllocator> allocator = MakeShared<BufferAllocator>(renderDevice, usage, 1024 * 1024, memoryType);
		mBufferAllocators[BufferAllocatorKey(usage, memoryType)] = allocator;

		return allocator.Get();
	}

	void FrameContext::Present(VkQueue queue)
	{
		// Add a new fence to be signaled by the graphic queue.
		// This will ensure a future frame won't use resources from the current frame and cause errors.
		mRenderContext->SignalFence(Renderer::CommandQueueType::Graphic, mFrameFence);

		// The GPU also must wait until the swapchain's image is not anymore involved in any rendering operations.
		for (const auto& [acquiredSurface, acquiredImage] : mAcquiredImages)
		{
			if (acquiredImage.ImageIndex.IsEmpty())
				continue;

			mRenderContext->SignalSemaphore(Renderer::CommandQueueType::Graphic, acquiredImage.ImagePresentable);
		}

		// Effectively submit everything on GPU
		mRenderContext->Submit();

		if (!mAcquiredImages.IsEmpty())
		{
			VkSemaphore* waitSemaphoreHandles = COCKTAIL_STACK_ALLOC(VkSemaphore, mAcquiredImages.GetSize());
			VkSwapchainKHR* swapchainHandles = COCKTAIL_STACK_ALLOC(VkSwapchainKHR, mAcquiredImages.GetSize());
			unsigned int* imageIndexes = COCKTAIL_STACK_ALLOC(unsigned int, mAcquiredImages.GetSize());
			VkResult* result = COCKTAIL_STACK_ALLOC(VkResult, mAcquiredImages.GetSize());

			unsigned int acquiredImageCount = 0;
			for (const auto& [acquiredSurface, acquiredImage] : mAcquiredImages)
			{
				if (acquiredImage.ImageIndex.IsEmpty())
					continue;

				waitSemaphoreHandles[acquiredImageCount] = acquiredImage.ImagePresentable->GetHandle();
				swapchainHandles[acquiredImageCount] = acquiredSurface->GetSwapchain()->GetHandle();
				imageIndexes[acquiredImageCount] = acquiredImage.ImageIndex.Get();

				++acquiredImageCount;
			}

			if (acquiredImageCount > 0)
			{
				VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, nullptr };
				{
					presentInfo.waitSemaphoreCount = acquiredImageCount;
					presentInfo.pWaitSemaphores = waitSemaphoreHandles;
					presentInfo.swapchainCount = acquiredImageCount;
					presentInfo.pSwapchains = swapchainHandles;
					presentInfo.pImageIndices = imageIndexes;
					presentInfo.pResults = result;
				}

				vkQueuePresentKHR(queue, &presentInfo);

				for (auto& [acquiredSurface, acquiredImage] : mAcquiredImages)
					acquiredImage.ImageIndex = Optional<unsigned int>::Empty();
			}
		}

		mSubmitted = true;
	}
}
