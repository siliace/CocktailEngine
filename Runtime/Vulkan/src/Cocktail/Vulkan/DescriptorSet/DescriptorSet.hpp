#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP

#include <Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp>

namespace Ck::Vulkan
{
	class DescriptorSetAllocator;
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
		DescriptorSet(std::shared_ptr<RenderDevice> renderDevice, std::shared_ptr<DescriptorPool> pool, const DescriptorSetCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<DescriptorSetLayout> GetLayout() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDescriptorSet GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		std::shared_ptr<DescriptorPool> mPool;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorSet mHandle;
		std::shared_ptr<DescriptorSetLayout> mLayout;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORSET_HPP
