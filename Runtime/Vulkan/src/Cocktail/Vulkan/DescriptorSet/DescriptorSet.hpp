#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP

#include <Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp>

namespace Ck::Vulkan
{
	class DescriptorPool;
	class RenderDevice;

	class DescriptorSet : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param pool 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DescriptorSet(RenderDevice* renderDevice, SharedPtr<DescriptorPool> pool, const DescriptorSetCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~DescriptorSet() override;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<DescriptorSetLayout> GetLayout() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDescriptorSet GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		SharedPtr<DescriptorPool> mPool;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorSet mHandle;
		SharedPtr<DescriptorSetLayout> mLayout;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
