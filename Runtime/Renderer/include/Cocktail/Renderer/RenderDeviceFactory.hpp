#ifndef COCKTAIL_RENDERER_RENDERDEVICEFACTORY_HPP
#define COCKTAIL_RENDERER_RENDERDEVICEFACTORY_HPP

#include <functional>
#include <memory>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck::Renderer
{
	using RenderDeviceFactory = std::function<Ref<RenderDevice>(bool)>;
}

#endif // COCKTAIL_RENDERER_RENDERDEVICEFACTORY_HPP
