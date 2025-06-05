#include <Cocktail/Core/System/Window/Window.hpp>

#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan::WSI
{
	struct WindowHandle
	{
		::Display* Display;
		::Visual Visual;
		::Colormap Colormap;
		::Window Window;
	};

	const char* GetSurfaceExtensionName()
	{
		return VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
	}

	VkSurfaceKHR CreateWindowSurface(VkInstance instance, const Window& window, const VkAllocationCallbacks* allocationCallbacks)
	{
		WindowHandle* windowHandle = static_cast<WindowHandle*>(window.GetSystemHandle());

		VkXlibSurfaceCreateInfoKHR createInfo { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, nullptr };
		{
			createInfo.flags = 0;
			createInfo.dpy = windowHandle->Display;
			createInfo.window = windowHandle->Window;
		}

		VkSurfaceKHR surface;
		COCKTAIL_VK_CHECK(vkCreateXlibSurfaceKHR(instance, &createInfo, allocationCallbacks, &surface));

		return surface;
	}

	bool GetPhysicalDevicePresentationSupport(VkPhysicalDevice physicalDevice, unsigned int queueFamilyIndex)
	{
		::Display* display = XOpenDisplay(nullptr);
		::Visual* visual = DefaultVisual(display, DefaultScreen(display));
		::VisualID visualId = XVisualIDFromVisual(visual);

		bool supported = vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, display, visualId) == VK_TRUE;
	
		XCloseDisplay(display);
	
		return supported;
	}
}
