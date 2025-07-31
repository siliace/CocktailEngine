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
			return std::make_unique<AtomManager>(display);
		});
		
		int version = 0;
		if (XQueryExtension(mDisplay, "RANDR", &version, &version, &version))
		{
			application->Singleton<Ck::MonitorService>([display = mDisplay]() -> std::unique_ptr<Ck::MonitorService> {
				return std::make_unique<MonitorService>(display);
			});
		}	
		else
		{
			CK_LOG(XlibLogCategory, LogLevel::Warning, "X11 extension RANDR is not supported, MonitorService will be disabled");
		}

		application->Singleton<Ck::MouseService>([display = mDisplay](Application* app) -> std::unique_ptr<Ck::MouseService> {
			return std::make_unique<MouseService>(display, app);
		});
		
        application->Singleton<Ck::KeyboardService>([display = mDisplay](Application* app) -> std::unique_ptr<Ck::KeyboardService> {
			return std::make_unique<KeyboardService>(display, app);
		});
		
        application->Singleton<Ck::WindowFactory>([display = mDisplay]() -> std::unique_ptr<Ck::WindowFactory> {
			return std::make_unique<WindowFactory>(display);
		});
	}
}
