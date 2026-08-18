#ifndef COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP
#define COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP

#include <CocktailEngine/Vulkan/Shader/Reflection/DescriptorSetBindingInfo.hpp>

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

#endif // COCKTAILENGINE_VULKAN_SHADER_REFLECTION_DESCRIPTORSETINFO_HPP
