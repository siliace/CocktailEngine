#include <algorithm>

#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/WSI/WSI.hpp>

namespace Ck::Vulkan
{
	template <Renderer::RenderDeviceExtension>
	struct ExtensionRequirement
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions = nullptr;

		inline static unsigned int DeviceExtensionCount = 0;
		inline static const char** DeviceExtensions = nullptr;
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::RenderSurface>
	{
		inline static unsigned int InstanceExtensionCount = 2;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_SURFACE_EXTENSION_NAME,
			WSI::GetSurfaceExtensionName()
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::RenderSurfaceColorSpace>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_SURFACE_EXTENSION_NAME,
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME
		};
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::Debug>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME
		};

		inline static unsigned int DeviceExtensionCount = 0;
		inline static const char** DeviceExtensions = nullptr;
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::InstanceDivisor>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME
		};
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::MutableTextureFormat>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions = nullptr;

		inline static unsigned int DeviceExtensionCount = 4;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_MAINTENANCE2_EXTENSION_NAME,
			VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
			VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
		};
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::ByteIndexType>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions = nullptr;

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_EXT_INDEX_TYPE_UINT8_EXTENSION_NAME,
		};
	};

	template <>
	struct ExtensionRequirement<Renderer::RenderDeviceExtension::TimelineSynchronization>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,
		};
	};

	template <RenderDeviceFeature>
	struct FeatureRequirement
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions = nullptr;

		inline static unsigned int DeviceExtensionCount = 0;
		inline static const char** DeviceExtensions = nullptr;
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::DedicatedAllocation>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions = nullptr;

		inline static unsigned int DeviceExtensionCount = 2;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,
			VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::DescriptorUpdateTemplate>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions;

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::PushDescriptors>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::Synchronization2>
	{
		inline static unsigned int InstanceExtensionCount = 1;
		inline static const char* InstanceExtensions[] = {
			VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		};

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::RenderPass2>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions;

		inline static unsigned int DeviceExtensionCount = 3;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_MULTIVIEW_EXTENSION_NAME,
			VK_KHR_MAINTENANCE2_EXTENSION_NAME,
			VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::RenderPassDepthStencilResolve>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions;

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
		};
	};

	template <>
	struct FeatureRequirement<RenderDeviceFeature::ValidationCache>
	{
		inline static unsigned int InstanceExtensionCount = 0;
		inline static const char** InstanceExtensions;

		inline static unsigned int DeviceExtensionCount = 1;
		inline static const char* DeviceExtensions[] = {
			VK_EXT_VALIDATION_CACHE_EXTENSION_NAME,
		};
	};

	bool ExtensionManager::IsLayerSupported(const char* layerName)
	{
		unsigned int propertyCount;
		vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);

		Array<VkLayerProperties> properties(propertyCount);
		vkEnumerateInstanceLayerProperties(&propertyCount, properties.GetData());

		for (const VkLayerProperties& property : properties)
		{
			if (std::strcmp(layerName, property.layerName) == 0)
				return true;
		}

		return false;
	}

#ifndef NDEBUG
	ExtensionManager::ExtensionManager()
	{
		StringUtils::Split(App::GetEnvironmentVariable("COCKTAIL_VULKAN_DISABLED_EXTENSIONS"), ';').ForEach([&](const std::string& disabledExtension) {
			mDisabledExtensions.emplace(disabledExtension);
			CK_LOG(VulkanLogCategory, LogLevel::Info, "Extension {} disabled", disabledExtension);
		});
	}
#else
	ExtensionManager::ExtensionManager() = default;
#endif

	bool ExtensionManager::EnableInstanceExtension(RenderDeviceFeature feature)
	{
		unsigned int extensionCount = 0;
		const char* const* extensionNames = nullptr;
#define CASE_FEATURE(__Feature)                                                      \
		case __Feature:                                                              \
			{                                                                        \
			 extensionCount = FeatureRequirement<__Feature>::InstanceExtensionCount; \
			 extensionNames = FeatureRequirement<__Feature>::InstanceExtensions;     \
			}                                                                        \
			break;

		switch (feature)
		{
		CASE_FEATURE(RenderDeviceFeature::DedicatedAllocation)
		CASE_FEATURE(RenderDeviceFeature::DescriptorUpdateTemplate)
		CASE_FEATURE(RenderDeviceFeature::PushDescriptors)
		CASE_FEATURE(RenderDeviceFeature::Synchronization2)
		CASE_FEATURE(RenderDeviceFeature::RenderPass2)
		CASE_FEATURE(RenderDeviceFeature::RenderPassDepthStencilResolve)
		CASE_FEATURE(RenderDeviceFeature::ValidationCache)
		}
#undef CASE_FEATURE

		if (!CheckExtensionsInstanceSupport(extensionCount, extensionNames))
			return false;

		for (unsigned int i = 0; i < extensionCount; i++)
			mInstanceExtensions.insert(extensionNames[i]);

		return true;
	}

	bool ExtensionManager::EnableInstanceExtension(Renderer::RenderDeviceExtension extension)
	{
		unsigned int extensionCount = 0;
		const char* const* extensionNames = nullptr;
#define CASE_EXTENSION(__Extension)                                                      \
		case __Extension:                                                                \
			{                                                                            \
			 extensionCount = ExtensionRequirement<__Extension>::InstanceExtensionCount; \
			 extensionNames = ExtensionRequirement<__Extension>::InstanceExtensions;     \
			}                                                                            \
			break;

		switch (extension)
		{
		CASE_EXTENSION(Renderer::RenderDeviceExtension::RenderSurface)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::RenderSurfaceColorSpace)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::Debug)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::InstanceDivisor)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::MutableTextureFormat)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::TextureView2DArrayCompatible)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::ByteIndexType)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::TimelineSynchronization)
		}
#undef CASE_EXTENSION

		if (!CheckExtensionsInstanceSupport(extensionCount, extensionNames))
			return false;

		for (unsigned int i = 0; i < extensionCount; i++)
			mInstanceExtensions.insert(extensionNames[i]);

		return true;
	}

	bool ExtensionManager::EnableDeviceExtension(RenderDeviceFeature feature, VkPhysicalDevice physicalDevice)
	{
		unsigned int extensionCount = 0;
		const char* const* extensionNames = nullptr;
#define CASE_FEATURE(__Feature)                                                    \
		case __Feature:                                                            \
			{                                                                      \
			 extensionCount = FeatureRequirement<__Feature>::DeviceExtensionCount; \
			 extensionNames = FeatureRequirement<__Feature>::DeviceExtensions;     \
			}                                                                      \
			break;

		switch (feature)
		{
		CASE_FEATURE(RenderDeviceFeature::DedicatedAllocation)
		CASE_FEATURE(RenderDeviceFeature::DescriptorUpdateTemplate)
		CASE_FEATURE(RenderDeviceFeature::PushDescriptors)
		CASE_FEATURE(RenderDeviceFeature::Synchronization2)
		CASE_FEATURE(RenderDeviceFeature::RenderPass2)
		CASE_FEATURE(RenderDeviceFeature::RenderPassDepthStencilResolve)
		CASE_FEATURE(RenderDeviceFeature::ValidationCache)
		}
#undef CASE_FEATURE

		if (!CheckExtensionPhysicalDeviceSupport(physicalDevice, extensionCount, extensionNames))
			return false;

		for (unsigned int i = 0; i < extensionCount; i++)
			mDeviceExtensions.insert(extensionNames[i]);

		return true;
	}

	bool ExtensionManager::EnableDeviceExtension(Renderer::RenderDeviceExtension extension, VkPhysicalDevice physicalDevice)
	{
		unsigned int extensionCount = 0;
		const char* const* extensionNames = nullptr;
#define CASE_EXTENSION(__Extension)                                                    \
		case __Extension:                                                              \
			{                                                                          \
			 extensionCount = ExtensionRequirement<__Extension>::DeviceExtensionCount; \
			 extensionNames = ExtensionRequirement<__Extension>::DeviceExtensions;     \
			}                                                                          \
			break;

		switch (extension)
		{
		CASE_EXTENSION(Renderer::RenderDeviceExtension::RenderSurface)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::RenderSurfaceColorSpace)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::Debug)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::InstanceDivisor)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::MutableTextureFormat)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::TextureView2DArrayCompatible)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::ByteIndexType)
		CASE_EXTENSION(Renderer::RenderDeviceExtension::TimelineSynchronization)
		}
#undef CASE_EXTENSION

		if (!CheckExtensionPhysicalDeviceSupport(physicalDevice, extensionCount, extensionNames))
			return false;

		for (unsigned int i = 0; i < extensionCount; i++)
			mDeviceExtensions.insert(extensionNames[i]);

		return true;
	}

	bool ExtensionManager::IsSupportedInstanceExtension(std::string_view extensionName) const
	{
		return std::find(mInstanceExtensions.begin(), mInstanceExtensions.end(), extensionName) != mInstanceExtensions.end();
	}

	bool ExtensionManager::IsSupportedDeviceExtension(std::string_view extensionName) const
	{
		return std::find(mDeviceExtensions.begin(), mDeviceExtensions.end(), extensionName) != mDeviceExtensions.end();
	}

	Array<const char*> ExtensionManager::GetInstanceExtensions() const
	{
		Array<const char*> extensions;
		for (auto it = mInstanceExtensions.begin(); it != mInstanceExtensions.end(); ++it)
			extensions.Add(*it);

		return extensions;
	}

	Array<const char*> ExtensionManager::GetDeviceExtensions() const
	{
		Array<const char*> extensions;
		for (auto it = mDeviceExtensions.begin(); it != mDeviceExtensions.end(); ++it)
			extensions.Add(*it);

		return extensions;
	}

	bool ExtensionManager::CheckExtensionsInstanceSupport(unsigned int extensionCount, const char* const* extensionNames) const
	{
		unsigned int propertyCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);

		Array<VkExtensionProperties> properties(propertyCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.GetData());

		return CheckExtensionSupport(properties, extensionCount, extensionNames);
	}

	bool ExtensionManager::CheckExtensionPhysicalDeviceSupport(VkPhysicalDevice physicalDevice, unsigned int extensionCount, const char* const* extensionNames) const
	{
		unsigned int propertyCount;
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, nullptr);

		Array<VkExtensionProperties> properties(propertyCount);
		vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &propertyCount, properties.GetData());

		return CheckExtensionSupport(properties, extensionCount, extensionNames);
	}

	bool ExtensionManager::CheckExtensionSupport(const Array<VkExtensionProperties>& properties, unsigned int extensionCount, const char* const* extensionNames) const
	{
		bool extensionsSupported = true;
		for (unsigned int i = 0; i < extensionCount && extensionsSupported; i++)
		{
			bool extensionSupported = false;
			const char* extensionName = extensionNames[i];
#ifndef NDEBUG
			if (IsExtensionDisabled(extensionName))
				return false;
#endif

			for (const VkExtensionProperties& property : properties)
			{
				if (std::strcmp(extensionName, property.extensionName) == 0)
					extensionSupported = true;
			}

			extensionsSupported &= extensionSupported;
		}

		return extensionsSupported;
	}

	bool ExtensionManager::IsExtensionDisabled(std::string_view extensionName) const
	{
		for (const std::string& disabledExtension : mDisabledExtensions)
		{
			if (disabledExtension == extensionName)
				return true;
		}
		return false;
	}
}
