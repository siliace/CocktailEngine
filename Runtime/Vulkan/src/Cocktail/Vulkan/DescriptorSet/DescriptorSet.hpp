#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP

#include <Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>

namespace Ck::Vulkan
{
	class DescriptorSetAllocator;
	class RenderDevice;

	class DescriptorSet : public Inherit<DescriptorSet, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param pool 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DescriptorSet(const Ref<RenderDevice>& renderDevice, const Ref<DescriptorPool>& pool, const DescriptorSetCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<DescriptorSetLayout> GetLayout() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDescriptorSet GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		Ref<DescriptorPool> mPool;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorSet mHandle;
		Ref<DescriptorSetLayout> mLayout;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
