#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>

namespace Ck::Vulkan
{
	CommandListPool::CommandListPool(RenderDevice* renderDevice, const CommandListPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mDescriptorSetAllocator(mRenderDevice),
		mTransient(createInfo.Transient),
		mCommandListResetable(createInfo.Reset)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();

		CommandPoolCreateInfo poolCreateInfo;
		poolCreateInfo.Transient = mTransient;
		poolCreateInfo.Reset = mCommandListResetable;

		if (queueFamilyContext.IsUnified())
		{
			poolCreateInfo.QueueType = Renderer::CommandQueueType::Graphic;
			SharedPtr<CommandPool> commandPool = MakeShared<CommandPool>(mRenderDevice, poolCreateInfo, allocationCallbacks);
			for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
				mCommandPools[queueType] = commandPool;
		}
		else
		{
			for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			{
				poolCreateInfo.QueueType = queueType;
				mCommandPools[queueType] = MakeShared<CommandPool>(mRenderDevice, poolCreateInfo, allocationCallbacks);
			}
		}

		Renderer::BufferUsageFlags bufferUsage;
		mStagingAllocator = MakeUnique<StagingAllocator>(mRenderDevice, bufferUsage, 16 * 1024 * 1024);
	}

	CommandListPool::~CommandListPool()
	{
		CommandListPool::Reset(true);
	}

	StagingBuffer* CommandListPool::AcquireStagingBuffer(std::size_t alignment, std::size_t length) const
	{
		return mStagingAllocator->AcquireStagingBuffer(alignment, length);
	}
	
	SharedPtr<CommandList> CommandListPool::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		SharedPtr<CommandList> commandList = mRenderDevice->CreateCommandList(AsShared(), &mDescriptorSetAllocator, createInfo);
		commandList->Connect(mOnReset, [self = commandList.Get()] {
			self->MarkInitial();
		});

		return commandList;
	}

	void CommandListPool::Reset(bool releaseResources)
	{
		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();
		if (queueFamilyContext.IsUnified())
		{
			mCommandPools[Renderer::CommandQueueType::Graphic]->Reset(releaseResources);
		}
		else 
		{
			for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
				mCommandPools[queueType]->Reset(releaseResources);
		}

		mStagingAllocator->Reset(releaseResources);

		mOnReset.Emit();
	}

	bool CommandListPool::IsTransient() const
	{
		return mTransient;
	}

	bool CommandListPool::IsCommandListResetable() const
	{
		return mCommandListResetable;
	}

	SharedPtr<CommandPool> CommandListPool::GetCommandPool(Renderer::CommandQueueType queueType) const
	{
		return mCommandPools[queueType];
	}
}
