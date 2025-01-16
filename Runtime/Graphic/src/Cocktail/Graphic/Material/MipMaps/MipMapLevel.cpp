#include <cstring>

#include <Cocktail/Graphic/Material/MipMaps/MipMapLevel.hpp>

namespace Ck
{
	MipMapLevel::MipMapLevel(const Extent3D<unsigned int>& size, const PixelFormat& pixelFormat) :
		mSize(size),
		mPixelFormat(pixelFormat)
	{
		std::size_t allocationSize = mPixelFormat.ComputeAllocationSize(mSize);
		mPixels = std::make_unique<unsigned char[]>(allocationSize);
		std::memset(mPixels.get(), 0, allocationSize);
	}

	MipMapLevel::MipMapLevel(const Extent3D<unsigned int>& size, const PixelFormat& pixelFormat, const void* pixels) :
		mSize(size),
		mPixelFormat(pixelFormat)
	{
		std::size_t allocationSize = mPixelFormat.ComputeAllocationSize(mSize);
		mPixels = std::make_unique<unsigned char[]>(allocationSize);
		std::memcpy(mPixels.get(), pixels, allocationSize);
	}

	void MipMapLevel::CopyPixels(const void* pixels) const
	{
		std::size_t allocationSize = mPixelFormat.ComputeAllocationSize(mSize);
		std::memcpy(mPixels.get(), pixels, allocationSize);
	}

	const Extent3D<unsigned int>& MipMapLevel::GetSize() const
	{
		return mSize;
	}

	const PixelFormat& MipMapLevel::GetPixelFormat() const
	{
		return mPixelFormat;
	}

	const void* MipMapLevel::GetPixels() const
	{
		return mPixels.get();
	}
}
