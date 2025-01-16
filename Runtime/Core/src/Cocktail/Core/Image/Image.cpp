#include <cstring>

#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
	Image::Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, ByteArray pixels) :
		mSize(size),
		mPixelFormat(pixelFormat),
		mPixels(std::move(pixels))
	{
		/// Nothing
	}

	Image::Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, const void* pixels) :
		mSize(size),
		mPixelFormat(pixelFormat)
	{
		mPixels = ByteArray(pixels, mPixelFormat.ComputeAllocationSize(mSize));
	}

	const Extent2D<unsigned int>& Image::GetSize() const
	{
		return mSize;
	}

	const PixelFormat& Image::GetFormat() const
	{
		return mPixelFormat;
	}

	const ByteArray& Image::GetPixels() const
	{
		return mPixels;
	}
}
