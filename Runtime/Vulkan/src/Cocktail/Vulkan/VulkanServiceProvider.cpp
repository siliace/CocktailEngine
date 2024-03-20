#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Renderer/RendererService.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/RenderDeviceCreateInfo.hpp>
#include <Cocktail/Vulkan/VolkService.hpp>
#include <Cocktail/Vulkan/VulkanServiceProvider.hpp>

CMRC_DECLARE(Vulkan);

namespace Ck::Vulkan
{
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
			renderDevice->InitializeMemoryAllocator();
			renderDevice->InitializePipelineCache();

			if (vulkanDeviceCreateInfo.EnableValidation && renderDevice->IsExtensionSupported(Renderer::RenderDeviceExtension::Debug))
				renderDevice->InitializeDebugMessenger();

			return renderDevice;
		};

		application->Resolve<VolkService>(); // Ensure Volk is initialized
		application->Resolve<Renderer::RendererService>()->SetFactory(Renderer::GraphicApi::Vulkan, factory);
	}
}
