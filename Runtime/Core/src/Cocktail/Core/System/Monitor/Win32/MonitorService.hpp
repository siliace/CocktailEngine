#ifndef COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP

#include <Cocktail/Core/System/Monitor/MonitorService.hpp>
#include <Cocktail/Core/System/Monitor/Win32/Monitor.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>

namespace Ck::Detail::Win32
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
		MonitorService();

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

		/**
		 * \brief 
		 * \param handle 
		 * \param deviceContext 
		 * \param area 
		 * \param userParam 
		 * \return 
		 */
		static BOOL CALLBACK EnumMonitorCallback(HMONITOR handle, HDC deviceContext, LPRECT area, LPARAM userParam);

		std::shared_ptr<Monitor> mPrimaryMonitor;
		Array<std::shared_ptr<Monitor>> mMonitors;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_MONITOR_WIN32_MONITORSERVICE_HPP
