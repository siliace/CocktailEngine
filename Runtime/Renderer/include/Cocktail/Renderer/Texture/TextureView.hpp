#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	class Texture;

	/**
	 * \brief 
	 */
	class TextureView : public RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::shared_ptr<Texture> GetTexture() const = 0;
		
		/**
		 * \brief 
		 * \return 
		 */
		virtual PixelFormat GetFormat() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP
