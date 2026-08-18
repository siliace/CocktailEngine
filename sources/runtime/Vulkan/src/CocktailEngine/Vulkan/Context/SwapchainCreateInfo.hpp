#ifndef COCKTAILENGINE_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP

#include <CocktailEngine/Renderer/PixelFormat.hpp>

#include <CocktailEngine/Renderer/ColorSpace.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

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

#endif // COCKTAILENGINE_VULKAN_CONTEXT_SWAPCHAINCREATEINFO_HPP
