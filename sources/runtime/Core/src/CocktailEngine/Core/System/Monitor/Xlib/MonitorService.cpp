#include <CocktailEngine/Core/Log/Log.hpp>
#include <CocktailEngine/Core/System/Monitor/Xlib/MonitorService.hpp>
#include <CocktailEngine/Core/System/Unix/Xlib/XlibServiceProvider.hpp>
#include <CocktailEngine/Core/System/Window/Xlib/Window.hpp>

namespace Ck::Detail::Xlib
{
    MonitorService::MonitorService(::Display* display) :
        mDisplay(display),
        mExtensionSupported(false),
        mPrimaryMonitor(nullptr)
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

                    mMonitors.Add(Move(monitor));
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

    Ck::Monitor* MonitorService::GetWindowMonitor(const Ck::Window& window) const
    {
        Window::Handle* windowHandle = static_cast<Window::Handle*>(window.GetSystemHandle());

        XWindowAttributes attributes{};
        if (!XGetWindowAttributes(windowHandle->Display, windowHandle->Window, &attributes))
            return nullptr;

        // Get the window position in root-window coordinates.
        int x = 0;
        int y = 0;
        ::Window child = 0;
        if (!XTranslateCoordinates(windowHandle->Display, windowHandle->Window, attributes.root, 0, 0, &x, &y, &child))
            return nullptr;

        unsigned int bestArea = 0;
        Optional<unsigned int> bestMonitorIndex;
        for (unsigned int i = 0; i < mMonitors.GetSize(); i++)
        {
            Monitor* monitor = mMonitors[i].Get();

            Extent2D<int> monitorPosition = monitor->GetPosition();
            Extent2D<unsigned int> monitorSize = monitor->GetSize();

            int left = std::max(x, monitorPosition.Width);
            int top = std::max(y, monitorPosition.Height);

            const int right = std::min(x + attributes.width, monitorPosition.Width + (int) monitorSize.Width);
            const int bottom = std::min(y + attributes.height, monitorPosition.Height + (int) monitorSize.Height);

            const std::int64_t overlapWidth = std::max(0, right - left);
            const std::int64_t overlapHeight = std::max(0, bottom - top);
            const std::int64_t area = overlapWidth * overlapHeight;
            if (area > bestArea)
            {
                bestArea = area;
                bestMonitorIndex = Optional<unsigned int>::Of(i);
            }
        }

        return bestMonitorIndex
            .Map([this](unsigned int index) {
                return mMonitors[index].Get();
            })
            .GetOr(nullptr);
    }

    Signal<PrimaryMonitorChangedEvent>& MonitorService::OnPrimaryMonitorChanged()
    {
        return mOnPrimaryMonitorChanged;
    }

    Signal<MonitorTopologyChangedEvent>& MonitorService::OnMonitorTopologyChanged()
    {
        return mOnMonitorTopologyChanged;
    }
}
