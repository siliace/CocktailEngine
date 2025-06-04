#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Allocator/CommandPool.hpp>
#include <Cocktail/Vulkan/Queue/QueueFamily.hpp>

namespace Ck::Vulkan
{
	CommandPool::CommandPool(std::shared_ptr<RenderDevice> renderDevice, const CommandPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks):
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mQueueType(createInfo.QueueType),
		mTransient(createInfo.Transient),
		mCommandListResetable(createInfo.Reset)
	{
		const QueueFamily& queueFamily = mRenderDevice->GetQueueFamilyContext().GetFamily(mQueueType);

		VkCommandPoolCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			if (mTransient)
				vkCreateInfo.flags |= VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

			if (mCommandListResetable)
				vkCreateInfo.flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			vkCreateInfo.queueFamilyIndex = queueFamily.GetIndex();
		}

		COCKTAIL_VK_CHECK(vkCreateCommandPool(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));
	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void CommandPool::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_COMMAND_POOL;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> CommandPool::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	void CommandPool::Reset(bool releaseResources) const
	{
		VkCommandPoolResetFlags flags = 0;
		if (releaseResources)
			flags |= VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT;

		COCKTAIL_VK_CHECK(vkResetCommandPool(mRenderDevice->GetHandle(), mHandle, flags));
	}

	bool CommandPool::IsTransient() const
	{
		return mTransient;
	}

	bool CommandPool::IsCommandListResetable() const
	{
		return mCommandListResetable;
	}

	VkCommandPool CommandPool::GetHandle() const
	{
		return mHandle;
	}
}
