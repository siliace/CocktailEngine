#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/System/Monitor/MonitorService.hpp>
#include <Cocktail/Core/System/Monitor/VideoMode.hpp>

namespace Ck
{
    /**
     * \brief Represents a physical display monitor and provides access to monitor services
     *
     * This class acts both as an abstraction of a physical monitor and as a static
     * entry point to query available monitors through the underlying MonitorService.
     *
     * Platform behavior may vary depending on the underlying windowing system
     * (e.g. Win32, X11, Wayland).
     */
    class COCKTAIL_CORE_API Monitor : public ServiceFacade<MonitorService>
    {
    public:

        /**
         * \brief Enumerator of possible monitor orientations
         */
        enum class Orientation
        {
            /**
             * \brief Standard horizontal orientation
             */
            Landscape,

            /**
             * \brief Horizontal orientation rotated 180 degrees
             */
            UpsideDownLandscape,

            /**
             * \brief Vertical orientation rotated clockwise
             */
            RightPortrait,

            /**
             * \brief Vertical orientation rotated counter-clockwise
             */
            LeftPortrait
        };

        /**
         * \brief Retrieves the primary monitor of the system
         *
         * \return A pointer to the primary monitor, or nullptr if unavailable
         */
        static Monitor* GetPrimaryMonitor();

        /**
         * \brief Retrieves the number of available monitors
         *
         * \return The total number of monitors detected on the system
         */
        static unsigned int GetMonitorCount();

        /**
         * \brief Retrieves a monitor by index
         *
         * \param index Index of the monitor (range: 0 to GetMonitorCount() - 1)
         *
         * \return A pointer to the requested monitor, or nullptr if the index is invalid
         */
        static Monitor* GetMonitor(unsigned int index);

        /**
         * \brief Retrieves the monitor associated with a given window
         *
         * \param window The window whose monitor is queried
         *
         * \return A pointer to the monitor displaying the window, or nullptr if unavailable
         */
        static Monitor* GetWindowMonitor(const Window& window);

        /**
         * \brief Virtual destructor
         */
        virtual ~Monitor() = default;

        /**
         * \brief Indicates whether this monitor is the primary display
         *
         * \return True if this is the primary monitor, false otherwise
         */
        virtual bool IsPrimary() const = 0;

        /**
         * \brief Retrieves the position of the monitor in pixels
         *
         * \return The monitor size as a 2D extent (width, height)
         */
        virtual Extent2D<int> GetPosition() const = 0;

        /**
         * \brief Retrieves the resolution of the monitor in pixels
         *
         * \return The monitor size as a 2D extent (width, height)
         */
        virtual Extent2D<unsigned int> GetSize() const = 0;

        /**
         * \brief Retrieves the current orientation of the monitor
         *
         * \return The monitor orientation
         */
        virtual Orientation GetOrientation() const = 0;

        /**
         * \brief Retrieves the human-readable name of the monitor
         *
         * \return The monitor name
         */
        virtual String GetName() const = 0;

        /**
         * \brief Retrieves the current video mode of the monitor
         *
         * \return The active video mode
         */
        virtual VideoMode GetCurrentVideoMode() const = 0;

        /**
         * \brief Sets the current video mode of the monitor
         *
         * Changing the video mode may affect resolution, refresh rate,
         * and color depth depending on the platform.
         *
         * \param videoMode The video mode to apply
         */
        virtual void SetCurrentVideoMode(const VideoMode& videoMode) = 0;

        /**
         * \brief Retrieves all video modes supported by the monitor
         *
         * \return An array of supported video modes
         */
        virtual Array<VideoMode> GetSupportedVideoModes() const = 0;

        /**
         * \brief Retrieves the underlying system-specific handle
         *
         * \return A pointer to the native monitor handle
         */
        virtual void* GetSystemHandle() const = 0;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_MONITOR_HPP
