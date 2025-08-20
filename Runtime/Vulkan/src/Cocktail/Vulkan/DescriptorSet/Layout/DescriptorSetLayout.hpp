#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP

#include <Cocktail/Core/Array.hpp>

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
		DescriptorSetLayout(RenderDevice* renderDevice, const DescriptorSetLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

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
		unsigned int GetDescriptorCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		const Array<DescriptorSetLayoutBinding>& GetBindings() const;

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

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorSetLayout mHandle;
		bool mSupportPushDescriptor;
		Array<DescriptorSetLayoutBinding> mBindings;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_LAYOUT_DESCRIPTORSETLAYOUT_HPP
