#ifndef COCKTAIL_VULKAN_TEXTURE_SWAPCHAINTEXTURE_HPP
#define COCKTAIL_VULKAN_TEXTURE_SWAPCHAINTEXTURE_HPP

#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	class SwapchainTexture final : public AbstractTexture
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param format  
		 * \param size 
		 * \param handle 
		 */
		SwapchainTexture(std::shared_ptr<RenderDevice> renderDevice, PixelFormat format, const Extent2D<unsigned int>& size, VkImage handle);

		/**
		 * \brief
		 * \param format
		 * \param size
		 * \param handle
		 * \return
		 */
		void Recycle(PixelFormat format, const Extent2D<unsigned int>& size, VkImage handle);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsExclusive() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::MemoryType GetMemoryType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::TextureType GetType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		PixelFormat GetFormat() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Extent3D<unsigned int> GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetMipMapCount() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetArrayLayerCount() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkImage GetHandle() const override;

	private:
		
		PixelFormat mFormat;
		Extent2D<unsigned int> mSize;
		VkImage mHandle;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_SWAPCHAINTEXTURE_HPP
