#include <Windows.h>

#include <Cocktail/Core/System/Window/Window.hpp>

#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan::WSI
{
	const AnsiChar* GetSurfaceExtensionName()
	{
		return VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
	}

	VkSurfaceKHR CreateWindowSurface(VkInstance instance, const Window& window, const VkAllocationCallbacks* allocationCallbacks)
	{
		VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, nullptr };
		{
			createInfo.flags = 0;
			createInfo.hinstance = GetModuleHandle(nullptr);
			createInfo.hwnd = static_cast<HWND>(window.GetSystemHandle());
		}

		VkSurfaceKHR surface;
		COCKTAIL_VK_CHECK(vkCreateWin32SurfaceKHR(instance, &createInfo, allocationCallbacks, &surface));

		return surface;
	}

	bool GetPhysicalDevicePresentationSupport(VkPhysicalDevice physicalDevice, unsigned int queueFamilyIndex)
	{
		return vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex) == VK_TRUE;
	}
}
