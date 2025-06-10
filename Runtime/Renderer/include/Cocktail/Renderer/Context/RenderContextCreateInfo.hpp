#ifndef COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP
#define COCKTAIL_RENDERER_CONTEXT_RENDERCONTEXTCREATEINFO_HPP

namespace Ck::Renderer
{
	/**
	 * \brief RenderContext descriptor structure
	 * Describe the set of parameters to create a RenderContext
	 * \see RenderDevice::CreateRenderContext
	 */
	struct RenderContextCreateInfo
	{
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
