#include <Cocktail/Renderer/RendererService.hpp>

namespace Ck::Renderer
{
	Ref<RenderDevice> RendererService::CreateRenderDevice(GraphicApi api, bool enableDebug) const
	{
		return std::invoke(mFactories[api], enableDebug);
	}

	void RendererService::SetFactory(GraphicApi api, const RenderDeviceFactory& factory)
	{
		mFactories[api] = factory;
	}

	bool RendererService::IsApiSupported(GraphicApi api) const
	{
		return mFactories[api] != nullptr;
	}
}
