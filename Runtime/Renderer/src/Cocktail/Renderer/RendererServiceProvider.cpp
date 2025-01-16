#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/RendererService.hpp>
#include <Cocktail/Renderer/RendererServiceProvider.hpp>

namespace Ck::Renderer
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(RendererServiceProvider);

	RendererServiceProvider::RendererServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		/// Nothing
	}

	void RendererServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<RendererService>();
	}
}
