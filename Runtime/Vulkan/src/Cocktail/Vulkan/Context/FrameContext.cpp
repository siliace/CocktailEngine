#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>
#include <Cocktail/Vulkan/Context/FrameContext.hpp>
#include <Cocktail/Vulkan/Context/RenderContext.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

namespace Ck::Vulkan
{
	BufferPool::BufferPool(Ref<RenderDevice> renderDevice, const Renderer::BufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Super(std::move(renderDevice), createInfo, allocationCallbacks),
		mRemainingCapacity(createInfo.Size)
	{
		/// Nothing
	}

	std::size_t BufferPool::PushData(std::size_t alignment, std::size_t length, const void* data)
	{
		std::size_t padding = ComputePadding(alignment);
		std::size_t offset = GetSize() - mRemainingCapacity;

		assert(alignment == 0 || (offset + padding) % alignment == 0);

		offset += padding;

		void* destination = Map(offset, length);
		std::memcpy(destination, data, length);
		Unmap();

		mRemainingCapacity -= padding + length;

		return offset;
	}

	std::size_t BufferPool::ComputePadding(std::size_t alignment) const
	{
		if (alignment)
		{
			std::size_t currentOffset = GetSize() - mRemainingCapacity;
			if (currentOffset)
			{
				std::size_t currentAlignment = currentOffset % alignment;
				if (currentAlignment)
					return alignment - currentAlignment;
			}
		}

		return 0;
	}

	void BufferPool::Reset()
	{
		mRemainingCapacity = GetSize();
	}

	std::size_t BufferPool::GetRemainingCapacity() const
	{
		return mRemainingCapacity;
	}

	BufferAllocator::BufferAllocator(Ref<RenderDevice> renderDevice, Renderer::BufferUsageFlags usage, std::size_t bufferSize, Renderer::MemoryType memoryType) :
		mRenderDevice(std::move(renderDevice)),
		mUsage(usage),
		mBufferSize(bufferSize),
		mMemoryType(memoryType),
		mMinAlignment(0)
	{
		if (mUsage & Renderer::BufferUsageFlagBits::Storage | Renderer::BufferUsageFlagBits::Uniform)
		{
			VkPhysicalDeviceProperties physicalDeviceProperties;
			vkGetPhysicalDeviceProperties(mRenderDevice->GetPhysicalDeviceHandle(), &physicalDeviceProperties);

			if (mUsage & Renderer::BufferUsageFlagBits::Storage)
				mMinAlignment = std::max(mMinAlignment, physicalDeviceProperties.limits.minStorageBufferOffsetAlignment);

			if (mUsage & Renderer::BufferUsageFlagBits::Uniform)
				mMinAlignment = std::max(mMinAlignment, physicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
		}
	}

	Renderer::BufferArea BufferAllocator::PushData(std::size_t size, const void* data)
	{
		Ref<BufferPool> bufferPool = AcquirePool(size);

		Renderer::BufferArea bufferArea;
		bufferArea.Buffer = bufferPool.Get();
		bufferArea.BaseOffset = bufferPool->PushData(mMinAlignment, size, data);
		bufferArea.Range = size;

		return bufferArea;
	}

	void BufferAllocator::Reserve(std::size_t size)
	{
	}

	void BufferAllocator::Reset(bool release)
	{
		if (!release && !mAcquiredBufferPools.empty())
		{
			for (Ref<BufferPool> bufferPool : mAcquiredBufferPools)
			{
				bufferPool->Reset();
				mAvailableBufferPools.push_back(bufferPool);
			}
		}
		else
		{
			mAvailableBufferPools.clear();
		}

		mAcquiredBufferPools.clear();
		mRenderDevice->Resolve<DeviceMemoryAllocator>()->GarbageCollect(release);
	}

	std::size_t BufferAllocator::GetBufferSize() const
	{
		return mBufferSize;
	}

	Ref<BufferPool> BufferAllocator::AcquirePool(std::size_t size)
	{
		for (Ref<BufferPool> buffer : mAcquiredBufferPools)
		{
			std::size_t padding = buffer->ComputePadding(mMinAlignment);
			if (padding + size <= buffer->GetRemainingCapacity())
				return buffer;
		}

		auto itBuffer = std::find_if(mAvailableBufferPools.begin(), mAvailableBufferPools.end(), [&](const Ref<BufferPool>& bufferPool) {
			return bufferPool->GetRemainingCapacity() + bufferPool->ComputePadding(mMinAlignment) > size;
		});

		Ref<BufferPool> buffer;
		if (itBuffer != mAvailableBufferPools.end())
		{
			buffer = std::move(*itBuffer);
			mAvailableBufferPools.erase(itBuffer);
		}
		else
		{
			Renderer::BufferCreateInfo bufferCreateInfo;
			bufferCreateInfo.Usage = mUsage;
			bufferCreateInfo.Size = std::max(mBufferSize, size);
			bufferCreateInfo.MemoryType = mMemoryType;
			bufferCreateInfo.Exclusive = true;

			buffer = mBufferPool.Allocate(mRenderDevice, bufferCreateInfo, nullptr);
		}

		mAcquiredBufferPools.push_back(buffer);

		return buffer;
	}

	FrameContext::FrameContext(Ref<RenderDevice> renderDevice, const FrameContextCreateInfo& createInfo) :
		mRenderDevice(std::move(renderDevice))
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

	Renderer::BufferAllocator* FrameContext::GetBufferAllocator(Renderer::BufferUsageFlags usage, Renderer::MemoryType memoryType)
	{
		for (const auto& [bufferAllocatorKey, bufferAllocator] : mBufferAllocators)
		{
			if (std::get<0>(bufferAllocatorKey) & usage && std::get<1>(bufferAllocatorKey) == memoryType)
				return bufferAllocator.Get();
		}

		Ref<BufferAllocator> allocator = BufferAllocator::New(mRenderDevice, usage, 8 * 1024 * 1024, memoryType);
		mBufferAllocators[BufferAllocatorKey(usage, memoryType)] = allocator;

		return allocator.Get();
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
		for (const auto& [bufferAllocatorKey, bufferAllocator] : mBufferAllocators)
			bufferAllocator->Reset(false);

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
