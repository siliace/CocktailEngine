#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/MipMapsTextureResource.hpp>

namespace Ck
{
	MipMapsTextureResource::MipMapsTextureResource(SharedPtr<GraphicEngine> graphicEngine, SharedPtr<MipMaps> mipMaps, const AnsiChar* name) :
		TextureResource(std::move(graphicEngine), mipMaps->GetBaseSize(), mipMaps->GetPixelFormat(), mipMaps->GetArrayLayerCount(), mipMaps->GetMipMapCount(), mipMaps->IsCube(), name),
		mMipMaps(std::move(mipMaps))
	{
		/// Nothing
	}

	void MipMapsTextureResource::LoadLevels(unsigned int baseLayer, unsigned int baseLevel, unsigned int levelCount)
	{
		for (unsigned int level = baseLevel; level < levelCount; level++)
		{
			const MipMapLevel& mipMap = mMipMaps->GetLevel(baseLayer, level);
			GetGraphicEngine()->UploadTexture(AsShared(), baseLayer, level, mipMap.GetPixels().GetData());
		}
	}

	SharedPtr<MipMaps> MipMapsTextureResource::GetMipMaps() const
	{
		return mMipMaps;
	}
}
