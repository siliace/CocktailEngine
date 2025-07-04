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
			CK_LOG(XlibLogCategory, LogLevel::Warning, "X11 extension RANDR is not supported, MonitorService will be disabled");
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
					std::shared_ptr<Monitor> monitor = std::make_shared<Monitor>(mDisplay, outputInfo->crtc, crtcInfo->outputs[j]);
					if (monitor->IsPrimary())
						mPrimaryMonitor = monitor;

					mMonitors.push_back(std::move(monitor));
				}
				
				XRRFreeOutputInfo(outputInfo);
			}

			XRRFreeCrtcInfo(crtcInfo);
		}

		XRRFreeScreenResources(screenResources);
	}

    std::shared_ptr<Ck::Monitor> MonitorService::GetPrimaryMonitor() const
    {
		return mPrimaryMonitor;
	}

	std::size_t MonitorService::GetMonitorCount() const
	{
		return mMonitors.size();
	}

	std::shared_ptr<Ck::Monitor> MonitorService::GetMonitor(std::size_t index) const
	{
		return mMonitors.at(index);
	}

	std::shared_ptr<Ck::Monitor> MonitorService::GetWindowMonitor(const Window& window) const
	{
		return nullptr;
	}
}
