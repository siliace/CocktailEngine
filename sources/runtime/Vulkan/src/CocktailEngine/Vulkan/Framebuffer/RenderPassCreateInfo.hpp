#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP

#include <CocktailEngine/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <CocktailEngine/Renderer/Framebuffer/FramebufferLayout.hpp>

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
		Renderer::ResolveMode DepthResolveMode;

		/**
		 * \brief 
		 */
		Renderer::ResolveMode StencilResolveMode;

		/**
		 * \brief 
		 */
		bool Presentable = false;
	};
}

#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERPASSCREATEINFO_HPP
