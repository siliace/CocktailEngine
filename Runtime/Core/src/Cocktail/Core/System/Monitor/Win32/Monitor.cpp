#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Monitor/Win32/Monitor.hpp>

namespace Ck::Detail::Win32
{
	Monitor::Monitor(HMONITOR handle, LPRECT area, LPCSTR deviceName, bool primary) :
		mHandle(handle),
		mPrimary(primary)
	{
		mSize.Width = area->right - area->left;
		mSize.Height = area->bottom - area->top;

		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);
		BOOL success = EnumDisplaySettings(deviceName, ENUM_CURRENT_SETTINGS, &devMode);
		if (success == FALSE)
			throw SystemError::GetLastError();

		mVideoMode.Size = MakeExtent<unsigned int>(devMode.dmPelsWidth, devMode.dmPelsHeight);
		mVideoMode.Frequency = devMode.dmDisplayFrequency;
		mVideoMode.ColorDepth = DisplayColorDepth::e32;
		mVideoMode.AlphaDepth = DisplayAlphaDepth::e8;
	}

	bool Monitor::IsPrimary() const
	{
		return mPrimary;
	}

	const VideoMode& Monitor::GetVideoMode() const
	{
		return mVideoMode;
	}

	void* Monitor::GetSystemHandle() const
	{
		return mHandle;
	}
}
