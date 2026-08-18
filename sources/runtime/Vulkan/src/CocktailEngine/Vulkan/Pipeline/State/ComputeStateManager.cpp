#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/Pipeline/ComputePipelineCreateInfo.hpp>
#include <CocktailEngine/Vulkan/Pipeline/PipelineManager.hpp>
#include <CocktailEngine/Vulkan/Pipeline/State/ComputeStateManager.hpp>
#include <CocktailEngine/Vulkan/Shader/ShaderProgram.hpp>

namespace Ck::Vulkan
{
	ComputeStateManager::ComputeStateManager(RenderDevice* renderDevice, DescriptorSetAllocator* descriptorSetAllocator) :
		StateManager(renderDevice, Move(descriptorSetAllocator)),
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
			mState.ComputeStageState.StageShader = mShaderStages[Renderer::ShaderType::Compute].Get();
		}
	}

	void ComputeStateManager::SetEntryPoint(Renderer::ShaderType shaderType, const char* name)
	{
		mDirtyFlags |= DirtyFlagBits::Pipeline;
		mState.ComputeStageState.EntryPoint = name;
	}

	SharedPtr<Pipeline> ComputeStateManager::CompilePipeline()
	{
		assert(mShaderProgram);

		ComputePipelineCreateInfo createInfo;
		createInfo.Layout = mShaderProgram->GetPipelineLayout();
		createInfo.State = mState;

		mDirtyFlags &= ~DirtyFlagBits::Pipeline;

		return mRenderDevice->Invoke([&](PipelineManager* pipelineManager) {
			return pipelineManager->CreateComputePipeline(createInfo);
		});
	}
}
