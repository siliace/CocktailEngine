#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutBinding.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct DescriptorSetLayoutCreateInfo
	{
		/**
		 * \brief 
		 */
		bool SupportPushDescriptor = false;

		/**
		 * \brief 
		 */
		unsigned int BindingCount = 0;

		/**
		 * \brief 
		 */
		const DescriptorSetLayoutBinding* Bindings = nullptr;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP
