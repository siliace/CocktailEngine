#include <Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp>
#include <Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp>

#include <Cocktail/Vulkan/RenderDevice.hpp>
#include <Cocktail/Vulkan/VolkService.hpp>
#include <Cocktail/Vulkan/VulkanServiceProvider.hpp>
#include <Cocktail/Vulkan/Framebuffer/DepthResolver.hpp>

CMRC_DECLARE(Vulkan);

namespace Ck::Vulkan
{
	CK_REGISTER_SERVICE_PROVIDER(VulkanServiceProvider);

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

		application->Resolve<VolkService>(); // Ensure Volk is initialized
	}
}
