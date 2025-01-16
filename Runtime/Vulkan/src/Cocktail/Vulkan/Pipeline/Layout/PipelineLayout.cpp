#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp>

namespace Ck::Vulkan
{
	PipelineLayout::PipelineLayout(const Ref<RenderDevice>& renderDevice, const PipelineLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		mRenderDevice(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE),
		mDescriptorSetLayouts(createInfo.DescriptorSetLayouts),
		mPipelineConstantBlocks(createInfo.PushConstantBlocks)
	{
		mBindPoint = createInfo.BindPoint;

		std::vector<VkDescriptorSetLayout> descriptorSetLayoutHandles(mDescriptorSetLayouts.size());
		for (unsigned int i = 0; i < descriptorSetLayoutHandles.size(); i++)
			descriptorSetLayoutHandles[i] = mDescriptorSetLayouts[i]->GetHandle();

		unsigned int pushConstantRangeCount = 0;
		VkPushConstantRange pushConstantRanges[Enum<Renderer::ShaderType>::ValueCount];
		for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
		{
			mPipelineConstantBlocks[shaderType].Map([&](const PushConstantBlockInfo& pushConstantBlock) {
				unsigned int size = 0;
				for (const BlockMember& member : pushConstantBlock.Members)
					size += member.GetSize();

				VkPushConstantRange pushConstantRange;
				pushConstantRange.stageFlags = ToVkType(shaderType);
				pushConstantRange.offset = pushConstantBlock.BaseOffset;
				pushConstantRange.size = pushConstantBlock.Size;

				return pushConstantRange;
			}).Then([&](const VkPushConstantRange& pushConstantRange) {
				pushConstantRanges[pushConstantRangeCount++] = pushConstantRange;
			});
		}

		VkPipelineLayoutCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.setLayoutCount = static_cast<unsigned int>(descriptorSetLayoutHandles.size());
			vkCreateInfo.pSetLayouts = descriptorSetLayoutHandles.data();
			vkCreateInfo.pushConstantRangeCount = pushConstantRangeCount;
			vkCreateInfo.pPushConstantRanges = pushConstantRanges;
		}

		COCKTAIL_VK_CHECK(vkCreatePipelineLayout(mRenderDevice->GetHandle(), &vkCreateInfo, mAllocationCallbacks, &mHandle));

		if (mRenderDevice->IsFeatureSupported(RenderDeviceFeature::DescriptorUpdateTemplate))
		{
			mUpdateTemplates.reserve(mDescriptorSetLayouts.size());
			for (unsigned int i = 0; i < mDescriptorSetLayouts.size(); i++)
			{
				if (mDescriptorSetLayouts[i]->GetDescriptorCount() == 0)
				{
					mUpdateTemplates.push_back(nullptr);
					continue;
				}

				DescriptorUpdateTemplateCreateInfo descriptorUpdateTemplateCreateInfo;
				descriptorUpdateTemplateCreateInfo.DescriptorSetLayout = mDescriptorSetLayouts[i];
				descriptorUpdateTemplateCreateInfo.PipelineLayout = this;
				descriptorUpdateTemplateCreateInfo.Set = i;

				mUpdateTemplates.push_back(
					mRenderDevice->CreateDescriptorUpdateTemplate(descriptorUpdateTemplateCreateInfo)
				);
			}
		}

		PipelineLayout::SetObjectName(createInfo.Name);
	}

	PipelineLayout::~PipelineLayout()
	{
		vkDestroyPipelineLayout(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	void PipelineLayout::SetObjectName(const char* name) const
	{
		VkDebugUtilsObjectNameInfoEXT objectNameInfo{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr };
		{
			objectNameInfo.objectType = VK_OBJECT_TYPE_PIPELINE_LAYOUT;
			objectNameInfo.objectHandle = reinterpret_cast<Uint64>(GetHandle());
			objectNameInfo.pObjectName = name;
		}

		COCKTAIL_VK_CHECK(vkSetDebugUtilsObjectNameEXT(mRenderDevice->GetHandle(), &objectNameInfo));
	}

	Ref<Renderer::RenderDevice> PipelineLayout::GetRenderDevice() const
	{
		return mRenderDevice;
	}

	unsigned int PipelineLayout::GetDescriptorSetLayoutCount() const
	{
		return static_cast<unsigned int>(mDescriptorSetLayouts.size());
	}

	Ref<DescriptorSetLayout> PipelineLayout::GetDescriptorSetLayout(unsigned int set) const
	{
		if (set >= mDescriptorSetLayouts.size())
			return nullptr;

		return mDescriptorSetLayouts[set];
	}

	Ref<DescriptorUpdateTemplate> PipelineLayout::GetDescriptorUpdateTemplate(unsigned int set) const
	{
		if (set >= mUpdateTemplates.size())
			return nullptr;

		return mUpdateTemplates[set];
	}

	const Optional<PushConstantBlockInfo>& PipelineLayout::GetPipelineConstantBlock(Renderer::ShaderType shaderType) const
	{
		return mPipelineConstantBlocks[shaderType];
	}

	VkPipelineBindPoint PipelineLayout::GetBindPoint() const
	{
		return mBindPoint;
	}

	VkPipelineLayout PipelineLayout::GetHandle() const
	{
		return mHandle;
	}
}
