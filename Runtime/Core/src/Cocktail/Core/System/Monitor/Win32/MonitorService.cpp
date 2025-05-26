#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Monitor/Win32/MonitorService.hpp>
#include <Cocktail/Core/System/Win32/Windows.hpp>
#include <Cocktail/Core/System/Window/Window.hpp>

namespace Ck::Detail::Win32
{
	MonitorService::MonitorService()
	{
		LPRECT clip = nullptr;
		HDC deviceContext = GetDC(nullptr);

		BOOL success = EnumDisplayMonitors(
			deviceContext, 
			clip, 
			&MonitorService::EnumMonitorCallback, 
			reinterpret_cast<LPARAM>(this)
		);
	}

	std::shared_ptr<Ck::Monitor> MonitorService::GetPrimaryMonitor() const
	{
		return mPrimaryMonitor;
	}

	std::size_t MonitorService::GetMonitorCount() const
	{
		return mMonitors.size();
	}

	std::shared_ptr<Ck::Monitor> MonitorService::GetMonitor(std::size_t index) const
	{
		return mMonitors.at(index);
	}

	std::shared_ptr<Ck::Monitor> MonitorService::GetWindowMonitor(const Window& window) const
	{
		HMONITOR monitorHandle = MonitorFromWindow(
			static_cast<HWND>(window.GetSystemHandle()), 
			MONITOR_DEFAULTTONULL
		);

		for (const std::shared_ptr<Monitor>& monitor : mMonitors)
		{
			if (monitor->GetSystemHandle() == monitorHandle)
				return monitor;
		}

		return nullptr;
	}

	BOOL CALLBACK MonitorService::EnumMonitorCallback(HMONITOR handle, HDC deviceContext, LPRECT area, LPARAM userParam)
	{
		MonitorService* self = reinterpret_cast<MonitorService*>(userParam);
		
		std::shared_ptr<Monitor> monitor = std::make_shared<Monitor>(handle);

		if (monitor->IsPrimary())
			self->mPrimaryMonitor = monitor;
		self->mMonitors.push_back(std::move(monitor));

		return TRUE;
	}
}
