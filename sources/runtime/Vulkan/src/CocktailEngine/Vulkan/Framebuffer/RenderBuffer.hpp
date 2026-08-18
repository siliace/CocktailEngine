#ifndef COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP
#define COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP

#include <CocktailEngine/Renderer/Texture/TextureCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Framebuffer/RenderBufferCreateInfo.hpp>
#include <CocktailEngine/Vulkan/Texture/AbstractTexture.hpp>

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
		RenderBuffer(RenderDevice* renderDevice, const RenderBufferCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
	    Renderer::MemoryPriority GetPriority() const override;

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

#endif // COCKTAILENGINE_VULKAN_FRAMEBUFFER_RENDERBUFFER_HPP
