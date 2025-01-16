#ifndef COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP
#define COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class TextureSampler : public Extends<TextureSampler, Object>
	{
	public:

		void UpdateLevels(unsigned int baseLevel, unsigned int levelCount)
		{
			for (unsigned int layer = 0; layer < mMipMaps->GetArrayLayerCount(); layer++)
			{
				for (unsigned int level = baseLevel; level < levelCount; level++)
				{
					mTextureResource->SetLevelPixels(layer, level, mMipMaps->GetLevel(levelCount, level).GetPixels());
				}
			}

			if (mGenerateMipMaps && baseLevel + levelCount < mMipMaps->GetLevelCount())
				mTextureResource->GenerateMipMaps(0, baseLevel + levelCount);
		}

	private:

		Ref<MipMaps> mMipMaps;
		bool mGenerateMipMaps;
		Ref<TextureResource> mTextureResource;
	};
}

#endif // COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP
