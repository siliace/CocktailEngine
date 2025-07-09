#ifndef COCKTAIL_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP
#define COCKTAIL_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP

#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetBindingInfo.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Structure describing a descriptor set in a shader stage
	 */
	struct DescriptorSetInfo
	{
		/**
		 * \brief 
		 */
		unsigned int Set = 0;

		/**
		 * \brief 
		 */
		Array<DescriptorSetBindingInfo> Bindings;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP
