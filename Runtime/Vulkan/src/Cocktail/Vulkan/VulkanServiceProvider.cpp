#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>
#include <Cocktail/Vulkan/VolkService.hpp>
#include <Cocktail/Vulkan/Vulkan.hpp>
#include <Cocktail/Vulkan/VulkanServiceProvider.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessenger.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

CMRC_DECLARE(Vulkan);

namespace Ck::Vulkan
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(VulkanServiceProvider);

	VulkanServiceProvider::VulkanServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

	void VulkanServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<VolkService>([]() {
			return std::make_unique<VolkService>();
		});
	}

	void VulkanServiceProvider::DoBoot(Application* application)
	{
		application->Invoke([](EmbeddedFileSystemDriver* embeddedDriver) {
			embeddedDriver->Register(cmrc::Vulkan::get_filesystem());
		});

		Renderer::RenderDeviceFactory factory = [&](bool enableDebug) {
			RenderDeviceCreateInfo vulkanDeviceCreateInfo;
			vulkanDeviceCreateInfo.EnableValidation = enableDebug;
			vulkanDeviceCreateInfo.ApiVersion = GetSupportedApiVersion();

			Ref<RenderDevice> renderDevice = RenderDevice::New(vulkanDeviceCreateInfo);

			if (enableDebug && renderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::Debug))
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

			renderDevice->Singleton<DeviceMemoryAllocator>([renderDevice = renderDevice]() {
				return std::make_unique<DeviceMemoryAllocator>(renderDevice);
			});

			renderDevice->Singleton<PipelineCache>([renderDevice = renderDevice]() {
				return std::make_unique<PipelineCache>(renderDevice, PipelineCacheCreateInfo{}, nullptr);
			});

			return renderDevice;
		};

		application->Resolve<VolkService>(); // Ensure Volk is initialized
		application->Resolve<Renderer::RendererService>()->SetFactory(Renderer::GraphicApi::Vulkan, factory);
	}
}
