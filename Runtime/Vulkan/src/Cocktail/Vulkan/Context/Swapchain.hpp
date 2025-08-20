#ifndef COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP
#define COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP

#include <Cocktail/Vulkan/Context/SwapchainCreateInfo.hpp>
#include <Cocktail/Vulkan/Texture/SwapchainTexture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class Swapchain : public Renderer::RenderDeviceObject
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
		Swapchain(RenderDevice* renderDevice, const SwapchainCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

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
		std::shared_ptr<SwapchainTexture> GetTexture(unsigned int index) const;

		/**
		 * \brief 
		 * \return 
		 */
		VkSwapchainKHR GetHandle() const;

	private:

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkSwapchainKHR mHandle;
		unsigned int mTextureCount;
		std::shared_ptr<SwapchainTexture> mTextures[MaxSwapchainTexture];
	};
}

#endif // COCKTAIL_VULKAN_CONTEXT_SWAPCHAIN_HPP
