#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

#include <Cocktail/Renderer/Texture/Texture.hpp>

namespace Ck
{
	std::shared_ptr<MipMaps> MipMaps::FromImage(const Image& image)
	{
		Extent3D<unsigned int> baseLevelSize = MakeExtent(image.GetSize(), 1u);

		unsigned int levelCount = Renderer::ComputeTextureLevelCount(baseLevelSize);

	    PixelFormat pixelFormat = PixelFormat::FromImageRawFormat(image.GetRawFormat(), image.GetGammaSpace());
		std::shared_ptr<MipMaps> mipMaps = std::make_shared<MipMaps>(baseLevelSize, pixelFormat, 1, levelCount);
		mipMaps->GetLevel(0, 0).CopyPixels(image.GetPixels().GetData());

		return mipMaps;
	}

    std::shared_ptr<MipMaps> MipMaps::FromImageArray(const ImageArray& imageArray)
	{
	    Extent3D<unsigned int> baseLevelSize = MakeExtent(imageArray.GetSize(), 1u);

	    unsigned int levelCount = Renderer::ComputeTextureLevelCount(baseLevelSize);

	    PixelFormat pixelFormat = PixelFormat::FromImageRawFormat(imageArray.GetRawFormat(), imageArray.GetGammaSpace());
	    std::shared_ptr<MipMaps> mipMaps = std::make_shared<MipMaps>(baseLevelSize, pixelFormat, imageArray.GetSliceCount(), levelCount);
	    for (unsigned int slice = 0; slice < imageArray.GetSliceCount(); slice++)
	        mipMaps->GetLevel(slice, 0).CopyPixels(imageArray.GetSlice(slice).GetPixels().GetData());

	    return mipMaps;
    }

    MipMaps::MipMaps(const Extent3D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int mipMapCount) :
		mBaseSize(baseSize),
		mPixelFormat(pixelFormat),
		mArrayLayerCount(arrayLayerCount),
		mMipMapCount(mipMapCount)
	{
		mLevels.Reserve(mArrayLayerCount * mMipMapCount);
		for (unsigned int level = 0; level < mMipMapCount; level++)
		{
			Extent3D<unsigned int> levelSize = Renderer::ComputeTextureLevelSize(mBaseSize, level);
			std::size_t levelAllocationSize = mPixelFormat.ComputeAllocationSize(levelSize);

			for (unsigned int layer = 0; layer < mArrayLayerCount; layer++)
				mLevels.Add(MipMapLevel(levelSize, mPixelFormat, levelAllocationSize));
		}
	}

	bool MipMaps::IsComplete() const
	{
		for (unsigned int level = 0; level < mMipMapCount; level++)
		{
			for (unsigned int layer = 0; layer < mArrayLayerCount; layer++)
			{
				if (!GetLevel(layer, level).IsComplete())
					return false;
			}
		}

		return mMipMapCount == Renderer::ComputeTextureLevelCount(mBaseSize);
	}

	bool MipMaps::IsCube() const
	{
		return false;
	}

	bool MipMaps::IsArray() const
	{
		return IsCube() ? mArrayLayerCount > 6 : mArrayLayerCount > 1;
	}

	MipMapLevel& MipMaps::GetLevel(unsigned int slice, unsigned int level)
	{
		return mLevels.At(level * mArrayLayerCount + slice);
	}

	const MipMapLevel& MipMaps::GetLevel(unsigned int slice, unsigned int level) const
	{
		return mLevels.At(level * mArrayLayerCount + slice);
	}

	const Extent3D<unsigned int>& MipMaps::GetBaseSize() const
	{
		return mBaseSize;
	}

	const PixelFormat& MipMaps::GetPixelFormat() const
	{
		return mPixelFormat;
	}

	unsigned int MipMaps::GetArrayLayerCount() const
	{
		return mArrayLayerCount;
	}

	unsigned int MipMaps::GetMipMapCount() const
	{
		return mMipMapCount;
	}
}
