#include <algorithm>
#include <cmath>

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	std::shared_ptr<MipMaps> MipMaps::FromImage(const Image& image)
	{
		Extent3D<unsigned int> baseLevelSize = MakeExtent(image.GetSize(), 1u);

		unsigned int levelCount = ComputeLevelCount(baseLevelSize);

		std::shared_ptr<MipMaps> mipMaps = std::make_shared<MipMaps>(baseLevelSize, image.GetFormat(), 1, levelCount);
		mipMaps->GetLevel(0, 0).CopyPixels(image.GetPixels().GetData());

		return mipMaps;
	}

	unsigned int MipMaps::ComputeLevelCount(const Extent3D<unsigned int>& baseSize)
	{
		unsigned int maxDimension = std::max({baseSize.Width, baseSize.Height, baseSize.Depth});
		return 1 + std::floor(std::log2(maxDimension));
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

	MipMaps::MipMaps(const Extent3D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int mipMapCount) :
		mBaseSize(baseSize),
		mPixelFormat(pixelFormat),
		mArrayLayerCount(arrayLayerCount),
		mMipMapCount(mipMapCount)
	{
		mLevels.reserve(mArrayLayerCount * mMipMapCount);
		for (unsigned int level = 0; level < mMipMapCount; level++)
		{
			Extent3D<unsigned int> levelSize = ComputeLevelSize(mBaseSize, level);
			std::size_t levelAllocationSize = mPixelFormat.ComputeAllocationSize(levelSize);

			for (unsigned int layer = 0; layer < mArrayLayerCount; layer++)
				mLevels.push_back(MipMapLevel(levelSize, mPixelFormat, levelAllocationSize));
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

		return mMipMapCount == ComputeLevelCount(mBaseSize);
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
