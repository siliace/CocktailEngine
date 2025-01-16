#ifndef COCKTAIL_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP
#define COCKTAIL_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP

#include <memory>

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class AbstractTexture : public Inherit<AbstractTexture, Object, Renderer::Texture>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 */
		explicit AbstractTexture(const Ref<RenderDevice>& renderDevice);

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

		Ref<RenderDevice> mRenderDevice;

	private:

		bool mHasMutableFormat;
		unsigned int mViewFormatCount;
		std::unique_ptr<PixelFormat[]> mViewFormats;
	};
}

#endif // COCKTAIL_VULKAN_TEXTURE_ABSTRACTTEXTURE_HPP
