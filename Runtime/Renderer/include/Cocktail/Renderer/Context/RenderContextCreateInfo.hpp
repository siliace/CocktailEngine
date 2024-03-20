#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP

#include <Cocktail/Renderer/Context/RenderSurface.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	struct RenderContextCreateInfo
	{
		/**
		 * \brief 
		 */
		Ref<RenderSurface> RenderSurface = nullptr;

		/**
		 * \brief Specifies the name of the RenderContext to create
		 * If the extension Debug is not supported by the RenderDevice creating the RenderContext, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP
