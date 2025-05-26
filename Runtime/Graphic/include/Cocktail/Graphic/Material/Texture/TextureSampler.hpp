#ifndef COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP
#define COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class TextureSampler
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

		std::shared_ptr<MipMaps> mMipMaps;
		bool mGenerateMipMaps;
		std::shared_ptr<TextureResource> mTextureResource;
	};
}

#endif // COCKTAIL_MATERIAL_TEXTURE_TEXTURESAMPLER_HPP
