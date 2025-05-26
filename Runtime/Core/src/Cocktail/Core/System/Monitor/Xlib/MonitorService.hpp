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
		std::shared_ptr<Ck::Monitor> GetPrimaryMonitor() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetMonitorCount() const override;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		std::shared_ptr<Ck::Monitor> GetMonitor(std::size_t index) const override;

		/**
		 * \brief 
		 * \param window 
		 * \return 
		 */
		std::shared_ptr<Ck::Monitor> GetWindowMonitor(const Window& window) const override;

	private:

		::Display* mDisplay;
		bool mExtensionSupported;
		std::shared_ptr<Monitor> mPrimaryMonitor;
		std::vector<std::shared_ptr<Monitor>> mMonitors;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_XLIB_MONITORSERVICE_HPP
