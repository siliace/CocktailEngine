#include <CocktailEngine/Core/Memory/Ptr.hpp>
#include <CocktailEngine/Core/System/Monitor/Win32/MonitorService.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/Window.hpp>

namespace Ck::Detail::Win32
{
    MonitorService::MonitorService() :
        mPrimaryMonitor(nullptr)
    {
        RefreshTopology();
    }

    void MonitorService::RefreshTopology()
    {
        EnumMonitorParameters parameters;
        BOOL success = EnumDisplayMonitors(nullptr, nullptr, &MonitorService::EnumMonitorCallback, reinterpret_cast<LPARAM>(&parameters));
        if (success == FALSE)
            throw SystemError::GetLastError();

        const MonitorTopologyHelper::ChangeFlags changes = MonitorTopologyHelper::Compare(mTopology, parameters.Topology);
        if (!changes)
            return;

        mMonitors = Move(parameters.Monitors);
        mPrimaryMonitor = parameters.PrimaryMonitor;
        mTopology = Move(parameters.Topology);

        if (changes & MonitorTopologyHelper::TopologyChangeFlagBits::Primary)
        {
            PrimaryMonitorChangedEvent event{ mPrimaryMonitor };
            mOnPrimaryMonitorChanged.Emit(event);
        }

        MonitorTopologyChangedEvent event;
        mOnMonitorTopologyChanged.Emit(event);
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

    Ck::Monitor* MonitorService::GetWindowMonitor(HWND windowHandle) const
    {
        HMONITOR monitorHandle = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONULL);
        return mMonitors
            .FindIf([&](Ptr<Monitor> monitor) {
                return monitor->GetSystemHandle() == monitorHandle;
            })
            .Map([](const UniquePtr<Monitor>& monitor) {
                return monitor.Get();
            })
            .GetOr(nullptr);
    }

    Ck::Monitor* MonitorService::GetWindowMonitor(const Window& window) const
    {
        return GetWindowMonitor(static_cast<HWND>(window.GetSystemHandle()));
    }

    Signal<PrimaryMonitorChangedEvent>& MonitorService::OnPrimaryMonitorChanged()
    {
        return mOnPrimaryMonitorChanged;
    }

    Signal<MonitorTopologyChangedEvent>& MonitorService::OnMonitorTopologyChanged()
    {
        return mOnMonitorTopologyChanged;
    }

    BOOL CALLBACK MonitorService::EnumMonitorCallback(HMONITOR handle, HDC deviceContext, LPRECT area, LPARAM userParam)
    {
        EnumMonitorParameters* parameters = reinterpret_cast<EnumMonitorParameters*>(userParam);

        UniquePtr<Monitor> monitor = MakeUnique<Monitor>(handle);
        const bool isPrimary = monitor->IsPrimary();

        parameters->Topology.Add({
            reinterpret_cast<Uint64>(handle),
            isPrimary,
            monitor->GetPosition(),
            monitor->GetSize()
        });

        if (isPrimary)
            parameters->PrimaryMonitor = monitor.Get();

        parameters->Monitors.Add(Move(monitor));

        return TRUE;
    }
}
