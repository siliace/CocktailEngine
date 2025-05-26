#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP

#include <vector>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutCreateInfo.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutSignature.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;
	
	/**
	 * \brief 
	 */
	class DescriptorSetLayout : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DescriptorSetLayout(std::shared_ptr<RenderDevice> renderDevice, const DescriptorSetLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~DescriptorSetLayout() override;

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
		 * \param other 
		 * \return 
		 */
		bool IsCompatibleWith(const DescriptorSetLayout& other) const;

		/**
		 * \brief 
		 * \return 
		 */
		bool SupportPushDescriptor() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetBindingCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetDescriptorCount() const;
		
		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const DescriptorSetLayoutBinding* GetBinding(unsigned int index) const;

		/**
		 * \brief 
		 * \return 
		 */
		DescriptorSetLayoutSignature ToSignature() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDescriptorSetLayout GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorSetLayout mHandle;
		bool mSupportPushDescriptor;
		std::vector<DescriptorSetLayoutBinding> mBindings;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP
