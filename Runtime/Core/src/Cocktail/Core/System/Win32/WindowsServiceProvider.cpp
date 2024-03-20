#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Clipboard/Win32/ClipboardService.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleService.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>
#include <Cocktail/Core/System/Keyboard/Win32/KeyboardService.hpp>
#include <Cocktail/Core/System/Monitor/Win32/MonitorService.hpp>
#include <Cocktail/Core/System/Mouse/Win32/MouseService.hpp>
#include <Cocktail/Core/System/Win32/WindowsServiceProvider.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowEventDispatcher.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowFactory.hpp>

namespace Ck::Detail::Win32
{
	WindowsServiceProvider::WindowsServiceProvider(const Ref<Application>& application):
		Extends<WindowsServiceProvider, ServiceProvider>(application)
	{
		mInstanceHandle = GetModuleHandle(nullptr);
	}

	void WindowsServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<Ck::ClipboardService, ClipboardService>();

		application->Singleton<Ck::ConsoleService, ConsoleService>();

		application->Singleton<Ck::KeyboardService, KeyboardService>();

		application->Singleton<Ck::LocalFileSystemService, LocalFileSystemService>();

		application->Singleton<Ck::MonitorService, MonitorService>();

		application->Singleton<Ck::MouseService, MouseService>();

		application->Singleton<WindowEventDispatcher>();

		application->Singleton<Ck::WindowFactory>([&]() -> Ref<Ck::WindowFactory> {
			return WindowFactory::New(mInstanceHandle);
		});
	}
}
