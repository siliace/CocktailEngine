#ifndef COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP

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
		SharedPtr<DescriptorSetLayout> InDescriptorSetLayout;
		
		/**
		 * \brief 
		 */
		PipelineLayout* InPipelineLayout;

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

#endif // COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATECREATEINFO_HPP
