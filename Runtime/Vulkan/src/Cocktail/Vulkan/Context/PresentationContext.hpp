#ifndef COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>
#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

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
		PresentationContext(std::shared_ptr<RenderDevice> renderDevice, VkSurfaceKHR surface, unsigned int bufferCount, DisplayColorDepth colorDepth, DisplayAlphaDepth alphaDepth, Renderer::ColorSpace colorSpace);

		/**
		 * \brief
		 * \param size
		 * \param presentMode
		 * \param old
		 * \return
		 */
		std::shared_ptr<Swapchain> CreateSwapchain(const Extent2D<unsigned int>& size, VkPresentModeKHR presentMode, Swapchain* old) const;

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

		std::shared_ptr<RenderDevice> mRenderDevice;
		VkSurfaceKHR mSurface;
		unsigned int mBufferCount;
		PixelFormat mSurfaceFormat;
		Renderer::ColorSpace mSurfaceColorSpace;
		std::vector<VkPresentModeKHR> mPresentModes;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
