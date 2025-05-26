#ifndef COCKTAIL_VULKAN_SEMAPHORE_HPP
#define COCKTAIL_VULKAN_SEMAPHORE_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/SemaphoreCreateInfo.hpp>

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
		Semaphore(std::shared_ptr<RenderDevice> renderDevice, const SemaphoreCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkSemaphore GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSemaphore mHandle;
	};
}

#endif // COCKTAIL_VULKAN_SEMAPHORE_HPP
