#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplateCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	class DescriptorUpdateTemplate : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		DescriptorUpdateTemplate(RenderDevice* renderDevice, const DescriptorUpdateTemplateCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 */
		~DescriptorUpdateTemplate();

		/**
		 * \brief
		 * \param name
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief
		 * \return
		 */
		std::size_t GetElementStride() const;

		/**
		 * \brief
		 * \return
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief
		 * \return
		 */
		VkDescriptorUpdateTemplateKHR GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		VkDescriptorUpdateTemplateKHR mHandle;
		const VkAllocationCallbacks* mAllocationCallbacks;
		std::shared_ptr<DescriptorSetLayout> mDescriptorSetLayout;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP
