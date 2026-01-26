#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP

#include <Cocktail/Renderer/PixelFormat.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>

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

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFERCREATEINFO_HPP
