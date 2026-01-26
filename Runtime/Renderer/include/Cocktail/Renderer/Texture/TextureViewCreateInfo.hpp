#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEWCREATEINFO_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEWCREATEINFO_HPP

#include <Cocktail/Renderer/PixelFormat.hpp>

#include <Cocktail/Renderer/Texture/TextureSubResource.hpp>
#include <Cocktail/Renderer/Texture/TextureSwizzle.hpp>

namespace Ck::Renderer
{
	class Texture;

	enum class TextureViewType
	{
		e1D,
		e1DArray,
		e2D,
		e2DArray,
		eCube,
		eCubeArray,
		e3D,
	};

	/**
	 * \brief TextureView descriptor structure
     * Describe the set of parameters to create a TextureView
     * \see RenderDevice::CreateTextureView
	 */
	struct TextureViewCreateInfo
	{
		/**
		 * \brief Specifies the Texture on which create the TextureView
		 */
		std::shared_ptr<Texture> Texture;

		/**
		 * \brief Specifies the type of the TextureView to create
		 */
		TextureViewType Type = TextureViewType::e1D;

		/**
		 * \brief Specifies the Format used by the TextureView to access to Texture's texels
		 * If \p Texture was not created using the TextureCreateFlagBits::MutableFormat flag, this value is ignored and the Format used is the one of the Texture
		 */
		PixelFormat Format;

		/**
		 * \brief Specifies the TextureSubResource to watch from \p Texture
		 * This parameter allow to access to only a subset of the levels and layers of \p Texture
		 * If this parameter nullptr, every level of every layer \p Texture will be accessible from the TextureView to create
		 */
		const TextureSubResource* SubResource = nullptr;
	
		/**
		 * \brief
		 */
		TextureSwizzle Swizzle;

        /**
         * \brief Specifies the name of the TextureView to create
         * If the extension Debug is not supported by the RenderDevice creating the TextureView, this parameter is ignored
         * \see RenderDeviceObject::GetName
         */
        const char* Name = nullptr;
	};
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTUREVIEWCREATEINFO_HPP
