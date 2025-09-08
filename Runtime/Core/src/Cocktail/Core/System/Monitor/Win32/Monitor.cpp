#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Monitor/Win32/Monitor.hpp>

#include "Cocktail/Core/String.hpp"

namespace Ck::Detail::Win32
{
	namespace
	{
		VideoMode DevModeToVideoMode(const DEVMODE& devMode)
		{
			VideoMode videoMode;
			videoMode.Size = MakeExtent<unsigned int>(devMode.dmPelsWidth, devMode.dmPelsHeight);
			videoMode.Frequency = devMode.dmDisplayFrequency;
			videoMode.ColorDepth = devMode.dmBitsPerPel == 32 ? DisplayColorDepth::e32 : DisplayColorDepth::e24;
			videoMode.AlphaDepth = devMode.dmBitsPerPel == 32 ? DisplayAlphaDepth::e8 : DisplayAlphaDepth::e0;

			return videoMode;
		}
	}

	Monitor::Monitor(HMONITOR handle) :
		mHandle(handle)
	{
		ZeroMemory(&mMonitorInfo, sizeof(MONITORINFOEX));
		mMonitorInfo.cbSize = sizeof(MONITORINFOEX);
		if (GetMonitorInfo(mHandle, &mMonitorInfo) == FALSE)
			throw SystemError::GetLastError();
	}

	bool Monitor::IsPrimary() const
	{
		return mMonitorInfo.dwFlags == MONITORINFOF_PRIMARY;
	}

	Extent2D<unsigned int> Monitor::GetSize() const
	{
		unsigned int width = mMonitorInfo.rcMonitor.right - mMonitorInfo.rcMonitor.left;
		unsigned int height = mMonitorInfo.rcMonitor.bottom - mMonitorInfo.rcMonitor.top;
		return MakeExtent(width, height);
	}

	Monitor::Orientation Monitor::GetOrientation() const
	{
		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmFields = DM_DISPLAYORIENTATION;
		devMode.dmDriverExtra = 0;

		if (EnumDisplaySettingsEx(mMonitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode, EDS_RAWMODE) == false)
			throw SystemError::GetLastError();

		switch (devMode.dmDisplayOrientation)
		{
		case DMDO_DEFAULT: return Orientation::Landscape;
		case DMDO_90: return Orientation::RightPortait;
		case DMDO_180: return Orientation::UpsideDownLandscape;
		case DMDO_270: return Orientation::LeftPortait;
		}

		COCKTAIL_UNREACHABLE();
	}

	String Monitor::GetName() const
	{
		return mMonitorInfo.szDevice;
	}

	VideoMode Monitor::GetCurrentVideoMode() const
	{
		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmDriverExtra = 0;

		if (EnumDisplaySettings(mMonitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode) == false)
			throw SystemError::GetLastError();

		return DevModeToVideoMode(devMode);
	}

	void Monitor::SetCurrentVideoMode(const VideoMode& videoMode)
	{
		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmDriverExtra = 0;

		for (DWORD i = 0; EnumDisplaySettings(mMonitorInfo.szDevice, i, &devMode); i++)
		{
			if (DevModeToVideoMode(devMode) != videoMode)
				continue;

			break;
		}

		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY | DM_BITSPERPEL;
		if (ChangeDisplaySettingsEx(mMonitorInfo.szDevice, &devMode, nullptr, 0, nullptr) == false)
			throw SystemError::GetLastError();
	}

	Array<VideoMode> Monitor::GetSupportedVideoModes() const
	{
		Array<VideoMode> videoModes;

		DEVMODE devMode;
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmDriverExtra = 0;

		for (DWORD i = 0; EnumDisplaySettings(mMonitorInfo.szDevice, i, &devMode); i++)
		{
			VideoMode videoMode = DevModeToVideoMode(devMode);
			if (std::find(videoModes.begin(), videoModes.end(), videoMode) == videoModes.end())
				videoModes.Add(videoMode);
		}

		return videoModes;
	}

	void* Monitor::GetSystemHandle() const
	{
		return mHandle;
	}
}
