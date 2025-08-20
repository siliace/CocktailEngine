#ifndef COCKTAIL_VULKAN_VULKANSERVICE_HPP
#define COCKTAIL_VULKAN_VULKANSERVICE_HPP

#include <Cocktail/Core/Log/LogCategory.hpp>

#include <Cocktail/Vulkan/Export.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>

namespace Ck
{
	namespace Renderer
	{
		class RenderDevice;
	}

	namespace Vulkan
	{
		COCKTAIL_DECLARE_LOG_CATEGORY(VulkanLogCategory, LogLevel::Info);

		/**
		 * \brief
		 * \param layerName
		 * \return
		 */
		COCKTAIL_VULKAN_API bool IsLayerSupported(const char* layerName);

		/**
		 * \brief Get the latest version of Vulkan supported by the graphic driver
		 * \return The latest version supported
		 */
		COCKTAIL_VULKAN_API ApiVersion GetSupportedApiVersion();

		/**
		 * \brief Create a new instance of RenderDevice implemented using Vulkan Api
		 * \param createInfo Parameters of the RenderDevice to create
		 * \return The RenderDevice created
		 */
		COCKTAIL_VULKAN_API std::unique_ptr<Renderer::RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
	}
}

#endif // COCKTAIL_VULKAN_VULKAN_HPP
