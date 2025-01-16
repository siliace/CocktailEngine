#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>

namespace Ck::Vulkan
{
	bool IsLayerSupported(const char* layerName)
	{
		return ExtensionManager::IsLayerSupported(layerName);
	}

	ApiVersion GetSupportedApiVersion() 
	{
		unsigned int version = 0;
		COCKTAIL_VK_CHECK(vkEnumerateInstanceVersion(&version));

		if (VK_API_VERSION_MAJOR(version) == 1)
		{
			if (VK_API_VERSION_MINOR(version) == 0)
				return ApiVersion::Version_1_0;

			if (VK_API_VERSION_MINOR(version) == 1)
				return ApiVersion::Version_1_1;

			if (VK_API_VERSION_MINOR(version) == 2)
				return ApiVersion::Version_1_2;

			if (VK_API_VERSION_MINOR(version) == 3)
				return ApiVersion::Version_1_3;
		}

		// Fallback :
		// TODO: find a better way to handle this ? Is it even possible to reach this place ? Maybe with a shitty driver ?
		return ApiVersion::Version_1_0;
	}
}
