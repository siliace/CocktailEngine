#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan::WSI
{
	VkSurfaceKHR CreateHeadlessSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallbacks)
	{
		VkHeadlessSurfaceCreateInfoEXT createInfo{ VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT, nullptr };
		{
			createInfo.flags = 0;
		}

		VkSurfaceKHR surface;
		vkCreateHeadlessSurfaceEXT(instance, &createInfo, allocationCallbacks, &surface);

		return surface;
	}

}