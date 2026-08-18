#ifndef COCKTAILENGINE_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP
#define COCKTAILENGINE_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP

#include <CocktailEngine/Core/Memory/SharedFromThis.hpp>

#include <CocktailEngine/Renderer/RasterizationSamples.hpp>
#include <CocktailEngine/Renderer/Texture/Texture.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class AbstractTexture : public Renderer::Texture, public SharedFromThis<AbstractTexture>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit AbstractTexture(RenderDevice* renderDevice);

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
		Renderer::ResourceType GetResourceType() const override;

		/**
		 * \brief
		 * \return
		 */
		bool HasMutableFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Renderer::RasterizationSamples GetSamples() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual VkImage GetHandle() const = 0;

	protected:

		/**
		 * \brief 
		 * \param viewFormatCount 
		 * \param viewFormats 
		 */
		void SetViewFormats(unsigned int viewFormatCount, const PixelFormat* viewFormats);

		RenderDevice* mRenderDevice;

	private:

		bool mHasMutableFormat;
		unsigned int mViewFormatCount;
		UniquePtr<PixelFormat[]> mViewFormats;
	};
}

#endif // COCKTAILENGINE_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP
