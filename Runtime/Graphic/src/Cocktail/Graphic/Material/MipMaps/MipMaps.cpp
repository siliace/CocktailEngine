#include <cmath>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	Ref<MipMaps> MipMaps::FromImage(const Image& image, bool generateMissingLevels)
	{
		Extent3D<unsigned int> baseLevelSize = MakeExtent(image.GetSize(), 1u);
		unsigned int levelCount = generateMissingLevels ? ComputeLevelCount(baseLevelSize) : 1;
		Ref<MipMaps> mipMaps = New(baseLevelSize, image.GetFormat(), 1, levelCount);
		mipMaps->GetLevel(0, 0).CopyPixels(image.GetPixels());

		return mipMaps;
	}

	unsigned int MipMaps::ComputeLevelCount(const Extent3D<unsigned>& baseSize)
	{
		return 1 + std::floor(std::log2(std::max({ baseSize.Width, baseSize.Height, baseSize.Depth })));
	}

	Extent3D<unsigned int> MipMaps::ComputeLevelSize(const Extent3D<unsigned int>& size, unsigned int level)
	{
		Extent3D<unsigned int> levelSize = size;
		for (unsigned int i = 0; i < level; i++)
			levelSize /= 2u;

		levelSize.Width = std::max(levelSize.Width, 1u);
		levelSize.Height = std::max(levelSize.Height, 1u);
		levelSize.Depth = std::max(levelSize.Depth, 1u);

		return levelSize;
	}

	MipMaps::MipMaps(const Extent3D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int mipMapCount):
		mBaseSize(baseSize),
		mPixelFormat(pixelFormat),
		mArrayLayerCount(arrayLayerCount),
		mMipMapCount(mipMapCount)
	{
		for (unsigned int level = 0; level < mMipMapCount; level++)
		{
			Extent3D<unsigned int> levelSize = ComputeLevelSize(mBaseSize, level);

			for (unsigned int layer = 0; layer < mArrayLayerCount; layer++)
				mLevels.emplace_back(levelSize, mPixelFormat);
		}
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
		return mLevels.at(level * mArrayLayerCount + slice);
	}

	const MipMapLevel& MipMaps::GetLevel(unsigned int slice, unsigned int level) const
	{
		return mLevels.at(level * mArrayLayerCount + slice);
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