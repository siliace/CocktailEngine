#ifndef COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
#define COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP

#include <Cocktail/Vulkan/Context/PresentationContextCreateInfo.hpp>
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
		 * \param createInfo
		 * \param allocationCallbacks
		 */
		PresentationContext(const Ref<RenderDevice>& renderDevice, const PresentationContextCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		PixelFormat mSurfaceFormat;
		Renderer::ColorSpace mSurfaceColorSpace;
		unsigned int mBufferCount;
		std::vector<VkPresentModeKHR> mPresentModes;
		const VkAllocationCallbacks* mAllocationCallbacks;
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_PRESENTATIONCONTEXT_HPP
