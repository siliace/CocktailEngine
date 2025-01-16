#ifndef COCKTAIL_VULKAN_WSI_WSI_HPP
#define COCKTAIL_VULKAN_WSI_WSI_HPP

#include <Cocktail/Core/System/Window/Window.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan::WSI
{
	/**
	 * \brief 
	 * \return 
	 */
	const char* GetSurfaceExtensionName();

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
}

#endif // COCKTAIL_VULKAN_WSI_WSI_HPP
