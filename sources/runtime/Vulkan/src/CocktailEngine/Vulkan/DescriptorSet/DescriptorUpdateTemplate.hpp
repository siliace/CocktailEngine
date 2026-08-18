#ifndef COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP
#define COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/DescriptorSet/DescriptorUpdateTemplateCreateInfo.hpp>

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
		SharedPtr<DescriptorSetLayout> mDescriptorSetLayout;
	};
}

#endif // COCKTAILENGINE_VULKAN_DESCRIPTORSET_DESCRIPTORUPDATETEMPLATE_HPP
