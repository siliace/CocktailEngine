#ifndef COCKTAIL_RENDERER_TEXTURE_TEXTURESUBRESOURCE_HPP
#define COCKTAIL_RENDERER_TEXTURE_TEXTURESUBRESOURCE_HPP

#include <tuple>

#include <Cocktail/Renderer/Texture/Texture.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Describe a subset of layers and levels of a Texture
	 * \see TextureViewCreateInfo::SubResource
	 */
	struct TextureSubResource
	{
		/**
		 * \brief 
		 * \return 
		 */
		static TextureSubResource None()
		{
			return {0, 0, 0, 0};
		}

		/**
		 * \brief
		 * \param arrayLayer
		 * \param mipMapLevel
		 * \return
		 */
		static TextureSubResource OneLayerOneLevel(unsigned int arrayLayer, unsigned int mipMapLevel)
		{
			return {arrayLayer, mipMapLevel, 1, 1};
		}

		/**
		 * \brief
		 * \return
		 */
		static TextureSubResource All(const Texture& texture)
		{
			return AllLayersAllLevels(texture, 0, 0);
		}

		/**
		 * \brief
		 * \param texture
		 * \param mipMapLevel
		 * \return
		 */
		static TextureSubResource AllLayersOneLevel(const Texture& texture, unsigned int mipMapLevel)
		{
			return AllLayersSomeLevels(texture, mipMapLevel, 1);
		}

		/**
		 * \brief 
		 * \param texture 
		 * \param baseMipMapLevel 
		 * \param levelCount 
		 * \return 
		 */
		static TextureSubResource AllLayersSomeLevels(const Texture& texture, unsigned int baseMipMapLevel, unsigned int levelCount)
		{
			return {0, baseMipMapLevel, texture.GetArrayLayerCount(), levelCount};
		}

		/**
		 * \brief
		 * \return
		 */
		static TextureSubResource AllLayersAllLevels(const Texture& texture, unsigned int baseArrayLayer, unsigned int baseMipMapLevel)
		{
			return {baseArrayLayer, baseMipMapLevel, texture.GetArrayLayerCount() - baseArrayLayer, texture.GetMipMapCount() - baseMipMapLevel};
		}

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

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		bool operator==(const TextureSubResource& rhs)
		{
			return std::tie(BaseArrayLayer, BaseMipMapLevel, ArrayLayerCount, MipMapLevelCount) ==
				std::tie(rhs.BaseArrayLayer, rhs.BaseMipMapLevel, rhs.ArrayLayerCount, rhs.MipMapLevelCount);
		}

		/**
		 * \brief
		 * \param rhs
		 * \return
		 */
		bool operator!=(const TextureSubResource& rhs)
		{
			return !(*this == rhs);
		}
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
