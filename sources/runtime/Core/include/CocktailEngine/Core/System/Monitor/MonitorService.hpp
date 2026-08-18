#ifndef COCKTAILENGINE_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
#define COCKTAILENGINE_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP

#include <CocktailEngine/Core/Signal/Signal.hpp>

namespace Ck
{
    class Monitor;
    class Window;

    /**
     * \brief Event emitted when a monitor refresh selects a different primary monitor.
     *
     * The monitor pointer is non-owning and belongs to the newly published topology.
     * It must not be retained beyond the next topology refresh.
     */
    struct PrimaryMonitorChangedEvent
    {
        /// Non-owning primary monitor from the newly published topology.
        Monitor* Primary;
    };

    /**
     * \brief Event emitted when the display topology is refreshed after a system display change.
     *
     * A topology refresh may invalidate every previously obtained Monitor pointer.
     * Consumers must query MonitorService again after receiving this event.
     */
    struct MonitorTopologyChangedEvent
    {
        /// Reserved for backend-independent topology-change details.
    };

    /**
     * \brief Service interface for querying system display monitors
     *
     * This service provides access to available monitors on the system,
     * including primary selection and window association.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland).
     *
     * Monitor pointers returned by this service are non-owning views. A backend may
     * replace its monitor snapshot during a topology refresh, invalidating all views
     * obtained before that refresh. Callers must not delete or retain them across a
     * MonitorTopologyChangedEvent.
     */
    class MonitorService
    {
    public:

        /**
         * \brief Virtual destructor
         */
        virtual ~MonitorService() = default;

        /**
         * \brief Retrieves the primary monitor of the system
         *
         * \return Non-owning pointer to the primary monitor, or nullptr if unavailable.
         *         The pointer may be invalidated by the next topology refresh.
         */
        virtual Monitor* GetPrimaryMonitor() const = 0;

        /**
         * \brief Retrieves the number of available monitors
         *
         * \return The total number of detected monitors
         */
        virtual unsigned int GetMonitorCount() const = 0;

        /**
         * \brief Retrieves a monitor by index
         *
         * \param index Index of the monitor (0 to GetMonitorCount() - 1)
         *
         * \return Non-owning pointer to the requested monitor, or nullptr if the index is invalid.
         *         The pointer may be invalidated by the next topology refresh.
         */
        virtual Monitor* GetMonitor(unsigned int index) const = 0;

        /**
         * \brief Retrieves the monitor associated with a window
         *
         * \param window Window whose display monitor is requested
         * \return Non-owning pointer to the monitor currently displaying the window,
         *         or nullptr if not determinable. The pointer may be invalidated by the
         *         next topology refresh.
         */
        virtual Monitor* GetWindowMonitor(const Window& window) const = 0;

        /**
         * \brief Retrieves the signal emitted when the primary monitor changes.
         *
         * \return The primary-monitor-change signal.
         */
        virtual Signal<PrimaryMonitorChangedEvent>& OnPrimaryMonitorChanged() = 0;

        /**
         * \brief Retrieves the signal emitted after the backend refreshes display topology.
         *
         * \return The display-topology-change signal.
         */
        virtual Signal<MonitorTopologyChangedEvent>& OnMonitorTopologyChanged() = 0;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
