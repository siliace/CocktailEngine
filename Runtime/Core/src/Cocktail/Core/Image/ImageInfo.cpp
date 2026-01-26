#include <Cocktail/Core/Image/ImageInfo.hpp>

namespace Ck
{
    ImageInfo::ImageInfo(Extent2D<unsigned int> size, ImageRawFormat::Type format, GammaSpace gammaSpace) :
        mSize(size),
        mFormat(format),
        mGammaSpace(gammaSpace)
    {
        /// Nothing
    }

    Uint64 ImageInfo::GetPixelOffset(unsigned int x, unsigned int y) const
    {
        assert(x < mSize.Width && y < mSize.Height);

        Uint64 offset = 0;
        offset += y * mSize.Width;
        offset += x;

        return offset * ImageRawFormat::GetPixelStride(mFormat);
    }

    Uint64 ImageInfo::GetPixelCount() const
    {
        return mSize.Width * mSize.Height;
    }

    const Extent2D<unsigned int>& ImageInfo::GetSize() const
    {
        return mSize;
    }
    ImageRawFormat::Type ImageInfo::GetRawFormat() const
    {
        return mFormat;
    }
    GammaSpace ImageInfo::GetGammaSpace() const
    {
        return mGammaSpace;
    }
}
