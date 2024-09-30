#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_MONITORSERVICE_HPP

#include <Cocktail/Core/System/Monitor/Monitor.hpp>

namespace Ck
{
	class Window;

	/**
	 * \brief 
	 */
	class MonitorService : public Implements<MonitorService, Interface>
	{
	public:
				
		/**
		 * \brief 
		 * \return 
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
