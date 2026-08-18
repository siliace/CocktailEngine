#ifndef COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP

#include <CocktailEngine/Core/System/Monitor/MonitorService.hpp>
#include <CocktailEngine/Core/System/Monitor/MonitorTopologyHelper.hpp>
#include <CocktailEngine/Core/System/Monitor/Win32/Monitor.hpp>
#include <CocktailEngine/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief
     */
    class MonitorService : public Ck::MonitorService
    {
    public:

        /**
         * \brief Creates the service and enumerates the current display topology
         */
        MonitorService();

        /**
         * \brief Re-enumerates Windows monitors after a display configuration change
         *
         * Refreshing updates the cached monitor list and reports primary-monitor and
         * topology notifications when applicable.
         */
        void RefreshTopology();

        /**
         * \brief
         * \return
         */
        Ck::Monitor* GetPrimaryMonitor() const override;

        /**
         * \brief
         * \return
         */
        unsigned int GetMonitorCount() const override;

        /**
         * \brief
         * \param index
         * \return
         */
        Ck::Monitor* GetMonitor(unsigned int index) const override;

        /**
         * \brief Retrieves the monitor currently associated with a native window.
         *
         * \param windowHandle Native window handle to resolve.
         * \return The matching enumerated monitor, or nullptr when none can be determined.
         */
        Ck::Monitor* GetWindowMonitor(HWND windowHandle) const;

        /**
         * \brief
         * \param window
         * \return
         */
        Ck::Monitor* GetWindowMonitor(const Window& window) const override;

        Signal<PrimaryMonitorChangedEvent>& OnPrimaryMonitorChanged() override;

        Signal<MonitorTopologyChangedEvent>& OnMonitorTopologyChanged() override;

    private:

        /**
         * \brief Collects a complete monitor snapshot during EnumDisplayMonitors().
         */
        struct EnumMonitorParameters
        {
            /// Primary monitor belonging to Monitors, if one was enumerated.
            Monitor* PrimaryMonitor = nullptr;

            /// Monitor wrappers created for the current enumeration pass.
            Array<UniquePtr<Monitor>> Monitors;

            /// Platform-independent description of the current enumeration pass.
            Array<MonitorTopologyHelper::Entry> Topology;
        };

        /**
         * \brief
         * \param handle
         * \param deviceContext
         * \param area
         * \param userParam
         * \return
         */
        static BOOL CALLBACK EnumMonitorCallback(HMONITOR handle, HDC deviceContext, LPRECT area, LPARAM userParam);

        Monitor* mPrimaryMonitor;
        Array<UniquePtr<Monitor>> mMonitors;
        Array<MonitorTopologyHelper::Entry> mTopology;
        Signal<PrimaryMonitorChangedEvent> mOnPrimaryMonitorChanged;
        Signal<MonitorTopologyChangedEvent> mOnMonitorTopologyChanged;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP
