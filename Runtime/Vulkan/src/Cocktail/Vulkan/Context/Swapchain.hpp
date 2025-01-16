#ifndef COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP
#define COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Vulkan/Context/SwapchainCreateInfo.hpp>
#include <Cocktail/Vulkan/Texture/SwapchainTexture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class Swapchain : public Inherit<Swapchain, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr unsigned int MaxSwapchainTexture = 8;

		/**
		 * \brief Constructor
		 * Create a new swapchain
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Swapchain(Ref<RenderDevice> renderDevice, const SwapchainCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Swapchain() override;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetTextureCount() const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		Ref<SwapchainTexture> GetTexture(unsigned int index) const;

		/**
		 * \brief 
		 * \return 
		 */
		VkSwapchainKHR GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSwapchainKHR mHandle;
		unsigned int mTextureCount;
		Ref<SwapchainTexture> mTextures[MaxSwapchainTexture];
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP
