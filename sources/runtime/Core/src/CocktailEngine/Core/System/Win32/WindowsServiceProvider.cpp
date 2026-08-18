#include <CocktailEngine/Core/Application/Application.hpp>
#include <CocktailEngine/Core/System/SystemError.hpp>
#include <CocktailEngine/Core/System/Clipboard/Win32/ClipboardService.hpp>
#include <CocktailEngine/Core/System/Concurrency/Win32/ThreadManager.hpp>
#include <CocktailEngine/Core/System/Console/Win32/ConsoleService.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp>
#include <CocktailEngine/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp>
#include <CocktailEngine/Core/System/Keyboard/Win32/KeyboardService.hpp>
#include <CocktailEngine/Core/System/Library/Win32/LibraryService.hpp>
#include <CocktailEngine/Core/System/Monitor/Win32/MonitorService.hpp>
#include <CocktailEngine/Core/System/Mouse/Win32/MouseService.hpp>
#include <CocktailEngine/Core/System/Win32/WindowsServiceProvider.hpp>
#include <CocktailEngine/Core/System/Window/Win32/WindowEventDispatcher.hpp>
#include <CocktailEngine/Core/System/Window/Win32/WindowFactory.hpp>

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

		application->Singleton<Ck::KeyboardService>([](Application* app) -> UniquePtr<Ck::KeyboardService> {
			return MakeUnique<KeyboardService>(app);
		});

		application->Singleton<Ck::LibraryService, LibraryService>();

		application->Singleton<Ck::LocalFileSystemService, LocalFileSystemService>();
		application->Singleton<Ck::LocalFileSystemDriver, LocalFileSystemDriver>();

		application->Singleton<Ck::MonitorService, MonitorService>();

		application->Singleton<Ck::MouseService>([](Application* app) -> UniquePtr<Ck::MouseService> {
			return MakeUnique<MouseService>(app);
		});

		application->Singleton<Ck::ThreadManager, ThreadManager>(false);

		application->Singleton<WindowEventDispatcher>();

		application->Singleton<Ck::WindowFactory>([&]() -> UniquePtr<Ck::WindowFactory> {
			return MakeUnique<WindowFactory>(mInstanceHandle);
		});

		application->Singleton<WsaService>();
	}

	void WindowsServiceProvider::DoBoot(Application* application)
	{
		application->Resolve<WsaService>();
	}
}
