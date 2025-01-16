#ifndef COCKTAIL_VULKAN_SHADER_SLOT_DESCRIPTORSETSLOT_HPP
#define COCKTAIL_VULKAN_SHADER_SLOT_DESCRIPTORSETSLOT_HPP

#include <Cocktail/Vulkan/Shader/Slot/UniformSlot.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief Implementation of UniformLocation describing an uniform binding in a descriptor set
	 */
	class DescriptorSetSlot : public Extends<DescriptorSetSlot, UniformSlot>
	{
	public:
		
		/**
		 * \brief 
		 * \param programType
		 * \param members 
		 * \param name 
		 * \param layoutBindingInfo
		 * \param set 
		 */
		explicit DescriptorSetSlot(Renderer::ShaderProgramType programType, const std::vector<BlockMember>& members, std::string name, const DescriptorSetLayoutBinding* layoutBindingInfo, unsigned int set);
		
		/**
		 * \brief 
		 * \return 
		 */
		Renderer::DescriptorType GetDescriptorType() const override;

		/**
		 * \brief
		 * \return
		 */
		bool IsArray() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetArrayLength() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Flags<Renderer::ShaderType> GetShaderStages() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetSet() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetBinding() const;

	private:

		const DescriptorSetLayoutBinding* mLayoutBindingInfo;
		unsigned int mSet;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SLOT_DESCRIPTORSETSLOT_HPP
