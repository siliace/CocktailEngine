#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/System/Clipboard/Xlib/ClipboardService.hpp>
#include <Cocktail/Core/System/Console/Unix/ConsoleService.hpp>
#include <Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.hpp>
#include <Cocktail/Core/System/Library/Unix/LibraryService.hpp>
#include <Cocktail/Core/System/Unix/UnixServiceProvider.hpp>
#include <Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.hpp>

namespace Ck::Detail::Unix
{  
    UnixServiceProvider::UnixServiceProvider(const Ref<Application>& application) :
        Extends<UnixServiceProvider, ServiceProvider>(application)
    {
		application->RegisterServiceProvider<Xlib::XlibServiceProvider>();
    }

	void UnixServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<Ck::ConsoleService, ConsoleService>();
        
		application->Singleton<Ck::LibraryService, LibraryService>();

		application->Singleton<Ck::LocalFileSystemService, LocalFileSystemService>();
	}
}
