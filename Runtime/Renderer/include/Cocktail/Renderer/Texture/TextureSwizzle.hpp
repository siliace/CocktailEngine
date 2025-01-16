#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTURESWIZZLE_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTURESWIZZLE_HPP

namespace Ck::Renderer
{
	enum class TextureSwizzleValue
	{
		/**
		 * \brief The component is kept unchanged
		 */
		Identity,

		/**
		 * \brief The component is replaced by the constant zero
		 */
		Zero,

		/**
		 * \brief The component is replaced by the constant one
		 */
		One,

		/**
		 * \brief The component is replaced by the red component
		 */
		Red,

		/**
		 * \brief The component is replaced by the green component
		 */
		Green,

		/**
		 * \brief The component is replaced by the blue component
		 */
		Blue,

		/**
		 * \brief The component is replaced by the alpha component
		 */
		Alpha
	};

	/**
	 * \brief Texture component swizzle structure for red, green, blue, and alpha components
	 * \remarks Can be used to change the order of texel components independently of a shader
	 * \see TextureViewCreateInfo::Swizzle
	 */
	struct TextureSwizzle
	{
		TextureSwizzleValue Red = TextureSwizzleValue::Identity;
		TextureSwizzleValue Green = TextureSwizzleValue::Identity;
		TextureSwizzleValue Blue = TextureSwizzleValue::Identity;
		TextureSwizzleValue Alpha = TextureSwizzleValue::Identity;
	};
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTURESWIZZLE_HPP