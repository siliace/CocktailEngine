#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Command/CommandList.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp>

namespace Ck::Vulkan
{
	CommandListPool::CommandListPool(Ref<RenderDevice> renderDevice, const Renderer::CommandListPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mTransient(createInfo.Transient),
		mCommandListResetable(createInfo.Reset)
	{
		mDescriptorSetAllocator = mRenderDevice->CreateDescriptorSetAllocator({});

		const QueueFamilyContext& queueFamilyContext = mRenderDevice->GetQueueFamilyContext();

		CommandPoolCreateInfo poolCreateInfo;
		poolCreateInfo.Transient = mTransient;
		poolCreateInfo.Reset = mCommandListResetable;

		if (queueFamilyContext.IsUnified())
		{
			poolCreateInfo.QueueType = Renderer::CommandQueueType::Graphic;
			Ref<CommandPool> commandPool = CommandPool::New(mRenderDevice, poolCreateInfo, allocationCallbacks);
			for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
				mCommandPools[queueType] = commandPool;
		}
		else
		{
			for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			{
				poolCreateInfo.QueueType = queueType;
				mCommandPools[queueType] = CommandPool::New(mRenderDevice, poolCreateInfo, allocationCallbacks);
			}
		}

		mStagingAllocator = StagingAllocator::New(mRenderDevice, 16 * 1024 * 1024);
	}

	CommandListPool::~CommandListPool()
	{
		CommandListPool::Reset(true);
	}

	void CommandListPool::SetObjectName(const char* name) const
	{
		for (Renderer::CommandQueueType queueType : Enum<Renderer::CommandQueueType>::Values)
			mCommandPools[queueType]->SetObjectName(name);
	}

	Ref<Renderer::RenderDevice> CommandListPool::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<StagingBuffer> CommandListPool::AcquireStagingBuffer(std::size_t alignment, std::size_t length) const
	{
		return mStagingAllocator->AcquireStagingBuffer(alignment, length);
	}
	
	Ref<Renderer::CommandList> CommandListPool::CreateCommandList(const Renderer::CommandListCreateInfo& createInfo)
	{
		Ref<CommandList> commandList = mRenderDevice->CreateCommandList(Self(), mDescriptorSetAllocator, createInfo);
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

	Ref<CommandPool> CommandListPool::GetCommandPool(Renderer::CommandQueueType queueType) const
	{
		return mCommandPools[queueType];
	}
}
