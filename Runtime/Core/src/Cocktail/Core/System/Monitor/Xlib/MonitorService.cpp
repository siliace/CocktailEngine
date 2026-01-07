#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/Monitor/Xlib/MonitorService.hpp>
#include <Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.hpp>

namespace Ck::Detail::Xlib
{
	MonitorService::MonitorService(::Display* display) :	
		mDisplay(display)
	{
		int version = 0;
		mExtensionSupported = XQueryExtension(mDisplay, "RANDR", &version, &version, &version);
		if (!mExtensionSupported)
		{
			CK_LOG(XlibLogCategory, LogLevel::Warning, CK_TEXT("X11 extension RANDR is not supported, MonitorService will be disabled"));
			return;
		}	
		
		::Window root = DefaultRootWindow(mDisplay);
		XRRScreenResources* screenResources = XRRGetScreenResourcesCurrent(mDisplay, root);
		for (int i = 0; i < screenResources->ncrtc; i++)
		{
			XRRCrtcInfo* crtcInfo = XRRGetCrtcInfo(mDisplay, screenResources, screenResources->crtcs[i]);
			for (int j = 0; j < crtcInfo->noutput; j++)
			{
				XRROutputInfo* outputInfo = XRRGetOutputInfo(mDisplay, screenResources, crtcInfo->outputs[j]);
				if (outputInfo->connection == RR_Connected)
				{
					UniquePtr<Monitor> monitor = MakeUnique<Monitor>(mDisplay, outputInfo->crtc, crtcInfo->outputs[j]);
					if (monitor->IsPrimary())
						mPrimaryMonitor = monitor.Get();

					mMonitors.Add(std::move(monitor));
				}
				
				XRRFreeOutputInfo(outputInfo);
			}

			XRRFreeCrtcInfo(crtcInfo);
		}

		XRRFreeScreenResources(screenResources);
	}

    Ck::Monitor* MonitorService::GetPrimaryMonitor() const
    {
		return mPrimaryMonitor;
	}

	unsigned int MonitorService::GetMonitorCount() const
	{
		return mMonitors.GetSize();
	}

	Ck::Monitor* MonitorService::GetMonitor(unsigned int index) const
	{
		return mMonitors.At(index).Get();
	}

	Ck::Monitor* MonitorService::GetWindowMonitor(const Window& window) const
	{
		return nullptr;
	}
}
