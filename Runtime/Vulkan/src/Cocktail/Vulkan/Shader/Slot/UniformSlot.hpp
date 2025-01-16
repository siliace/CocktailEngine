#ifndef COCKTAIL_VULKAN_SHADER_SLOT_UNIFORMSLOT_HPP
#define COCKTAIL_VULKAN_SHADER_SLOT_UNIFORMSLOT_HPP

#include <memory>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetBindingInfo.hpp>

namespace Ck::Vulkan
{
	class UniformSlot : public Inherit<UniformSlot, Object, Renderer::UniformSlot>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ShaderProgramType GetProgramType() const override;

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
		const std::string& GetName() const override;

	protected:

		/**
		 * \brief 
		 * \param programType
		 * \param members 
		 * \param name 
		 */
		UniformSlot(Renderer::ShaderProgramType programType, const std::vector<BlockMember>& members, std::string name);

	private:

		Renderer::ShaderProgramType mProgramType;
		std::vector<Ref<Renderer::UniformMember>> mMembers;
		std::string mName;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SLOT_UNIFORMSLOT_HPP
