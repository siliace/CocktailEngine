#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
    Image::Image(const ImageInfo& imageInfo, LargeByteArray pixels) :
        mImageInfo(imageInfo),
        mPixels(std::move(pixels))
    {
    }

    Image::Image(const Extent2D<unsigned int>& size, ImageRawFormat::Type format, GammaSpace gammaSpace, LargeByteArray pixels) :
        mImageInfo(size, format, gammaSpace),
        mPixels(std::move(pixels))
    {
        /// Nothing
    }

    const Byte* Image::GetPixelPointer(unsigned int x, unsigned int y) const
    {
        return GetPixels().GetData() + mImageInfo.GetPixelOffset(x, y);
    }

    LinearColor Image::GetPixelColor(unsigned int x, unsigned int y) const
    {
        return ImageRawFormat::GetPixelColor(GetPixelPointer(x, y), GetRawFormat(), GetGammaSpace());
    }

    LargeArray<LinearColor> Image::AsRGBA() const
    {
        LargeArray<LinearColor> colors;

        Extent2D<unsigned int> size = GetSize();
        colors.Reserve(mImageInfo.GetPixelCount());

        for (Uint64 j = 0; j < size.Height; j++)
        {
            for (Uint64 i = 0; i < size.Width; i++)
            {
                const Byte* pixel = GetPixelPointer(j, i);
                LinearColor color = ImageRawFormat::GetPixelColor(pixel, GetRawFormat(), GetGammaSpace());

                colors.Add(color);
            }
        }

        return colors;
    }

    Extent2D<unsigned int> Image::GetSize() const
    {
        return GetImageInfo().GetSize();
    }

    ImageRawFormat::Type Image::GetRawFormat() const
    {
        return GetImageInfo().GetRawFormat();
    }

    GammaSpace Image::GetGammaSpace() const
    {
        return GetImageInfo().GetGammaSpace();
    }

    const ImageInfo& Image::GetImageInfo() const
    {
        return mImageInfo;
    }

    const LargeByteArray& Image::GetPixels() const
    {
        return mPixels;
    }

    Image::operator ImageView() const
    {
        return ImageView(mImageInfo, LargeByteArrayView(mPixels));
    }
}
