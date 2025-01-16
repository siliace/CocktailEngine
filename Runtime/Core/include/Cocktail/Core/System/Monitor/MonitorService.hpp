#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP

#include <Cocktail/Core/System/Monitor/Monitor.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class MonitorService
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~MonitorService() = default;
				
		/**
		 * \brief Get the primary monitor of the system
		 * On Windows, the primary monitor is the one where is the start button
		 * On Linux, the primary monitor is usually chosen by the window manager
		 * \return The primary monitor
		 */
		virtual Ref<Monitor> GetPrimaryMonitor() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual std::size_t GetMonitorCount() const = 0;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		virtual Ref<Monitor> GetMonitor(std::size_t index) const = 0;

		/**
		 * \brief 
		 * \param window 
		 * \return 
		 */
		virtual Ref<Monitor> GetWindowMonitor(const Window& window) const = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
