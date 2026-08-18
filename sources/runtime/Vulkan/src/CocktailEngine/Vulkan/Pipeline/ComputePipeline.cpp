#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/VulkanUtils.hpp>
#include <CocktailEngine/Vulkan/Pipeline/ComputePipeline.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineCache.hpp>
#include <CocktailEngine/Vulkan/Shader/Shader.hpp>

namespace Ck::Vulkan
{
	ComputePipeline::ComputePipeline(RenderDevice* renderDevice, const PipelineCache* pipelineCache, const ComputePipelineCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks) :
		Pipeline(renderDevice),
		mAllocationCallbacks(allocationCallbacks),
		mHandle(VK_NULL_HANDLE)
	{
		mLayout = createInfo.Layout;

		VkPipelineShaderStageCreateInfo shaderStageCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr };
		{
			shaderStageCreateInfo.flags = 0;
			shaderStageCreateInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
			shaderStageCreateInfo.module = createInfo.State.ComputeStageState.StageShader->GetHandle();
			shaderStageCreateInfo.pName = createInfo.State.ComputeStageState.EntryPoint;
		}

		// Declared before the create info it chains into, so that the storage the driver writes
		// into outlives the vkCreateComputePipelines call below.
		PipelineCreationFeedbackCollector creationFeedbackCollector(mRenderDevice, 1);

		VkComputePipelineCreateInfo vkCreateInfo{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO, nullptr };
		{
			vkCreateInfo.flags = 0;
			vkCreateInfo.stage = shaderStageCreateInfo;
			vkCreateInfo.layout = mLayout->GetHandle();
		}
		creationFeedbackCollector.ChainTo(vkCreateInfo);

		VkPipelineCache pipelineCacheHandle = pipelineCache ? pipelineCache->GetHandle() : VK_NULL_HANDLE;
		COCKTAIL_VK_CHECK(vkCreateComputePipelines(mRenderDevice->GetHandle(), pipelineCacheHandle, 1, &vkCreateInfo, mAllocationCallbacks, &mHandle));

		mCreationFeedback = creationFeedbackCollector.Collect();
	}

	ComputePipeline::~ComputePipeline()
	{
		vkDestroyPipeline(mRenderDevice->GetHandle(), mHandle, mAllocationCallbacks);
	}

	SharedPtr<PipelineLayout> ComputePipeline::GetLayout() const
	{
		return mLayout;
	}
	
	VkPipeline ComputePipeline::GetHandle() const
	{
		return mHandle;
	}
}
