#include <Cocktail/Core/Image/ImageArray.hpp>

namespace Ck
{
    ImageArray::ImageArray(const ImageInfo& imageInfo) :
        mImageInfo(imageInfo),
        mSliceCount(0)
    {
        /// Nothing
    }

    void ImageArray::Add(const Image& image)
    {
        assert(mImageInfo == image.GetImageInfo());

        mPixels.Append(image.GetPixels());
        mSliceCount++;
    }

    void ImageArray::Add(const ImageView& image)
    {
        assert(mImageInfo == image.GetImageInfo());

        mPixels.Append(image.GetPixels());
        mSliceCount++;
    }

    void ImageArray::Add(const ImageArray& imageArray)
    {
        for (unsigned int i = 0; i < imageArray.GetSliceCount(); i++)
            Add(imageArray.GetSlice(i));
    }

    ImageView ImageArray::GetSlice(unsigned int slice) const
    {
        LargeByteArrayView pixelView(mPixels, GetSliceOffset(slice), GetSliceSize());
        return ImageView(mImageInfo.GetSize(), mImageInfo.GetRawFormat(), mImageInfo.GetGammaSpace(), pixelView);
    }

    Extent2D<unsigned int> ImageArray::GetSize() const
    {
        return GetImageInfo().GetSize();
    }

    ImageRawFormat::Type ImageArray::GetRawFormat() const
    {
        return GetImageInfo().GetRawFormat();
    }

    GammaSpace ImageArray::GetGammaSpace() const
    {
        return GetImageInfo().GetGammaSpace();
    }

    const ImageInfo& ImageArray::GetImageInfo() const
    {
        return mImageInfo;
    }

    const LargeByteArray& ImageArray::GetPixels() const
    {
        return mPixels;
    }

    unsigned int ImageArray::GetSliceCount() const
    {
        return mSliceCount;
    }

    Uint64 ImageArray::GetSliceSize() const
    {
        return mImageInfo.GetPixelCount() * ImageRawFormat::GetPixelStride(mImageInfo.GetRawFormat());
    }

    Uint64 ImageArray::GetSliceOffset(unsigned int slice) const
    {
        assert(slice < mSliceCount);

        return GetSliceSize() * slice;
    }
}
