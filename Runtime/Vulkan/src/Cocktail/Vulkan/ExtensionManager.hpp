#ifndef COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP
#define COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP

#include <string>
#include <unordered_set>

#include <Cocktail/Renderer/RenderDeviceExtension.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	enum class RenderDeviceFeature
	{
		DedicatedAllocation,
		DescriptorUpdateTemplate,
		PushDescriptors,
		Synchronization2,
		RenderPass2,
		RenderPassDepthStencilResolve,
		ValidationCache,
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
		 */
		ExtensionManager();

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
		bool CheckExtensionsInstanceSupport(unsigned int extensionCount, const char* const* extensionNames) const;

		/**
		 * \brief
		 * \param physicalDevice
		 * \param extensionCount
		 * \param extensionNames
		 * \return
		 */
		bool CheckExtensionPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, unsigned int extensionCount, const char* const* extensionNames) const;

		/**
		 * \brief
		 * \param properties
		 * \param extensionCount
		 * \param extensionNames
		 * \return
		 */
		bool CheckExtensionSupport(const std::vector<VkExtensionProperties>& properties, unsigned int extensionCount, const char* const* extensionNames) const;

		/**
		 * \brief 
		 * \param extensionName 
		 * \return 
		 */
		bool IsExtensionDisabled(std::string_view extensionName) const;

		std::unordered_set<const char*> mInstanceExtensions;
		std::unordered_set<const char*> mDeviceExtensions;
		std::unordered_set<std::string> mDisabledExtensions;
	};
}

#endif // COCKTAIL_VULKAN_EXTENSIONMANAGER_HPP
