#include <cstring>

#include <Cocktail/Core/Image/Image.hpp>

namespace Ck
{
	Image::Image(const Extent2D<unsigned>& size, const PixelFormat& pixelFormat, const Uint8* pixels) :
		mSize(size),
		mPixelFormat(pixelFormat)
	{
		std::size_t allocationSize = mPixelFormat.ComputeAllocationSize(mSize);

		mPixels = std::make_unique<Uint8[]>(allocationSize);
		std::memcpy(mPixels.get(), pixels, allocationSize);
	}

	Image::Image(const Extent2D<unsigned int>& size, const PixelFormat& pixelFormat, std::unique_ptr<Uint8[]> pixels) :
		mSize(size),
		mPixelFormat(pixelFormat),
		mPixels(std::move(pixels))
	{
		/// Nothing
	}

	const Extent2D<unsigned int>& Image::GetSize() const
	{
		return mSize;
	}

	const PixelFormat& Image::GetFormat() const
	{
		return mPixelFormat;
	}

	const void* Image::GetPixels() const
	{
		return mPixels.get();
	}
}
