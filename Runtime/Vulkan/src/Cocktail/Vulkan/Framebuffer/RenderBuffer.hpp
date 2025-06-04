#ifndef COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP
#define COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP

#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

#include <Cocktail/Vulkan/Framebuffer/RenderBufferCreateInfo.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	class DeviceMemoryBlock;

	/**
	 * \brief 
	 */
	class RenderBuffer final : public AbstractTexture
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo
		 * \param allocationCallbacks 
		 */
		RenderBuffer(std::shared_ptr<RenderDevice> renderDevice, const RenderBufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~RenderBuffer() override;

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
		Renderer::RasterizationSamples GetSamples() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkImage GetHandle() const override;

	private:
		
		PixelFormat mFormat;
		Extent3D<unsigned int> mSize;
		Renderer::RasterizationSamples mSamples;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkImage mHandle;
		DeviceMemoryBlock* mDeviceMemoryBlock;
	};
}

#endif // COCKTAIL_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP
