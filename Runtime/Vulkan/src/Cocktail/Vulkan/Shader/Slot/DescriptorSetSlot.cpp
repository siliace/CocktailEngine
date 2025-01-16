#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Shader/Slot/DescriptorSetSlot.hpp>

namespace Ck::Vulkan
{
	DescriptorSetSlot::DescriptorSetSlot(Renderer::ShaderProgramType programType, const std::vector<BlockMember>& members, std::string name, const DescriptorSetLayoutBinding* layoutBindingInfo, unsigned int set) :
		Super(programType, members, std::move(name)),
		mLayoutBindingInfo(layoutBindingInfo),
		mSet(set)
	{
		assert(mLayoutBindingInfo);
	}

	Renderer::DescriptorType DescriptorSetSlot::GetDescriptorType() const
	{
		return mLayoutBindingInfo->Type;
	}

	bool DescriptorSetSlot::IsArray() const
	{
		return mLayoutBindingInfo->DescriptorCount > 1;
	}

	unsigned int DescriptorSetSlot::GetArrayLength() const
	{
		return mLayoutBindingInfo->DescriptorCount;
	}

	Flags<Renderer::ShaderType> DescriptorSetSlot::GetShaderStages() const
	{
		return mLayoutBindingInfo->ShaderStages;
	}
	
	unsigned int DescriptorSetSlot::GetSet() const
	{
		return mSet;
	}

	unsigned int DescriptorSetSlot::GetBinding() const
	{
		return mLayoutBindingInfo->Binding;
	}
}
