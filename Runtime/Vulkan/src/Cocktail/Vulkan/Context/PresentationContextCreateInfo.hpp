#ifndef COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXTCREATEINFO_HPP
#define COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXTCREATEINFO_HPP

#include <volk.h>

#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

#include <Cocktail/Renderer/ColorSpace.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct PresentationContextCreateInfo
	{
		/**
		 * \brief
		 * TODO: Add an abstraction to handle VK_EXT_headless_surface ?
		 */
		VkSurfaceKHR Surface = VK_NULL_HANDLE;

		/**
		 * \brief 
		 */
		unsigned int BufferCount = 2;

		/**
		 * \brief 
		 */
		DisplayColorDepth ColorDepth = DisplayColorDepth::e32;

		/**
		 * \brief 
		 */
		DisplayAlphaDepth AlphaDepth = DisplayAlphaDepth::e8;

		/**
		 * \brief 
		 */
		Renderer::ColorSpace ColorSpace = Renderer::ColorSpace::Srgb;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXTCREATEINFO_HPP
