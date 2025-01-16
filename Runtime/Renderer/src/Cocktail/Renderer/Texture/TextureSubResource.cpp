#include <tuple>

#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureSubResource.hpp>

namespace Ck
{
	Renderer::TextureSubResource Renderer::TextureSubResource::None()
	{
		return {0, 0, 0, 0};
	}

	Renderer::TextureSubResource Renderer::TextureSubResource::OneLayerOneLevel(unsigned int arrayLayer, unsigned int mipMapLevel)
	{
		return {arrayLayer, mipMapLevel, 1, 1};
	}

	Renderer::TextureSubResource Renderer::TextureSubResource::All(const Texture& texture)
	{
		return AllLayersAllLevels(texture, 0, 0);
	}

	Renderer::TextureSubResource Renderer::TextureSubResource::AllLayersOneLevel(const Texture& texture, unsigned int mipMapLevel)
	{
		return AllLayersSomeLevels(texture, mipMapLevel, 1);
	}

	Renderer::TextureSubResource Renderer::TextureSubResource::AllLayersSomeLevels(const Texture& texture, unsigned int baseMipMapLevel, unsigned int levelCount)
	{
		return {0, baseMipMapLevel, texture.GetArrayLayerCount(), levelCount};
	}

	Renderer::TextureSubResource Renderer::TextureSubResource::AllLayersAllLevels(const Texture& texture, unsigned int baseArrayLayer, unsigned int baseMipMapLevel)
	{
		return {baseArrayLayer, baseMipMapLevel, texture.GetArrayLayerCount() - baseArrayLayer, texture.GetMipMapCount() - baseMipMapLevel};
	}

	bool Renderer::TextureSubResource::operator==(const TextureSubResource& rhs)
	{
		return std::tie(BaseArrayLayer, BaseMipMapLevel, ArrayLayerCount, MipMapLevelCount) ==
			std::tie(rhs.BaseArrayLayer, rhs.BaseMipMapLevel, rhs.ArrayLayerCount, rhs.MipMapLevelCount);
	}

	bool Renderer::TextureSubResource::operator!=(const TextureSubResource& rhs)
	{
		return !(*this == rhs);
	}
}
