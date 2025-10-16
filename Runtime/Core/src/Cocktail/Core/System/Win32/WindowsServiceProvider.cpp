#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/SystemError.hpp>
#include <Cocktail/Core/System/Clipboard/Win32/ClipboardService.hpp>
#include <Cocktail/Core/System/Concurrency/Win32/ThreadManager.hpp>
#include <Cocktail/Core/System/Console/Win32/ConsoleService.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>
#include <Cocktail/Core/System/Keyboard/Win32/KeyboardService.hpp>
#include <Cocktail/Core/System/Library/Win32/LibraryService.hpp>
#include <Cocktail/Core/System/Monitor/Win32/MonitorService.hpp>
#include <Cocktail/Core/System/Mouse/Win32/MouseService.hpp>
#include <Cocktail/Core/System/Win32/WindowsServiceProvider.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowEventDispatcher.hpp>
#include <Cocktail/Core/System/Window/Win32/WindowFactory.hpp>

namespace Ck::Detail::Win32
{
	COCKTAIL_DEFINE_LOG_CATEGORY(Win32LogCategory);

	class WsaService
	{
	public:

		WsaService()
		{
			WSADATA data;
			if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
				throw SystemError::GetLastError();
		}

		~WsaService()
		{
			WSACleanup();
		}
	};

	WindowsServiceProvider::WindowsServiceProvider(Application* application) :
		ServiceProvider(application)
	{
		mInstanceHandle = GetModuleHandle(nullptr);
	}

	void WindowsServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<Ck::ClipboardService, ClipboardService>();

		application->Singleton<Ck::ConsoleService, ConsoleService>();

		application->Singleton<Ck::KeyboardService>([](Application* app) -> std::unique_ptr<Ck::KeyboardService> {
			return std::make_unique<KeyboardService>(app);
		});

		application->Singleton<Ck::LibraryService, LibraryService>();

		application->Singleton<Ck::LocalFileSystemService, LocalFileSystemService>();
		application->Singleton<Ck::LocalFileSystemDriver, LocalFileSystemDriver>();

		application->Singleton<Ck::MonitorService, MonitorService>();

		application->Singleton<Ck::MouseService>([](Application* app) -> std::unique_ptr<Ck::MouseService> {
			return std::make_unique<MouseService>(app);
		});

		application->Singleton<Ck::ThreadManager, ThreadManager>(false);

		application->Singleton<WindowEventDispatcher>();

		application->Singleton<Ck::WindowFactory>([&]() -> std::unique_ptr<Ck::WindowFactory> {
			return std::make_unique<WindowFactory>(mInstanceHandle);
		});

		application->Singleton<WsaService>();
	}

	void WindowsServiceProvider::DoBoot(Application* application)
	{
		application->Resolve<WsaService>();
	}
}
