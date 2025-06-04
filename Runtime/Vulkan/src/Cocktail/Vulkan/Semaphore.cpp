#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Semaphore.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>

namespace Ck::Vulkan
{
	Semaphore::Semaphore(std::shared_ptr<RenderDevice> renderDevice, const SemaphoreCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks)
	{
		VkSemaphoreTypeCreateInfoKHR vkTypeCreateInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO_KHR, nullptr };
		{
			vkTypeCreateInfo.semaphoreType = createInfo.Binary ? VK_SEMAPHORE_TYPE_BINARY_KHR : VK_SEMAPHORE_TYPE_TIMELINE_KHR;
			vkTypeCreateInfo.initialValue = createInfo.InitialValue;
		}

		VkSemaphoreCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, nullptr };
		{
			if (mRenderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::TimelineSynchronization))
				Chain(vkCreateInfo, vkTypeCreateInfo);

			vkCreateInfo.flags = 0;
		}

		COCKTAIL_VK_CHECK(vkCreateSemaphore(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		Semaphore::SetObjectName(createInfo.Name);
	}

	Semaphore::~Semaphore()
	{
		vkDestroySemaphore(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void Semaphore::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_SEMAPHORE;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> Semaphore::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	VkSemaphore Semaphore::GetHandle() const
	{
		return mHandle;
	}
}
