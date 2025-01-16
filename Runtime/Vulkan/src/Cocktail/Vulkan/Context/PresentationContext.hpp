#ifndef COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP

#include <Cocktail/Vulkan/Context/Swapchain.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief
	 */
	class PresentationContext : public Extends<PresentationContext, Object>
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
		PresentationContext(Ref<RenderDevice> renderDevice, VkSurfaceKHR surface, unsigned int bufferCount, DisplayColorDepth colorDepth, DisplayAlphaDepth alphaDepth, Renderer::ColorSpace colorSpace);

		/**
		 * \brief
		 * \param size
		 * \param presentMode
		 * \param old
		 * \return
		 */
		Ref<Swapchain> CreateSwapchain(const Extent2D<unsigned int>& size, VkPresentModeKHR presentMode, Swapchain* old) const;

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

		Ref<RenderDevice> mRenderDevice;
		VkSurfaceKHR mSurface;
		unsigned int mBufferCount;
		PixelFormat mSurfaceFormat;
		Renderer::ColorSpace mSurfaceColorSpace;
		std::vector<VkPresentModeKHR> mPresentModes;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
