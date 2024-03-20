#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>
#include <Cocktail/Vulkan/VolkService.hpp>
#include <Cocktail/Vulkan/VulkanServiceProvider.hpp>
#include <Cocktail/Vulkan/Debug/DebugMessenger.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>
#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp>

CMRC_DECLARE(Vulkan);

namespace Ck::Vulkan
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(VulkanServiceProvider);

	VulkanServiceProvider::VulkanServiceProvider(const Ref<Application>& application) :
		Extends<VulkanServiceProvider, ServiceProvider>(application)
	{
		/// Nothing
	}

	void VulkanServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<VolkService>([]() {
			return VolkService::New();
		});
	}

	void VulkanServiceProvider::DoBoot(const Ref<Application>& application)
	{
		application->Invoke([](Ref<EmbeddedFileSystemDriver> embeddedDriver) {
			embeddedDriver->Register(cmrc::Vulkan::get_filesystem());
		});

		Renderer::RenderDeviceFactory factory = [&](bool enableDebug) {
			RenderDeviceCreateInfo vulkanDeviceCreateInfo;
			vulkanDeviceCreateInfo.EnableValidation = enableDebug;

			Ref<RenderDevice> renderDevice = RenderDevice::New(vulkanDeviceCreateInfo);

			if (enableDebug && renderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::Debug))
				renderDevice->Instance(DebugMessenger::New(renderDevice, DebugMessengerCreateInfo{}, nullptr));

			renderDevice->Singleton<DepthResolver>([renderDevice = renderDevice]() {
				return DepthResolver::New(renderDevice);
			});

			renderDevice->Singleton<DeviceMemoryAllocator>([renderDevice = renderDevice]() {
				return DeviceMemoryAllocator::New(renderDevice);
			});

			renderDevice->Singleton<PipelineCache>([renderDevice = renderDevice]() {
				return PipelineCache::New(renderDevice, PipelineCacheCreateInfo{}, nullptr);
			});

			return renderDevice;
		};

		application->Resolve<VolkService>(); // Ensure Volk is initialized
		application->Resolve<Renderer::RendererService>()->SetFactory(Renderer::GraphicApi::Vulkan, factory);
	}
}
