#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>

namespace Ck::Vulkan
{
	DeviceMemory::DeviceMemory(std::shared_ptr<RenderDevice> renderDevice, const DeviceMemoryCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks):
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mSize(createInfo.Size)
	{
		VkMemoryDedicatedAllocateInfoKHR dedicatedAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR, nullptr };
		{
			dedicatedAllocateInfo.image = createInfo.Texture ? createInfo.Texture->GetHandle() : VK_NULL_HANDLE;
			dedicatedAllocateInfo.buffer = createInfo.Buffer ? createInfo.Buffer->GetHandle() : VK_NULL_HANDLE;
		}

		VkMemoryAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr };
		{
			if (createInfo.Dedicated && mRenderDevice->IsFeatureSupported(RenderDeviceFeature::DedicatedAllocation))
				Chain(allocateInfo, dedicatedAllocateInfo);

			allocateInfo.allocationSize = createInfo.Size;
			allocateInfo.memoryTypeIndex = createInfo.MemoryTypeIndex;
		}

		COCKTAIL_VK_CHECK(vkAllocateMemory(mRenderDevice->GetHandle(), &allocateInfo, mAllocationCallbacks, &mHandle));
	}

	DeviceMemory::~DeviceMemory()
	{
		vkFreeMemory(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void DeviceMemory::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::shared_ptr<Renderer::RenderDevice> DeviceMemory::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	VkDeviceMemory DeviceMemory::GetHandle() const
	{
		return mHandle;
	}
}
