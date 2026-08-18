#ifndef COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP

#include <CocktailEngine/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct DescriptorSetCreateInfo
	{
		/**
		 * \brief 
		 */
		SharedPtr<DescriptorSetLayout> Layout;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP
