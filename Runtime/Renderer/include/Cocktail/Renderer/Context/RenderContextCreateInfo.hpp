#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	struct RenderContextCreateInfo
	{
		/**
		 * \brief Specifies the number of RenderSurface to be acquired per frame
		 */
		unsigned int RenderSurfaceCount = 1;

		/**
		 * \brief Specifies the number of concurrent frame
		 */
		unsigned int ConcurrentFrameCount = 3;

		/**
		 * \brief Specifies the name of the RenderContext to create
		 * If the extension Debug is not supported by the RenderDevice creating the RenderContext, this parameter is ignored
		 * \see RenderDeviceObject::GetName
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP
