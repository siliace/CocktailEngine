#ifndef COCKTAILENGINE_VULKAN_VULKANSERVICE_HPP
#define COCKTAILENGINE_VULKAN_VULKANSERVICE_HPP

#include <CocktailEngine/Core/Log/LogCategory.hpp>

#include <CocktailEngine/Vulkan/Export.hpp>
#include <CocktailEngine/Vulkan/RenderDeviceCreateInfo.hpp>

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
		COCKTAILENGINE_VULKAN_API bool IsLayerSupported(const char* layerName);

		/**
		 * \brief Get the latest version of Vulkan supported by the graphic driver
		 * \return The latest version supported
		 */
		COCKTAILENGINE_VULKAN_API VulkanApiVersion GetSupportedApiVersion();

		/**
		 * \brief Create a new instance of RenderDevice implemented using Vulkan Api
		 * \param createInfo Parameters of the RenderDevice to create
		 * \return The RenderDevice created
		 */
		COCKTAILENGINE_VULKAN_API UniquePtr<Renderer::RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo);
	}
}

#endif // COCKTAILENGINE_VULKAN_VULKAN_HPP
