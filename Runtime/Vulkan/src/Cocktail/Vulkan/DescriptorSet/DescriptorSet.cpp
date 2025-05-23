#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp>

namespace Ck::Vulkan
{
	DescriptorSet::DescriptorSet(const Ref<RenderDevice>& renderDevice, const Ref<DescriptorPool>& pool, const DescriptorSetCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mPool(pool),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mLayout = createInfo.Layout;

		VkDescriptorSetAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr };
		{
			VkDescriptorSetLayout layouts[] = {
				mLayout->GetHandle()
			};
				 
			allocateInfo.descriptorPool = mPool->GetHandle();
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = layouts;
		}

		COCKTAIL_VK_CHECK(vkAllocateDescriptorSets(mRenderDevice->GetHandle(), &allocateInfo, &mHandle));
	}

	DescriptorSet::~DescriptorSet()
	{
		if (mPool->SupportFree())
			COCKTAIL_VK_CHECK(vkFreeDescriptorSets(mRenderDevice->GetHandle(), mPool->GetHandle(), 1, &mHandle));
	}

	void DescriptorSet::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_DESCRIPTOR_SET;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Ref<Renderer::RenderDevice> DescriptorSet::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	Ref<DescriptorSetLayout> DescriptorSet::GetLayout() const
	{
		return mLayout;
	}

	VkDescriptorSet DescriptorSet::GetHandle() const
	{
		return mHandle;
	}
}
