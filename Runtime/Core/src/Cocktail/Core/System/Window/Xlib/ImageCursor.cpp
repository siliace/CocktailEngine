#include <Cocktail/Core/System/Unix/Xlib/Xlib.hpp>
#include <Cocktail/Core/System/Window/Xlib/ImageCursor.hpp>

namespace Ck::Detail::Xlib
{
	ImageCursor::ImageCursor(::Display* display, const Image& image, const Extent2D<unsigned int>& hotspot) :	
		mDisplay(display),
		mHotSpot(hotspot)
	{
		Extent2D<unsigned int> size = image.GetSize();

		XcursorImage* cursorImage = XcursorImageCreate(size.Width, size.Height);
		cursorImage->xhot = mHotSpot.Width;
		cursorImage->yhot = mHotSpot.Height;

		PixelFormat imageFormat = image.GetFormat();
		std::size_t allocationSize = imageFormat.ComputeAllocationSize(image.GetSize());
		UniquePtr<unsigned char[]> bgraPixels = MakeUnique<unsigned char[]>(allocationSize);

		const Uint8* pixelOffset = image.GetPixels().GetData();
		std::uint32_t* bitmapOffset = cursorImage->pixels;
		for (std::size_t remaining = size.Width * size.Height; remaining > 0; --remaining, pixelOffset += 4)
			*bitmapOffset++ = pixelOffset[3] << 24 | pixelOffset[0] << 16 | pixelOffset[1] << 8 | pixelOffset[2];

		mHandle = XcursorImageLoadCursor(mDisplay, cursorImage);

		XcursorImageDestroy(cursorImage);
	}

	ImageCursor::~ImageCursor()
	{
		XFreeCursor(mDisplay, mHandle);
	}

	Extent2D<unsigned int> ImageCursor::GetHotSpot() const
	{
		return mHotSpot;
	}

	void* ImageCursor::GetSystemHandle() const
	{
		return reinterpret_cast<void*>(mHandle);
	}
}
