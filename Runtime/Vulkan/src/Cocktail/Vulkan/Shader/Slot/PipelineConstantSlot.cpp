#include <Cocktail/Vulkan/Shader/Slot/PipelineConstantSlot.hpp>

namespace Ck::Vulkan
{
	PipelineConstantSlot::PipelineConstantSlot(Renderer::ShaderProgramType programType, Renderer::ShaderType shaderType, const PushConstantBlockInfo& pushConstantBlock) :
		Super(programType, pushConstantBlock.Members, pushConstantBlock.Name),
		mShaderType(shaderType),
		mBaseOffset(pushConstantBlock.BaseOffset)
	{
		/// Nothing
	}

	unsigned int PipelineConstantSlot::ComputeOffset(unsigned int offset) const
	{
		return mBaseOffset + offset;
	}

	Renderer::DescriptorType PipelineConstantSlot::GetDescriptorType() const
	{
		return Renderer::DescriptorType::PipelineConstants;
	}

	bool PipelineConstantSlot::IsArray() const
	{
		return false;
	}

	unsigned int PipelineConstantSlot::GetArrayLength() const
	{
		return 1;
	}

	Flags<Renderer::ShaderType> PipelineConstantSlot::GetShaderStages() const
	{
		return mShaderType;
	}
}
