#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp>

namespace Ck::Vulkan
{
	DescriptorUpdateTemplate::DescriptorUpdateTemplate(const Ref<RenderDevice>& renderDevice, const DescriptorUpdateTemplateCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mHandle(VK_NULL_HANDLE),
		mAllocationCallbacks(allocationCallbacks)
	{
		mDescriptorSetLayout = createInfo.DescriptorSetLayout;

		unsigned int k = 0;
		const std::size_t elementStride = GetElementStride();
		VkDescriptorUpdateTemplateEntry* entries = COCKTAIL_STACK_ALLOC(VkDescriptorUpdateTemplateEntry, mDescriptorSetLayout->GetDescriptorCount());
		for (unsigned int i = 0; i < mDescriptorSetLayout->GetBindingCount(); i++)
		{
			const DescriptorSetLayoutBinding* binding = mDescriptorSetLayout->GetBinding(i);

			for (unsigned int j = 0; j < binding->DescriptorCount; j++)
			{
				entries[k].dstBinding = binding->Binding;
				entries[k].dstArrayElement = j;
				entries[k].descriptorCount = 1;
				entries[k].descriptorType = ToVkType(binding->Type);
				entries[k].offset = k * elementStride;
				entries[k].stride = elementStride;

				++k;
			}
		}

		VkDescriptorUpdateTemplateCreateInfoKHR vkCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO_KHR, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.descriptorUpdateEntryCount = mDescriptorSetLayout->GetDescriptorCount();
			vkCreateInfo.pDescriptorUpdateEntries = entries;

			if (mDescriptorSetLayout->SupportPushDescriptor())
			{
				vkCreateInfo.templateType = VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_PUSH_DESCRIPTORS_KHR;
				vkCreateInfo.pipelineBindPoint = createInfo.PipelineLayout->GetBindPoint();
				vkCreateInfo.pipelineLayout = createInfo.PipelineLayout->GetHandle();
				vkCreateInfo.set = createInfo.Set;
			}
			else
			{
				vkCreateInfo.descriptorSetLayout = mDescriptorSetLayout->GetHandle();
				vkCreateInfo.templateType = VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET_KHR;
			}
		}

		COCKTAIL_VK_CHECK(vkCreateDescriptorUpdateTemplateKHR(mRenderDevice->GetHandle(), &vkCreateInfo, nullptr, &mHandle));
	}

	DescriptorUpdateTemplate::~DescriptorUpdateTemplate()
	{
		vkDestroyDescriptorUpdateTemplateKHR(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void DescriptorUpdateTemplate::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(mHandle);
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	std::size_t DescriptorUpdateTemplate::GetElementStride() const
	{
		return std::max(sizeof(VkDescriptorBufferInfo), sizeof(VkDescriptorImageInfo));
	}

	Ref<Renderer::RenderDevice> DescriptorUpdateTemplate::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	VkDescriptorUpdateTemplateKHR DescriptorUpdateTemplate::GetHandle() const
	{
		return mHandle;
	}
}
