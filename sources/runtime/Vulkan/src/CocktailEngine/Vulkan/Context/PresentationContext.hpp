#ifndef COCKTAILENGINE_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
#define COCKTAILENGINE_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP

#include <CocktailEngine/Renderer/PixelFormat.hpp>
#include <CocktailEngine/Core/System/Monitor/VideoMode.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;
	class Swapchain;

	/**
	 * \brief
	 */
	class PresentationContext
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param surface 
		 * \param bufferCount
		 * \param colorDepth 
		 * \param alphaDepth 
		 * \param colorSpace 
		 */
		PresentationContext(RenderDevice* renderDevice, RenderSurface* surface, unsigned int bufferCount, DisplayColorDepth colorDepth, DisplayAlphaDepth alphaDepth, Renderer::ColorSpace colorSpace);

		/**
		 * \brief
		 * \param size
		 * \param presentMode
		 * \param old
		 * \return
		 */
		SharedPtr<Swapchain> CreateSwapchain(const Extent2D<unsigned int>& size, VkPresentModeKHR presentMode, Swapchain* old) const;

		/**
		 * \brief
		 * \param presentMode
		 * \return
		 */
		bool IsPresentationModeSupported(VkPresentModeKHR presentMode) const;

		/**
		 * \brief
		 * \return
		 */
		PixelFormat GetSurfaceFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ColorSpace GetSurfaceColorSpace() const;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetBufferCount() const;

	private:

		RenderDevice* mRenderDevice;
		RenderSurface* mSurface;
		unsigned int mBufferCount;
		PixelFormat mSurfaceFormat;
		Renderer::ColorSpace mSurfaceColorSpace;
		Array<VkPresentModeKHR> mPresentModes;
	};
}

#endif // COCKTAILENGINE_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
