#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Command/Fence.hpp>

namespace Ck::Vulkan
{
	Fence::Fence(RenderDevice* renderDevice, const Renderer::FenceCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mSignaled = createInfo.Signaled;

		VkFenceCreateInfo vkCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, nullptr};
		{
			vkCreateInfo.flags = 0;
			if (mSignaled)
				vkCreateInfo.flags |= VK_FENCE_CREATE_SIGNALED_BIT;
		}

		COCKTAIL_VK_CHECK(vkCreateFence(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		Fence::SetObjectName(createInfo.Name);
	}

	Fence::~Fence()
	{
		vkDestroyFence(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void Fence::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_FENCE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}
		
		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Renderer::RenderDevice* Fence::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	void Fence::Wait()
	{
		Wait(Duration::Infinite());
	}

	bool Fence::Wait(const Duration& timeout)
	{
		if (!mSignaled)
		{
			Uint64 nanoseconds = timeout.GetCount(TimeUnit::Nanoseconds());
			mSignaled = vkWaitForFences(mRenderDevice->GetHandle(), 1, &mHandle, VK_TRUE, nanoseconds) == VK_SUCCESS;
			if (mSignaled)
				mOnSignaled.Emit();
		}

		return mSignaled;
	}

	bool Fence::IsSignaled() const
	{
		return vkGetFenceStatus(mRenderDevice->GetHandle(), mHandle) == VK_SUCCESS;
	}

	void Fence::Reset()
	{
		if (mSignaled)
		{
			COCKTAIL_VK_CHECK(vkResetFences(mRenderDevice->GetHandle(), 1, &mHandle));
			mSignaled = false;
		}
	}

	VkFence Fence::GetHandle() const
	{
		return mHandle;
	}

	Signal<>& Fence::OnSignaled()
	{
		return mOnSignaled;
	}
}
