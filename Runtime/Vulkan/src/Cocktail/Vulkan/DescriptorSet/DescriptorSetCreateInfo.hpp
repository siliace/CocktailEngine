#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>

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
		std::shared_ptr<DescriptorSetLayout> Layout;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSETCREATEINFO_HPP
