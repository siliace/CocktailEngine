#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Pipeline/ComputePipeline.hpp>
#include <Cocktail/Vulkan/Pipeline/PipelineCache.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>

namespace Ck::Vulkan
{
	ComputePipeline::ComputePipeline(std::shared_ptr<RenderDevice> renderDevice, const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Pipeline(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mLayout = createInfo.PipelineLayout;

		VkPipelineShaderStageCreateInfo shaderStageCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr };
		{
			shaderStageCreateInfo.flags = 0;
			shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
			shaderStageCreateInfo.module = createInfo.ComputeState.ComputeStageState.Shader->GetHandle();
			shaderStageCreateInfo.pName = createInfo.ComputeState.ComputeStageState.EntryPoint;
		}

		VkComputePipelineCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.stage = shaderStageCreateInfo;
			vkCreateInfo.layout = mLayout->GetHandle();
		}

		VkPipelineCache pipelineCacheHandle = pipelineCache ? pipelineCache->GetHandle() : VK_NULL_HANDLE;
		COCKTAIL_VK_CHECK(vkCreateComputePipelines(mRenderDevice->GetHandle(), pipelineCacheHandle, 1, &vkCreateInfo, mAllocationCallbacks, &mHandle));
	}

	ComputePipeline::~ComputePipeline()
	{
		vkDestroyPipeline(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	std::shared_ptr<PipelineLayout> ComputePipeline::GetLayout() const
	{
		return mLayout;
	}
	
	VkPipeline ComputePipeline::GetHandle() const
	{
		return mHandle;
	}
}
