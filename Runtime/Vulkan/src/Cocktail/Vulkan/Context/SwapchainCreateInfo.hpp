#ifndef COCKTAIL_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP
#define COCKTAIL_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/ColorSpace.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderSurface;
	class Swapchain;

	/**
	 * \brief 
	 */
	struct SwapchainCreateInfo
	{
		/**
		 * \brief 
		 */
		RenderSurface* Surface = nullptr;

		/**
		 * \brief 
		 */
		unsigned int MinImageCount = 2;

		/**
		 * \brief 
		 */
		PixelFormat Format;

		/**
		 * \brief 
		 */
		Renderer::ColorSpace ColorSpace;

		/**
		 * \brief 
		 */
		Extent2D<unsigned int> Size;

		/**
		 * \brief 
		 */
		VkPresentModeKHR PresentMode = VK_PRESENT_MODE_FIFO_KHR;

		/**
		 * \brief 
		 */
		Swapchain* Old = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP
