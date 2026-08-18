#ifndef COCKTAILENGINE_VULKAN_TEXTURE_TEXTUREVIEW_HPP
#define COCKTAILENGINE_VULKAN_TEXTURE_TEXTUREVIEW_HPP

#include <CocktailEngine/Renderer/Texture/TextureView.hpp>
#include <CocktailEngine/Renderer/Texture/TextureViewCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Texture/AbstractTexture.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class TextureView : public Renderer::TextureView
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		TextureView(RenderDevice* renderDevice, const Renderer::TextureViewCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

		/**
		 * \brief 
		 * \return 
		 */
		SharedPtr<Renderer::Texture> GetTexture() const override;

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

		RenderDevice* mRenderDevice;
		SharedPtr<AbstractTexture> mTexture;
		PixelFormat mFormat;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkImageView mHandle;
	};
}

#endif // COCKTAILENGINE_VULKAN_TEXTURE_TEXTUREVIEW_HPP
