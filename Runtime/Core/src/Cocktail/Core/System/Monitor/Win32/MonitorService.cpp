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

	Ck::Monitor* MonitorService::GetPrimaryMonitor() const
	{
		return mPrimaryMonitor;
	}

	unsigned int MonitorService::GetMonitorCount() const
	{
		return mMonitors.GetSize();
	}

	Ck::Monitor* MonitorService::GetMonitor(unsigned int index) const
	{
		return mMonitors.At(index).Get();
	}

	Ck::Monitor* MonitorService::GetWindowMonitor(const Window& window) const
	{
		HMONITOR monitorHandle = MonitorFromWindow(
			static_cast<HWND>(window.GetSystemHandle()), 
			MONITOR_DEFAULTTONULL
		);

		for (const UniquePtr<Monitor>& monitor : mMonitors)
		{
			if (monitor->GetSystemHandle() == monitorHandle)
				return monitor.Get();
		}

		return nullptr;
	}

	BOOL CALLBACK MonitorService::EnumMonitorCallback(HMONITOR handle, HDC deviceContext, LPRECT area, LPARAM userParam)
	{
		MonitorService* self = reinterpret_cast<MonitorService*>(userParam);
		
		UniquePtr<Monitor> monitor = MakeUnique<Monitor>(handle);

		if (monitor->IsPrimary())
			self->mPrimaryMonitor = monitor.Get();

		self->mMonitors.Add(std::move(monitor));

		return TRUE;
	}
}
