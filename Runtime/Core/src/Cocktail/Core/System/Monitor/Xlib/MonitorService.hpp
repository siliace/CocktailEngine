#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITORSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITORSERVICE_HPP

#include <vector>

#include <Cocktail/Core/System/Monitor/MonitorService.hpp>
#include <Cocktail/Core/System/Monitor/Xlib/Monitor.hpp>

namespace Ck::Detail::Xlib
{
	/**
	 * \brief 
	 */
	class MonitorService : public Ck::MonitorService
	{
	public:

		/**
		 * \brief 
		 */
		explicit MonitorService(::Display* display);

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
		 * \brief 
		 * \param window 
		 * \return 
		 */
		Ck::Monitor* GetWindowMonitor(const Window& window) const override;

	private:

		::Display* mDisplay;
		bool mExtensionSupported;
		Monitor* mPrimaryMonitor;
		Array<UniquePtr<Monitor>> mMonitors;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITORSERVICE_HPP
