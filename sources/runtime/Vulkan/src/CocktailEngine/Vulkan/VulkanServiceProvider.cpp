#include <CocktailEngine/Core/Application/Detail/ServiceFacadeBase.hpp>

#include <CocktailEngine/Vulkan/RenderDevice.hpp>
#include <CocktailEngine/Vulkan/VolkService.hpp>
#include <CocktailEngine/Vulkan/VulkanServiceProvider.hpp>
#include <CocktailEngine/Vulkan/Framebuffer/DepthResolver.hpp>

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
		application->Singleton<VolkService>(false);
	}
}
