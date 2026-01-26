#include <Cocktail/Core/Image/ImageView.hpp>

namespace Ck
{
    ImageView::ImageView(const ImageInfo& imageInfo, LargeByteArrayView pixels) :
        mImageInfo(imageInfo),
        mPixels(pixels)
    {
        /// Nothing
    }

    ImageView::ImageView(const Extent2D<unsigned int>& size, ImageRawFormat::Type format, GammaSpace gammaSpace, LargeByteArrayView pixels) :
        mImageInfo(size, format, gammaSpace),
        mPixels(pixels)
    {
        /// Nothing
    }

    const Byte* ImageView::GetPixelPointer(unsigned int x, unsigned int y) const
    {
        return mPixels.GetData() + mImageInfo.GetPixelOffset(x, y);
    }

    LinearColor ImageView::GetPixelColor(unsigned int x, unsigned int y) const
    {
        return ImageRawFormat::GetPixelColor(GetPixelPointer(x, y), GetRawFormat(), GetGammaSpace());
    }

    Extent2D<unsigned int> ImageView::GetSize() const
    {
        return GetImageInfo().GetSize();
    }

    ImageRawFormat::Type ImageView::GetRawFormat() const
    {
        return GetImageInfo().GetRawFormat();
    }

    GammaSpace ImageView::GetGammaSpace() const
    {
        return GetImageInfo().GetGammaSpace();
    }

    const ImageInfo& ImageView::GetImageInfo() const
    {
        return mImageInfo;
    }

    LargeByteArrayView ImageView::GetPixels() const
    {
        return mPixels;
    }
}
