#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/RendererService.hpp>
#include <Cocktail/Renderer/RendererServiceProvider.hpp>

namespace Ck::Renderer
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(RendererServiceProvider);

	RendererServiceProvider::RendererServiceProvider(const Ref<Application>& application) :
		Super(application)
	{
		/// Nothing
	}

	void RendererServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<RendererService>();
	}
}
