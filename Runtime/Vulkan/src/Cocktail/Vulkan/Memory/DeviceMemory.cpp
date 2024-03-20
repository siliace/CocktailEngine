#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Memory/DeviceMemory.hpp>

namespace Ck::Vulkan
{
	DeviceMemory::DeviceMemory(Ref<RenderDevice> renderDevice, const DeviceMemoryCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks):
		mRenderDevice(std::move(renderDevice)),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mSize = createInfo.Size;
		mDedicated = createInfo.Dedicated;

		VkMemoryPriorityAllocateInfoEXT priorityAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT, nullptr };
		{
			priorityAllocateInfo.priority = createInfo.Priority;
		}

		VkMemoryDedicatedAllocateInfoKHR dedicatedAllocateInfo{ VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO_KHR, nullptr };
		{
			dedicatedAllocateInfo.image = createInfo.Texture ? createInfo.Texture->GetHandle() : VK_NULL_HANDLE;
			dedicatedAllocateInfo.buffer = createInfo.Buffer ? createInfo.Buffer->GetHandle() : VK_NULL_HANDLE;
		}

		VkMemoryAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr };
		{
			if (mDedicated)
			{
				if (!mRenderDevice->IsFeatureSupported(RenderDeviceFeature::DedicatedAllocation))
					throw std::runtime_error("Dedicated memory allocation not supported");

				Chain(allocateInfo, dedicatedAllocateInfo);
			}

			if (createInfo.HasPriority)
			{
				if (!mRenderDevice->IsFeatureSupported(RenderDeviceFeature::MemoryPriority))
					throw std::runtime_error("Memory priority not supported");

				Chain(allocateInfo, priorityAllocateInfo);
			}

			allocateInfo.allocationSize = mSize;
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

	Ref<Renderer::RenderDevice> DeviceMemory::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	VkDeviceMemory DeviceMemory::GetHandle() const
	{
		return mHandle;
	}
}
