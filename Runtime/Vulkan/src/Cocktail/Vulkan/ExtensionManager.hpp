#ifndef COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP
#define COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP

#include <string_view>
#include <vector>

#include <volk.h>

#include <Cocktail/Renderer/RenderDeviceExtension.hpp>

namespace Ck::Vulkan
{
	enum class RenderDeviceFeature
	{
		DedicatedAllocation,
		DescriptorUpdateTemplate,
		MemoryPriority,
		PushDescriptors,
		Synchronization2,
		RenderPass2,
		RenderPassDepthStencilResolve,
		WideLine,
	};

	class ExtensionManager
	{
	public:

		/**
		 * \brief
		 * \param layerName
		 * \return
		 */
		static bool IsLayerSupported(const char* layerName);

		/**
		 * \brief
		 * \param feature
		 * \return
		 */
		bool EnableInstanceExtension(RenderDeviceFeature feature);

		/**
		 * \brief
		 * \param extension
		 * \return
		 */
		bool EnableInstanceExtension(Renderer::RenderDeviceExtension extension);

		/**
		 * \brief
		 * \param feature
		 * \param physicalDevice
		 * \return
		 */
		bool EnableDeviceExtension(RenderDeviceFeature feature, VkPhysicalDevice physicalDevice);

		/**
		 * \brief
		 * \param extension
		 * \param physicalDevice
		 * \return
		 */
		bool EnableDeviceExtension(Renderer::RenderDeviceExtension extension, VkPhysicalDevice physicalDevice);

		/**
		 * \brief
		 * \param extensionName
		 * \return
		 */
		bool IsSupportedInstanceExtension(std::string_view extensionName) const;
		
		/**
		 * \brief 
		 * \param extensionName 
		 * \return 
		 */
		bool IsSupportedDeviceExtension(std::string_view extensionName) const;

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<const char*> GetInstanceExtensions() const;

		/**
		 * \brief 
		 * \return 
		 */
		std::vector<const char*> GetDeviceExtensions() const;

	private:

		/**
		 * \brief
		 * \param extensionCount
		 * \param extensionNames
		 * \return
		 */
		static bool CheckExtensionsInstanceSupport(unsigned int extensionCount, const char* const* extensionNames);

		/**
		 * \brief
		 * \param physicalDevice
		 * \param extensionCount
		 * \param extensionNames
		 * \return
		 */
		static bool CheckExtensionPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, unsigned int extensionCount, const char* const* extensionNames);

		/**
		 * \brief
		 * \param properties
		 * \param extensionCount
		 * \param extensionNames
		 * \return
		 */
		static bool CheckExtensionSupport(const std::vector<VkExtensionProperties>& properties, unsigned int extensionCount, const char* const* extensionNames);

		std::vector<const char*> mInstanceExtensions;
		std::vector<const char*> mDeviceExtensions;
	};
}

#endif // COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP
