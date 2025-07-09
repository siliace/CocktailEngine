#ifndef COCKTAIL_VULKAN_SHADER_UNIFORMSLOT_HPP
#define COCKTAIL_VULKAN_SHADER_UNIFORMSLOT_HPP

#include <memory>

#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutBinding.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetBindingInfo.hpp>

namespace Ck::Vulkan
{
	class UniformSlot : public Renderer::UniformSlot
	{
	public:

		/**
		 * \brief 
		 * \param programType
		 * \param members 
		 * \param name 
		 */
		UniformSlot(Renderer::ShaderProgramType programType, const Array<BlockMember>& members, std::string name, const DescriptorSetLayoutBinding& layoutBindingInfo, unsigned int set);

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ShaderProgramType GetProgramType() const override;
		
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
		std::size_t GetMemberCount() const override;

		/**
		 * \brief 
		 * \param members 
		 * \param memberCount 
		 * \param firstMember 
		 * \return 
		 */
		std::size_t GetMembers(Renderer::UniformMember** members, std::size_t memberCount, std::size_t firstMember = 0) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Flags<Renderer::ShaderType> GetShaderStages() const override;

		/**
		 * \brief
		 * \return
		 */
		const std::string& GetName() const override;

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

		Renderer::ShaderProgramType mProgramType;
		Array<std::unique_ptr<Renderer::UniformMember>> mMembers;
		std::string mName;
		DescriptorSetLayoutBinding mLayoutBindingInfo;
		unsigned int mSet;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_UNIFORMSLOT_HPP
