#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOLCREATEINFO_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOLCREATEINFO_HPP

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutSignature.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Structure describing parameters to create a DescriptorPool
	 * \see DescriptorPool
	 */
	struct DescriptorPoolCreateInfo
	{
		/**
		 * \brief Specifies the maximal number of descriptor set the created pool can allocate
		 */
		unsigned int MaxSet = 16;

		/**
		 * \brief Specifies the signature of the descriptor set layout the descriptor pool can allocate
		 */
		DescriptorSetLayoutSignature LayoutSignature;

		/**
		 * \brief Specifies whether descriptor sets allocated from this pool can be freed individually 
		 */
		bool SupportFree = false;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOLCREATEINFO_HPP
