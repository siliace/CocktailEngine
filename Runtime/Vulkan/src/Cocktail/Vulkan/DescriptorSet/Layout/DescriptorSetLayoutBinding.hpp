#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTBINDING_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTBINDING_HPP

#include <Cocktail/Renderer/Shader/DescriptorType.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>

namespace Ck::Vulkan
{
	class Sampler;

	/**
	 * \brief 
	 */
	struct DescriptorSetLayoutBinding
	{
		/**
		 * \brief
		 */
		unsigned int Binding = 0;

		/**
		 * \brief
		 */
		Renderer::DescriptorType Type = Renderer::DescriptorType::UniformBuffer;

		/**
		 * \brief
		 */
		unsigned int DescriptorCount = 0;

		/**
		 * \brief
		 */
		Flags<Renderer::ShaderType> ShaderStages;

		/**
		 * \brief 
		 */
		const Sampler* StaticSampler = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTBINDING_HPP
