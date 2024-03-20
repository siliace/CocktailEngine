#include <Cocktail/Core/System/Window/Win32/ImageCursor.hpp>

namespace Ck::Detail::Win32
{
	ImageCursor::ImageCursor(const CursorCreateInfo& createInfo)
	{
		mHandle = nullptr;
	}

	ImageCursor::~ImageCursor()
	{
		DestroyCursor(mHandle);
	}

	Extent2D<unsigned int> ImageCursor::GetHotSpot() const
	{
		return {0, 0};
	}

	void* ImageCursor::GetSystemHandle() const
	{
		return mHandle;
	}
}
