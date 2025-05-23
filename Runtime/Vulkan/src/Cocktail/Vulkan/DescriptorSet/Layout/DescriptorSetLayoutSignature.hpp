#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTSIGNATURE_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTSIGNATURE_HPP

namespace Ck::Vulkan
{
	/**
	 * \brief Structure describing a DescriptorSetLayout by its number of descriptors and flags
	 */
	struct DescriptorSetLayoutSignature
	{
		/**
		 * \brief Specifies whether the DescriptorSetLayout use push descriptors
		 */
		bool SupportPushDescriptor = false;

		/**
		 * \brief Specifies the number of sampler descriptors
		 */
		unsigned int SamplerCount = 0;

		/**
		 * \brief Specifies the number of combined image sampler descriptors
		 */
		unsigned int TextureSamplerCount = 0;

		/**
		 * \brief Specifies the number of image descriptors
		 */
		unsigned int TextureCount = 0;

		/**
		 * \brief Specifies the number of image descriptors
		 */
		unsigned int StorageTextureCount = 0;

		/**
		 * \brief Specifies the number of uniform buffer descriptors
		 */
		unsigned int UniformBufferCount = 0;

		/**
		 * \brief Specifies the number of storage buffer descriptors
		 */
		unsigned int StorageBufferCount = 0;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUTSIGNATURE_HPP
