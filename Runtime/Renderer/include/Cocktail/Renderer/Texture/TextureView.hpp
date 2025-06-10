#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP

#include <Cocktail/Core/Image/PixelFormat.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	class Texture;

	/**
     * \brief Interface representing an opaque handle to a GPU texture view object
     * A TextureView is a contiguous ranges of the image subresources and containing additional metadata are used for that purpose.
     * They can be used for various purposes (e.g. attachments, textures), by binding them to a CommandList of a Framebuffer.
	 */
	class TextureView : public RenderDeviceObject
	{
	public:

		/**
	     * \brief Get the underling Texture
	     * \return The underlying Texture
		 */
		virtual std::shared_ptr<Texture> GetTexture() const = 0;
		
		/**
		 * \brief Get the format used to interpret Texture data
		 * \return The interpretation format
		 */
		virtual PixelFormat GetFormat() const = 0;
	};
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEW_HPP
