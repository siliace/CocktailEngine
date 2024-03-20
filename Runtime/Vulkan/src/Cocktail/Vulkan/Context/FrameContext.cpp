#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/FrameContext.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>

namespace Ck::Vulkan
{
	FrameContext::FrameContext(const Ref<RenderDevice>& renderDevice, const FrameContextCreateInfo& createInfo) :
		mRenderDevice(renderDevice)
	{
		Renderer::CommandListPoolCreateInfo commandListPoolCreateInfo;
		mCommandListPool = CommandListPool::New(mRenderDevice, commandListPoolCreateInfo, nullptr);

		SemaphoreCreateInfo semaphoreCreateInfo;
		mImageAvailable = mRenderDevice->CreateSemaphore(semaphoreCreateInfo);
		mImagePresentable = mRenderDevice->CreateSemaphore(semaphoreCreateInfo);

		Renderer::FenceCreateInfo fenceCreateInfo;
		fenceCreateInfo.Signaled = true;
		mFrameFence = Fence::Cast(mRenderDevice->CreateFence(fenceCreateInfo));
	}

	void FrameContext::SetObjectName(const char* name) const
	{

	}

	Ref<Renderer::RenderDevice> FrameContext::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<Renderer::CommandList> FrameContext::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		for (const Ref<Renderer::CommandList>& commandList : mCommandLists)
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
			return commandList;
		}	

		Ref<Renderer::CommandList> commandList = mCommandListPool->CreateCommandList(createInfo);
		mCommandLists.push_back(commandList);

		return commandList;
	}

	Renderer::FrameToken FrameContext::GetToken() const
	{
		return reinterpret_cast<Renderer::FrameToken>(this);
	}

	void FrameContext::Synchronize() const
	{
		mFrameFence->Wait();
	}

	Ref<Framebuffer> FrameContext::AcquireNextFramebuffer(const Ref<RenderSurface>& renderSurface)
	{
		assert(mCurrentFramebufferIndex.IsEmpty());

		// Acquire the index of the next framebuffer to render
		mCurrentFramebufferIndex = Optional<unsigned int>::Of(
			renderSurface->AcquireNextFramebuffer(UINT64_MAX, mImageAvailable, nullptr)
		);

		return renderSurface->GetFramebuffer(mCurrentFramebufferIndex.Get());
	}

	void FrameContext::WaitQueue(RenderContext& renderContext, Renderer::CommandQueueType commandQueue) const
	{
		renderContext.WaitSemaphore(commandQueue, mImageAvailable, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	}

	void FrameContext::Reset() const
	{
		mCommandListPool->Reset(false);
		mFrameFence->Reset();
	}

	void FrameContext::Present(const Ref<RenderSurface>& renderSurface, RenderContext* renderContext)
	{
		assert(!mCurrentFramebufferIndex.IsEmpty());

		// Add a new fence to be signaled by the graphic queue.
		// This will ensure a future frame won't use resources from the current frame and cause errors.
		renderContext->SignalFence(Renderer::CommandQueueType::Graphic, mFrameFence);

		// The GPU also must to wait until the swapchain's image is not anymore involved in any rendering operations.
		renderContext->SignalSemaphore(Renderer::CommandQueueType::Graphic, mImagePresentable);

		// Effectively submit everything on GPU
		renderContext->Flush();

		VkSemaphore waitSemaphore = mImagePresentable->GetHandle();
		VkSwapchainKHR swapchain = renderSurface->GetSwapchain()->GetHandle();
		unsigned int imageIndex = mCurrentFramebufferIndex.Get();
		VkResult result;

		VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR, nullptr };
		{
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &waitSemaphore;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapchain;
			presentInfo.pImageIndices = &imageIndex;
			presentInfo.pResults = &result;
		}

		QueueFamily graphicFamily = mRenderDevice->GetQueueFamilyContext().GetFamily(Renderer::CommandQueueType::Graphic);

		VkQueue queue;
		vkGetDeviceQueue(mRenderDevice->GetHandle(), graphicFamily.GetIndex(), 0, &queue);

		vkQueuePresentKHR(queue, &presentInfo);

		mCurrentFramebufferIndex = Optional<unsigned int>::Empty();
	}
}
