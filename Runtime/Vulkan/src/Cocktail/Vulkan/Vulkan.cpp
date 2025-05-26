#include <Cocktail/Vulkan/ExtensionManager.hpp>
#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/VulkanUtils.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessenger.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

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

	std::shared_ptr<Renderer::RenderDevice> CreateRenderDevice(const RenderDeviceCreateInfo& createInfo)
	{
		std::shared_ptr<RenderDevice> renderDevice = std::make_shared<RenderDevice>(createInfo);

		if (createInfo.EnableValidation && renderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::Debug))
		{
			renderDevice->Singleton<DebugMessenger>([renderDevice = renderDevice]() {
				return std::make_unique<DebugMessenger>(renderDevice, DebugMessengerCreateInfo{}, nullptr);
			});
			renderDevice->Resolve<DebugMessenger>();
		}

		if (renderDevice->IsFeatureSupported(RenderDeviceFeature::ValidationCache))
		{
			renderDevice->Singleton<ValidationCache>([renderDevice = renderDevice]() {
				return std::make_unique<ValidationCache>(renderDevice, ValidationCacheCreateInfo{}, nullptr);
			});
		}

		renderDevice->Singleton<DepthResolver>([renderDevice = renderDevice]() {
			return std::make_unique<DepthResolver>(renderDevice);
		});

		renderDevice->Singleton<DeviceMemoryAllocator>([renderDevice = renderDevice, createInfo = createInfo]() {
			return std::make_unique<DeviceMemoryAllocator>(renderDevice, createInfo.DeviceMemoryBlockSize);
		});

		renderDevice->Singleton<PipelineCache>([renderDevice = renderDevice]() {
			return std::make_unique<PipelineCache>(renderDevice, PipelineCacheCreateInfo{}, nullptr);
		});

		return renderDevice;
	}
}
