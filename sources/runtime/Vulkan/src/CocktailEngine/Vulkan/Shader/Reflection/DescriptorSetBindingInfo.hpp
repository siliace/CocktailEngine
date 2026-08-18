#ifndef COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETBINDINGINFO_HPP
#define COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETBINDINGINFO_HPP

#include <CocktailEngine/Renderer/Shader/DescriptorType.hpp>

#include <CocktailEngine/Vulkan/Shader/Reflection/BlockMember.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Structure describing a binding within a descriptor set
	 */
	struct DescriptorSetBindingInfo
	{
		/**
		 * \brief The binding number in the set of the binding
		 */
		unsigned int Binding = 0;

		/**
		 * \brief The type of descriptor
		 */
		Renderer::DescriptorType Type = Renderer::DescriptorType::UniformBuffer;

		/**
		 * \brief 
		 */
		Array<BlockMember> Members;

		/**
		 * \brief
		 */
		AsciiString Name;

		/**
		 * \brief The array size of the descriptor
		 * If the binding is not an array, this value must be 1
		 */
		unsigned int ArrayLength = 0;
	};
}

#endif // COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETBINDINGINFO_HPP
