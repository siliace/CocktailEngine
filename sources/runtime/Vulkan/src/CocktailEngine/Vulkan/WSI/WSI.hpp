#ifndef COCKTAILENGINE_VULKAN_WSI_WSI_HPP
#define COCKTAILENGINE_VULKAN_WSI_WSI_HPP

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck
{
	class Window;

	namespace Vulkan::WSI
	{
		/**
		 * \brief
		 * \return
		 */
		const AnsiChar* GetSurfaceExtensionName();

		/**
		 * \brief
		 * \param instance
		 * \param window
		 * \param allocationCallbacks
		 * \return
		 */
		VkSurfaceKHR CreateWindowSurface(VkInstance instance, const Window& window, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 * \param instance
		 * \param allocationCallbacks
		 * \return
		 */
		VkSurfaceKHR CreateHeadlessSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief
		 * \param physicalDevice
		 * \param queueFamilyIndex
		 * \return
		 */
		bool GetPhysicalDevicePresentationSupport(VkPhysicalDevice physicalDevice, unsigned int queueFamilyIndex);
	};
}

#endif // COCKTAILENGINE_VULKAN_WSI_WSI_HPP
