#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP

namespace Ck::Vulkan
{
	class DescriptorSetLayout;
	class PipelineLayout;

	/**
	 * \brief 
	 */
	struct DescriptorUpdateTemplateCreateInfo
	{
		/**
		 * \brief 
		 */
		std::shared_ptr<DescriptorSetLayout> DescriptorSetLayout;
		
		/**
		 * \brief 
		 */
		PipelineLayout* PipelineLayout;

		/**
		 * \brief 
		 */
		unsigned int Set = 0;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP
