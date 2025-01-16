#ifndef COCKTAIL_VULKAN_VULKANSERVICE_HPP
#define COCKTAIL_VULKAN_VULKANSERVICE_HPP

#include <Cocktail/Vulkan/Export.hpp>
#include <Cocktail/Vulkan/ApiVersion.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 * \param layerName 
	 * \return 
	 */
	bool COCKTAIL_VULKAN_API IsLayerSupported(const char* layerName);

	/**
	 * \brief Get the latest version of Vulkan supported by the graphic driver
	 * \return The latest version supported
	 */
	ApiVersion COCKTAIL_VULKAN_API GetSupportedApiVersion();
}

#endif // COCKTAIL_VULKAN_VULKAN_HPP
