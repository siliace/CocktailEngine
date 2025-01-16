#ifndef COCKTAIL_RENDERER_GRAPHICAPI_HPP
#define COCKTAIL_RENDERER_GRAPHICAPI_HPP

namespace Ck::Renderer
{
	/**
	 * \brief
	 */
	enum class GraphicApi
	{
		/**
		 * \brief 
		 */
		Direct3D11,

		/**
		 * \brief 
		 */
		Direct3D12,

		/**
		 * \brief 
		 */
		Metal,

		/**
		 * \brief 
		 */
		OpenGL,

		/**
		 * \brief 
		 */
		OpenGLES,

		/**
		 * \brief
		 */
		Vulkan,
	};
}

#endif // COCKTAIL_RENDERER_GRAPHICAPI_HPP
