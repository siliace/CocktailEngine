#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP

#include <CocktailEngine/Renderer/PixelFormat.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct RenderBufferCreateInfo
	{
		/**
		 * \brief 
		 */
		PixelFormat Format;

		/**
		 * \brief 
		 */
		Extent2D<unsigned int> Size;

		/**
		 * \brief 
		 */
		Renderer::RasterizationSamples Samples;
		
		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP
