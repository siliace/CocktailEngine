#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Pipeline/ComputePipelineCreateInfo.hpp>
#include <Cocktail/Vulkan/Pipeline/State/ComputeStateManager.hpp>

namespace Ck::Vulkan
{
	ComputeStateManager::ComputeStateManager(Ref<RenderDevice> renderDevice, Ref<DescriptorSetAllocator> descriptorSetAllocator) :
		StateManager(std::move(renderDevice), std::move(descriptorSetAllocator)),
		mShaderProgram(nullptr)
	{
		/// Nothing
	}

	void ComputeStateManager::SetShaderProgram(const ShaderProgram* shaderProgram)
	{
		assert(shaderProgram->GetType() == Renderer::ShaderProgramType::Compute);

		if (CheckedAssign(mShaderProgram, shaderProgram))
		{
			StateManager::SetShaderProgram(mShaderProgram);

			mDirtyFlags |= DirtyFlagBits::Pipeline;
			mState.ComputeStageState.Shader = mShaderStages[Renderer::ShaderType::Compute].Get();
		}
	}

	void ComputeStateManager::SetEntryPoint(Renderer::ShaderType shaderType, const char* name)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.ComputeStageState.EntryPoint = name;
	}

	Ref<Pipeline> ComputeStateManager::CompilePipeline()
	{
		assert(mShaderProgram);

		ComputePipelineCreateInfo createInfo;
		createInfo.PipelineLayout = mShaderProgram->GetPipelineLayout();
		createInfo.ComputeState = mState;

		mDirtyFlags &= ~DirtyFlagBits::Pipeline;

		return mRenderDevice->Invoke([&](PipelineCache* pipelineCache) {
			return pipelineCache->CreateComputePipeline(createInfo);
		});
	}
}
