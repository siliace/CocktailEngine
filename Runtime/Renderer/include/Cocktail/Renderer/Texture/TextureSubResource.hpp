#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTURESUBRESOURCE_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTURESUBRESOURCE_HPP

#include <Cocktail/Renderer/Export.hpp>

namespace Ck::Renderer
{
	class Texture;

	/**
	 * \brief Describe a subset of layers and levels of a Texture
	 * \see TextureViewCreateInfo::SubResource
	 */
	class COCKTAIL_RENDERER_API TextureSubResource
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static TextureSubResource None();

		/**
		 * \brief
		 * \param arrayLayer
		 * \param mipMapLevel
		 * \return
		 */
		static TextureSubResource OneLayerOneLevel(unsigned int arrayLayer, unsigned int mipMapLevel);

		/**
		 * \brief
		 * \return
		 */
		static TextureSubResource All(const Texture& texture);

		/**
		 * \brief
		 * \param texture
		 * \param mipMapLevel
		 * \return
		 */
		static TextureSubResource AllLayersOneLevel(const Texture& texture, unsigned int mipMapLevel);

		/**
		 * \brief 
		 * \param texture 
		 * \param baseMipMapLevel 
		 * \param levelCount 
		 * \return 
		 */
		static TextureSubResource AllLayersSomeLevels(const Texture& texture, unsigned int baseMipMapLevel, unsigned int levelCount);

		/**
		 * \brief
		 * \return
		 */
		static TextureSubResource AllLayersAllLevels(const Texture& texture, unsigned int baseArrayLayer, unsigned int baseMipMapLevel);

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		bool operator==(const TextureSubResource& rhs);

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		bool operator!=(const TextureSubResource& rhs);

		/**
		 * \brief The first layer to include into the subresource
		 */
		unsigned int BaseArrayLayer = 0;

		/**
		 * \brief The first level to include into the subresource
		 */
		unsigned int BaseMipMapLevel = 0;

		/**
		 * \brief The number of layers to include into the subresource
		 */
		unsigned int ArrayLayerCount = 1;

		/**
		 * \brief The number of levels to include into the subresource
		 */
		unsigned int MipMapLevelCount = 1;
	};
}

namespace std
{
	template <>
	struct hash<Ck::Renderer::TextureSubResource>
	{
		size_t operator()(const Ck::Renderer::TextureSubResource& textureSubResource) const noexcept
		{
			size_t hash = 0;
			Ck::HashCombine(hash, textureSubResource.BaseArrayLayer);
			Ck::HashCombine(hash, textureSubResource.BaseMipMapLevel);
			Ck::HashCombine(hash, textureSubResource.ArrayLayerCount);
			Ck::HashCombine(hash, textureSubResource.MipMapLevelCount);

			return hash;
		}
	};
}

#endif // COCKTAIL_RENDERER_TEXTURE_TEXTURESUBRESOURCE_HPP
