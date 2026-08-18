#ifndef COCKTAILENGINE_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP

#include <CocktailEngine/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutBinding.hpp>

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

#endif // COCKTAILENGINE_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTCREATEINFO_HPP
