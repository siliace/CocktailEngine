#ifndef COCKTAIL_VULKAN_TEXTURE_TEXTUREVIEW_HPP
#define COCKTAIL_VULKAN_TEXTURE_TEXTUREVIEW_HPP

#include <Cocktail/Renderer/Texture/TextureView.hpp>
#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

#include <Cocktail/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class TextureView : public Inherit<TextureView, Object, Renderer::TextureView>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		TextureView(const Ref<RenderDevice>& renderDevice, const Renderer::TextureViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~TextureView() override;

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
		Ref<Renderer::Texture> GetTexture() const override;

		/**
		 * \brief 
		 * \return 
		 */
		PixelFormat GetFormat() const override;

		/**
		 * \brief 
		 * \return 
		 */
		VkImageView GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		Ref<AbstractTexture> mTexture;
		PixelFormat mFormat;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkImageView mHandle;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_TEXTUREVIEW_HPP
