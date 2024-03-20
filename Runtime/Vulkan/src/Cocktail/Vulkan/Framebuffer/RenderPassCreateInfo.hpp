#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP

#include <Cocktail/Renderer/Framebuffer/FramebufferLayout.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct RenderPassCreateInfo
	{
		/**
		 * \brief 
		 */
		Renderer::FramebufferLayout FramebufferLayout;

		/**
		 * \brief 
		 */
		bool Presentable = false;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP
