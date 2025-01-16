#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Window/Win32/ImageCursor.hpp>

namespace Ck::Detail::Win32
{
	ImageCursor::ImageCursor(const Image& image, const Extent2D<unsigned int>& hotspot) :
		mHotspot(hotspot)
	{
		Extent2D<unsigned int> size = image.GetSize();
		PixelFormat format = image.GetFormat();

		assert(format.GetLayout() == PixelFormat::Layout::RGBA);

		BITMAPV5HEADER bitmapHeader;
		bitmapHeader.bV5Size = sizeof(BITMAPV5HEADER);
		bitmapHeader.bV5Width = static_cast<int>(size.Width);
		bitmapHeader.bV5Height = -static_cast<int>(size.Height);
		bitmapHeader.bV5Planes = 1;
		bitmapHeader.bV5BitCount = 32;
		bitmapHeader.bV5Compression = BI_BITFIELDS;
		bitmapHeader.bV5RedMask = 0x00ff0000;
		bitmapHeader.bV5GreenMask = 0x0000ff00;
		bitmapHeader.bV5BlueMask = 0x000000ff;
		bitmapHeader.bV5AlphaMask = 0xff000000;

		Uint32* bitmapData = nullptr;
		HDC screenDC = GetDC(nullptr);
		HBITMAP color = CreateDIBSection(screenDC, reinterpret_cast<const BITMAPINFO*>(&bitmapHeader), DIB_RGB_COLORS, reinterpret_cast<void**>(&bitmapData), nullptr, 0);
		ReleaseDC(nullptr, screenDC);

		const Uint8* pixelOffset = image.GetPixels().GetData();
		std::uint32_t* bitmapOffset = bitmapData;
		for (std::size_t remaining = size.Width * size.Height; remaining > 0; --remaining, pixelOffset += 4)
			*bitmapOffset++ = pixelOffset[3] << 24 | pixelOffset[0] << 16 | pixelOffset[1] << 8 | pixelOffset[2];

		if (!color)
			throw SystemError::GetLastError();

		// Create a dummy mask bitmap (it won't be used)
		HBITMAP mask = CreateBitmap(static_cast<int>(size.Width), static_cast<int>(size.Height), 1, 1, nullptr);
		if (!mask)
		{
			DeleteObject(color);
			throw SystemError::GetLastError();
		}

		ICONINFO cursorInfo;
		cursorInfo.fIcon = FALSE;
		cursorInfo.xHotspot = mHotspot.Width;
		cursorInfo.yHotspot = mHotspot.Height;
		cursorInfo.hbmColor = color;
		cursorInfo.hbmMask = mask;
		mHandle = CreateIconIndirect(&cursorInfo);
		
		DeleteObject(color);
		DeleteObject(mask);

		if (!mHandle)
			throw SystemError::GetLastError();
	}

	ImageCursor::~ImageCursor()
	{
		DestroyCursor(mHandle);
	}

	Extent2D<unsigned int> ImageCursor::GetHotSpot() const
	{
		return mHotspot;
	}

	void* ImageCursor::GetSystemHandle() const
	{
		return mHandle;
	}
}
