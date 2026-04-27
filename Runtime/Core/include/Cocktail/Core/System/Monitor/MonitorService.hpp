#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP

namespace Ck
{
    class Monitor;
    class Window;

    /**
     * \brief Service interface for querying system display monitors
     *
     * This service provides access to available monitors on the system,
     * including primary selection and window association.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland).
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
         * \return Pointer to the primary monitor, or nullptr if unavailable
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
         * \return Pointer to the requested monitor, or nullptr if index is invalid
         */
        virtual Monitor* GetMonitor(unsigned int index) const = 0;

        /**
         * \brief Retrieves the monitor associated with a window
         *
         * \param window Window whose display monitor is requested
         * \return Pointer to the monitor currently displaying the window,
         *         or nullptr if not determinable
         */
        virtual Monitor* GetWindowMonitor(const Window& window) const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
