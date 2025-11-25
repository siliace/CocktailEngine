#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/Keyboard/Xlib/KeyboardService.hpp>
#include <Cocktail/Core/System/Monitor/Xlib/MonitorService.hpp>
#include <Cocktail/Core/System/Mouse/Xlib/MouseService.hpp>
#include <Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.hpp>
#include <Cocktail/Core/System/Window/Xlib/AtomManager.hpp>
#include <Cocktail/Core/System/Window/Xlib/WindowFactory.hpp>

namespace Ck::Detail::Xlib
{
	COCKTAIL_DEFINE_LOG_CATEGORY(XlibLogCategory);

    XlibServiceProvider::XlibServiceProvider(Application* application) :
		ServiceProvider(application)
	{
    	XInitThreads();
		mDisplay = ::XOpenDisplay(nullptr);
		XSetErrorHandler([](::Display* display, XErrorEvent* errorEvent) {
			return 0;
		});
	}

	XlibServiceProvider::~XlibServiceProvider()
	{
		XSetErrorHandler(mPreviousErrorHandler);
		XCloseDisplay(mDisplay);
	    XFreeThreads();
	}    

    void XlibServiceProvider::DoRegister(Application* application)
	{
		application->Singleton<Xlib::AtomManager>([display = mDisplay]() {
			return MakeUnique<AtomManager>(display);
		});
		
		int version = 0;
		if (XQueryExtension(mDisplay, "RANDR", &version, &version, &version))
		{
			application->Singleton<Ck::MonitorService>([display = mDisplay]() -> UniquePtr<Ck::MonitorService> {
				return MakeUnique<MonitorService>(display);
			});
		}	
		else
		{
			CK_LOG(XlibLogCategory, LogLevel::Warning, CK_TEXT("X11 extension RANDR is not supported, MonitorService will be disabled"));
		}

		application->Singleton<Ck::MouseService>([display = mDisplay](Application* app) -> UniquePtr<Ck::MouseService> {
			return MakeUnique<MouseService>(display, app);
		});
		
        application->Singleton<Ck::KeyboardService>([display = mDisplay](Application* app) -> UniquePtr<Ck::KeyboardService> {
			return MakeUnique<KeyboardService>(display, app);
		});
		
        application->Singleton<Ck::WindowFactory>([display = mDisplay]() -> UniquePtr<Ck::WindowFactory> {
			return MakeUnique<WindowFactory>(display);
		});
	}
}
