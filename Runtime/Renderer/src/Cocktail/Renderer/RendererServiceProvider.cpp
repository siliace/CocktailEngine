#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/RendererService.hpp>
#include <Cocktail/Renderer/RendererServiceProvider.hpp>

namespace Ck::Renderer
{
	RendererServiceProvider::RendererServiceProvider(const Ref<Application>& application):
		Extends<RendererServiceProvider, ServiceProvider>(application)
	{
		/// Nothing
	}

	void RendererServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<RendererService>();
	}
}
