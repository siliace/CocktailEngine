#ifndef COCKTAIL_VULKAN_TEXTURE_TEXTURE_HPP
#define COCKTAIL_VULKAN_TEXTURE_TEXTURE_HPP

#include <Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp>
#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class Texture final : public AbstractTexture
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Texture(std::shared_ptr<RenderDevice> renderDevice, const Renderer::TextureCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Texture() override;

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
		
		Renderer::TextureType mType;
		PixelFormat mFormat;
		Extent3D<unsigned int> mSize;
		bool mExclusive;
		unsigned int mMipMapCount;
		unsigned int mArrayLayerCount;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkImage mHandle;
		DeviceMemoryBlock* mDeviceMemoryBlock;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_TEXTURE_HPP
