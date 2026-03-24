#include <Cocktail/Core/System/Monitor/Monitor.hpp>

namespace Ck
{
    Monitor* Monitor::GetPrimaryMonitor()
    {
        return ResolveFacadeInstance()->GetPrimaryMonitor();
    }

    unsigned int Monitor::GetMonitorCount()
    {
        return ResolveFacadeInstance()->GetMonitorCount();
    }

    Monitor* Monitor::GetMonitor(unsigned int index)
    {
        return ResolveFacadeInstance()->GetMonitor(index);
    }

    Monitor* Monitor::GetWindowMonitor(const Window& window)
    {
        return ResolveFacadeInstance()->GetWindowMonitor(window);
    }
}
