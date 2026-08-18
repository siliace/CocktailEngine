#ifndef COCKTAILENGINE_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOL_HPP
#define COCKTAILENGINE_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOL_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/DescriptorSet/Allocator/DescriptorPoolCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class DescriptorPool : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		DescriptorPool(RenderDevice* renderDevice, const DescriptorPoolCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief Destructor
		 */
		~DescriptorPool() override;

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
		 */
		void Reset() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool SupportFree() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkDescriptorPool GetHandle() const;

	private:

		/**
		 * \brief 
		 * \param sizes 
		 * \param index 
		 * \param type 
		 * \param descriptorCount 
		 * \param setCount 
		 */
		static void PushPoolSize(VkDescriptorPoolSize* sizes, unsigned int& index, Renderer::DescriptorType type, unsigned int descriptorCount, unsigned int setCount);

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkDescriptorPool mHandle;
		bool mSupportFree;
	};
}

#endif // COCKTAILENGINE_VULKAN_DESCRIPTORSET_ALLOCATOR_DESCRIPTORPOOL_HPP
