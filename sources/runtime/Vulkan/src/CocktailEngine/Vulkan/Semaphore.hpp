#ifndef COCKTAILENGINE_VULKAN_SEMAPHORE_HPP
#define COCKTAILENGINE_VULKAN_SEMAPHORE_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

#include <CocktailEngine/Vulkan/SemaphoreCreateInfo.hpp>
#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class Semaphore : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Semaphore(RenderDevice* renderDevice, const SemaphoreCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Semaphore() override;

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
		VkSemaphore GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSemaphore mHandle;
	};
}

#endif // COCKTAILENGINE_VULKAN_SEMAPHORE_HPP
